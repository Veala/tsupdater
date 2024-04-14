#include <QCoreApplication>
#include "QPHCreator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("ICS");
    app.setApplicationName("QPH Creator");
    app.setApplicationVersion("1.0");

    QPHCreator qphCreator;
    switch (qphCreator.parseCommandLine()) {
    case CommandLineOk:
        break;
    case CommandLineError:
        return 1;
    case CommandLineVersionRequested:
        return 0;
    }

    switch (qphCreator.createQPHFile()) {
    case CreateOk:
        return 0;
    case CreateError:
        return 1;
    }

    return 0;
}
