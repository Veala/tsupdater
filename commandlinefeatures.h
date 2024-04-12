#ifndef COMMANDLINEFEATURES_H
#define COMMANDLINEFEATURES_H

#include "common.h"
#include <QCommandLineParser>
#include <QFileInfo>

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

int checkFile(int argNumber, const QStringList& positionalArguments, QString *errorMessage);

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage, InputData *inputData);

#endif // COMMANDLINEFEATURES_H
