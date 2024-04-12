#include <QCoreApplication>
#include "GPHCreator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("GPH Creator");
    app.setApplicationVersion("1.0");

    GPHCreator gphCreator;
    switch (gphCreator.parseCommandLine()) {
    case CommandLineOk:
        break;
    case CommandLineError:
        return 1;
    case CommandLineVersionRequested:
        return 0;
    }

    switch (gphCreator.createGPHFile()) {
    case CreateOk:
        return 0;
    case CreateError:
        return 1;
    }

    return 0;
}
