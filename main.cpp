#include <QCoreApplication>
#include "commandlinefeatures.h"
#include "common.h"
#include "GPHCreator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("GPH creator");
    app.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Creates Linguist \".gph\" files");

    QString errorMessage;
    InputData inputData;
    switch (parseCommandLine(parser, &errorMessage, &inputData)) {
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

    GPHCreator creator;
    creator.addPath(PosArgNames.at(0), parser.positionalArguments().at(0));
    creator.addPath(PosArgNames.at(1), parser.positionalArguments().at(1));

    switch (creator.createGPHFile()) {
    case GPHCreator::CreateOk:
        return 0;
    case GPHCreator::CreateError:
        return 1;
    }

    return 0;
}
