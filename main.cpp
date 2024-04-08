#include <QCoreApplication>
#include <tsupdaterfeatures.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("TS file updater");
    app.setApplicationVersion("1.0");


    QCommandLineParser parser;
    parser.setApplicationDescription("Updates Linguist .ts files");
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
