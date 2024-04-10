#include <QCoreApplication>
#include "commandlinefeatures.h"
#include "updater.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("TS updater");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Updates Linguist \".ts\" files");

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

    Updater updater;
    updater.addPath(ArgNames.at(0), parser.positionalArguments().at(0));
    updater.addPath(ArgNames.at(1), parser.positionalArguments().at(1));
    updater.addPath(ArgNames.at(2), parser.positionalArguments().at(2));

    switch (updater.updateFile()) {
    case Updater::UpdateOk:
        return 0;
    case Updater::UpdateError:
        return 1;
    }

    return 0;
}
