#include "updater.h"

Updater::Updater()
{

}

Updater::FileUpdateResult Updater::updateFile()
{
    if (getExtension("sdffile") == "sdf" && getExtension("stffile") == "stf" && getExtension("tsfile") == "ts") {

        printf("File \"%s\" is updated\n", qPrintable(getPath("tsfile")));
        return Updater::UpdateOk;
    } else {
        fputs(qPrintable(QString("Unknown algorithm")), stderr);
        fputs("\n\n", stderr);
        return Updater::UpdateError;
    }
};