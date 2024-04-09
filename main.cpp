#include <QCoreApplication>
#include <tsupdaterfeatures.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("TS updater");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Updates Linguist \"ts\" files");

    QString errorMessage;
    switch (parseCommandLine(parser, &errorMessage)) {
    case CommandLineOk:
        break;
    case CommandLineError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        return 1;
    case CommandLineVersionRequested:
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
               qPrintable(QCoreApplication::applicationVersion()));
        return 0;
    case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

    const QStringList srcAndTsPaths = parser.positionalArguments();
    switch (updateTsFile(srcAndTsPaths, &errorMessage)) {
    case UpdateOk:
        printf("File %s is updated\n", qPrintable(srcAndTsPaths.last()));
        return 0;
    case UpdateError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        return 1;
    }

    return 0;
}
