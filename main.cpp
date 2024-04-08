#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("TS file updater");
    app.setApplicationVersion("1.0");


    QCommandLineParser parser;
    parser.setApplicationDescription("Translates Linguist .ts files");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("srcfile", QCoreApplication::translate("args", "Source translation file."));
    parser.addPositionalArgument("tsfile", QCoreApplication::translate("args", "TS file to update."));

    QCommandLineOption localeOption({"l", "languageCode"}, "Language code ex: \"fe\" or \"fr_CA\"", "pigLatin");
    parser.addOption(localeOption);
    parser.process(app);


    const QStringList opt = parser.optionNames();
    QString locale;
    bool usePigLatin = false;
    bool success =false;

    if (parser.isSet(localeOption)) {
        locale = parser.value(localeOption);
    } else  {
        //Use PigLatin
        usePigLatin = true;
    }


    const QStringList args = parser.positionalArguments();
    if (args.isEmpty()) {
        parser.showHelp(1);
    }
    return 0;
}
