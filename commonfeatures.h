#ifndef COMMONFEATURES_H
#define COMMONFEATURES_H

#include <QStringList>

namespace {

QStringList ArgNames = { "sdffile", "stffile", "tsfile" };

QList<QStringList> ValidExtensions = { { "sdf" }, { "stf" }, { "ts" }};

QStringList Descriptions = { QString("Default file (\".%1\").").arg(ValidExtensions.at(0).join("\", \".")),
                             QString("Translation file (\".%1\").").arg(ValidExtensions.at(1).join("\", \".")),
                             QString("File to update (\".%1\").").arg(ValidExtensions.at(2).join("\", \".")) };

};


#endif // COMMONFEATURES_H
