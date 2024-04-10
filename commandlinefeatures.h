#ifndef FEATURES_H
#define FEATURES_H

#include <QCommandLineParser>
#include <QFileInfo>

#include "commonfeatures.h"

namespace  {

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

//int checkFile(int argNumber, const QStringList& positionalArguments, QString *errorMessage);

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage);

}
#endif // FEATURES_H
