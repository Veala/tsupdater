#ifndef COMMON_H
#define COMMON_H

#include <QStringList>

struct InputData {
    QString sdfFilePath;
    QString stfFilePath;
    QString gphFilePath;
};

namespace {

QStringList PosArgNames = { "sdffile", "stffile" };

QList<QStringList> PosArgValidExtensions = { { "sdf" }, { "stf" } };

QStringList PosArgDescriptions = { QString("Default \".%1\" file.").arg(PosArgValidExtensions.at(0).join("\", \".")),
                            QString("Translation \".%1\" file.").arg(PosArgValidExtensions.at(1).join("\", \"."))};

}

#endif // COMMON_H
