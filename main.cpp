/*!
 * \file
 * Tests xqtparser
 */

#include <QDebug>
#include <QFile>

#include "xqtparser.h"

using namespace std;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("xqtclp-test");
    MyQCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption fileOpt (QStringList () << "f"<< "file", "Input file", "file");
    parser.addOption(fileOpt, MyQCommandLineParser::OptionType::InFile);
    QCommandLineOption pidOpt (QStringList () << "p"<< "pid"<< "P", "Example PID", "pid");
    parser.addOption(pidOpt, MyQCommandLineParser::OptionType::Pid);
    parser.addOption(QCommandLineOption(QStringList() << "u"<< "user", "Example user", "user"), MyQCommandLineParser::OptionType::User);
    parser.addOption(QCommandLineOption(QStringList() << "v", "Verbose"));
    parser.addOption(QCommandLineOption(QStringList() << "g"<< "group", "Example user group", "usergroup"), MyQCommandLineParser::OptionType::UserGroup);
    parser.addOption(QCommandLineOption(QStringList() << "d" << "debug", "Set debug mode"));
    QCommandLineOption mycmdOption(QStringList() << "c"<< "command", "Command (load|save|delete)", "command"); //XXX replication!
    parser.addOption(mycmdOption, QStringList() << "load" << "save" << "delete");
    parser.addOption(QCommandLineOption (QStringList() << "chdir", "chdir to directory", "dir"), MyQCommandLineParser::OptionType::Directory);
    parser.addOption(QCommandLineOption(QStringList() << "s"<< "sudo", "Sudoer command", "cmd"), MyQCommandLineParser::OptionType::Command);

    parser.process(app);

    if (parser.isSet(mycmdOption)) {
        qDebug() << "Command to run: " << parser.value(mycmdOption);
    } else {
        qWarning() << "Nothing to do.";
    }

    return EXIT_SUCCESS;
}
