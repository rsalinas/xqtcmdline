#include <QCommandLineParser>
#include <QMap>
#include <iostream>

/*!
 * \brief MyQCommandLineParser extends QCommandLineParser to offer option typing.
 *        This is used later to offer seamless bash completion.
 *
 * \author Raúl Salinas-Monteagudo <rausalinas@gmail.com>
 */

class MyQCommandLineParser : public QCommandLineParser{
public:
    /*!
     * \brief The DataType enum defines the different kinds of options.
     *
     *  They are mostly based on bash's compgen.
     */
    enum OptionType {
        FreeText, Command, Directory, Job, User, UserGroup, Pid, File, InFile,
        OutFile, EnvironVar, Service, RuntimeCustom, Enumerated
    };

    /*!
     * \brief addOption adds a typed option.
     * \param option The option to add.
     * \param type The type of the option.
     */
    void addOption(const QCommandLineOption &option, OptionType type = FreeText);

    /*!
     * \brief Adds an option of time "enumerated". The enumeration is provided.
     * \param option The option to add.
     * \param customCompletions The possible values.
     */
    void addOption(const QCommandLineOption &option, QStringList customCompletions);

    /*!
     * \brief An enriched process() that implements the completion mechanism
     *        when the program is called with the "-C" option.
     * \param app The QCoreApplication context.
     */
    void process(const QCoreApplication &app);


protected:
    /*!
     * \brief findOptionByName finds an option given a name.
     * \param searchedName The option searched (short/long, whatever).
     * \return The associated QCommandLineOption.
     *
     * A raw pointer is returned because we assume the processing of this return
     * value will outlive the command-line parsing information (usually, the
     * full lifetime of the program).
     *
     * We also need to be able to indicate failure to find it.
     */
    QCommandLineOption const *findOptionByName(const QString &searchedName);

    /*!
     * \brief Dumps possible completion based on the current prefix and the previous token.
     * \param prefix The word being written now. It comes from bash completion's -C (argv[3]).
     * \param prev The previous token.  (argv[4] from bash completion's -C).
     */
    void dumpCompletions(const QString &prefix, const QString &prev);

    /*!
     * \brief Returns all strings of the given string list that begin with the given prefix.
     * \param list The list.
     * \param prefix The filtering prefix.
     * \return The filtered list.
     */
    QStringList getPrefixed(const QStringList &list, const QString &prefix);

    /*!
     * \brief A copy of all the options added, needed to traverse it from outside.
     *
     *  This should be part of the main class.
     */
    QList<QCommandLineOption> m_options;


    /*!
     * \brief optionTypes associates option names to types.
     *
     *        This should be part of the QCommandLineOption.  This solution
     *        is a workaround to avoid modifying that class.
     */
    QMap<QString, OptionType> m_optionTypes;

    /*!
     * \brief Contains the enumerated completions if any.
     *
     *        This should be part of the QCommandLineOption.  This solution
     *        is a workaround to avoid modifying that class.
     */
    QMap<QString, QStringList> m_optionEnumerates;
};
