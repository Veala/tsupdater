#ifndef COMMANDLINEFEATURES_H
#define COMMANDLINEFEATURES_H

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

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage);

#endif // COMMANDLINEFEATURES_H
