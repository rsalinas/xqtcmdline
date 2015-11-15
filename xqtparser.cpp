#include <iostream>

#include "xqtparser.h"

void MyQCommandLineParser::addOption(const QCommandLineOption &commandLineOption, OptionType type) {
    QCommandLineParser::addOption(commandLineOption);
    m_options << commandLineOption;
    m_optionTypes[commandLineOption.names()[0]] = type;
}

void MyQCommandLineParser::addOption(const QCommandLineOption &commandLineOption, QStringList customCompletions) {
    MyQCommandLineParser::addOption(commandLineOption, OptionType::Enumerated);
    m_optionEnumerates[commandLineOption.names()[0]] = customCompletions;
}

static QString quote (const QString &s) {
    auto result = s;
    result.replace(QRegExp("\""), "\"");
    return "\""+result+"\"";
}

/*!
 * \brief Compgen wraps calls to bash's compgen.
 *
 * All of these completions could be implemented in a purely native way, which
 * would be more portable but much longer.
 */
class Compgen {
public:
    bool isSupported(MyQCommandLineParser::OptionType type) const {
        return compgenTable.contains(type);
    }

    Compgen() {
        compgenTable[MyQCommandLineParser::OptionType::File] = "f";
        compgenTable[MyQCommandLineParser::OptionType::InFile] = "f";
        compgenTable[MyQCommandLineParser::OptionType::OutFile] = "f";
        compgenTable[MyQCommandLineParser::OptionType::User] = "u";
        compgenTable[MyQCommandLineParser::OptionType::Command] = "c";
        compgenTable[MyQCommandLineParser::OptionType::Directory] = "d";
        compgenTable[MyQCommandLineParser::OptionType::EnvironVar] = "e";
        compgenTable[MyQCommandLineParser::OptionType::UserGroup] = "g";
        compgenTable[MyQCommandLineParser::OptionType::Service] = "s";
    }

    void run(MyQCommandLineParser::OptionType type, const QString &prefix) const {
        /*
         * Is this completely secure?  Is the quoting right?
         * Anyway, it's the user's context and the user's program. (no setuid)
         */
        auto l=("bash -c "+quote("compgen  -"+compgenTable[type]+" "+quote(prefix)));
        system(l.toStdString().c_str());
    }

    static const Compgen instance;

private:
    QMap<MyQCommandLineParser::OptionType, QString> compgenTable;
};

const Compgen Compgen::instance;

void MyQCommandLineParser::process(const QCoreApplication &app) {
    if (app.arguments().length() == 5 && app.arguments()[1] == QStringLiteral("-C")) {
        dumpCompletions(app.arguments()[3], app.arguments()[4]);
        exit(EXIT_SUCCESS);
    }

    QCommandLineOption bashCompleteLineOption(QStringList() << "setenv", "Sets the environment to use this tool.", "");
    addOption(bashCompleteLineOption);
    QCommandLineParser::process(app);

    if (isSet(bashCompleteLineOption)) {
        std::cout << QString("complete -C \"%1 -C\" %1;").arg(basename(app.arguments()[0].toStdString().c_str())).toStdString()<<std::endl;

        auto path = QString(getenv("PATH")).split(":");
        auto mypath = app.applicationDirPath();
        if (!path.contains(mypath))
            std::cout << QString("export PATH=$PATH:%1;").arg(mypath).toStdString() << std::endl;

        exit(EXIT_SUCCESS);
    }

    /* Extra service: we can check the enumerate types */
    for (const auto &option : m_options) {
        if (isSet(option) && m_optionTypes[option.names()[0]] == OptionType::Enumerated && ! m_optionEnumerates[option.names()[0]].contains(value(option))) {
            std::clog << QString("Bad argument for «%1»: %2").arg(option.names()[0], value(option)).toStdString() << std::endl << std::endl;
            showHelp(EXIT_FAILURE);
        }
    }
}

QString removeByRegExp(QString string, QRegExp regExp) {
    return string.remove(regExp);
}

void MyQCommandLineParser::dumpCompletions(const QString &prefix, const QString &prev)
{
    /* If the previous element was an option...*/
    if (prev.startsWith("-")) {
        auto prevWithoutLeadingDashes = removeByRegExp(prev, QRegExp("^-*"));
        const QCommandLineOption *previousOption = findOptionByName(prevWithoutLeadingDashes);
        /* If we recognize the option ...*/
        if (previousOption && previousOption->valueName().length()!=0) {
            auto firstName = previousOption->names()[0];
            /* If Compgen can handle it, let it do it  */
            if (Compgen::instance.isSupported((m_optionTypes[firstName]))) {
                Compgen::instance.run(m_optionTypes[firstName ], prefix);
                return;
            }
            /* If it is an enumerated, process it */
            if (m_optionEnumerates.contains(previousOption->names()[0])) {
                for (const auto &c : getPrefixed(m_optionEnumerates[previousOption->names()[0]], prefix)) {
                    std::cout <<c.toStdString() << '\n';
                }
                return;
            }
        }
    }

    if (prefix.length() == 0 || prefix == "-") {
        for (const auto &o : m_options) {
            auto longest = o.names()[0];
            for (const auto &n : o.names()) {
                if (n.length() > longest.length())
                    longest = n;
            }
            std::cout << (longest.length() == 1 ? "-" : "--") + longest.toStdString() << std::endl;
        }
    } else if (prefix.startsWith("--")) {
        for (const auto &option : m_options) {
            for (const auto &name : option.names()) {
                if (name.length() > 1 && name.startsWith(prefix.mid(2))) {
                    std::cout << "--" << name.toStdString() << std::endl;
                }
            }
        }
    }
}

QCommandLineOption const *MyQCommandLineParser::findOptionByName(const QString &searchedName) {
    for (const auto &option : m_options) {
        for (auto name : option.names()) {
            if (name == searchedName)
                return &option;
        }
    }
    return nullptr;
}

QStringList MyQCommandLineParser::getPrefixed(const QStringList &list, const QString &prefix) {
    QStringList returnList;
    for (const auto &string : list) {
        if (string.startsWith(prefix))
            returnList.append(string);
    }
    return returnList;
}
