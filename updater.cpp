#include "updater.h"

Updater::Updater()
{

}

void Updater::addPath(QString argType, QString path)
{
    m_paths.insert(argType, path);
}

QString Updater::getPath(QString argType)
{
    return m_paths.value(argType);
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
}

QString Updater::getExtension(QString arg)
{
    QFileInfo fileInfo(m_paths.value(arg));
    return fileInfo.suffix();
};
