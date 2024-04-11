#include "commandlinefeatures.h"
#include "common.h"

int checkFile(int argNumber, const QStringList& positionalArguments, QString *errorMessage)
{
    QString filePath = positionalArguments.at(argNumber);
    QFileInfo fileinfo(filePath);
    if (!fileinfo.exists()) {
        *errorMessage = QString("Error: \"%1\" does not exist.").arg(fileinfo.filePath());
        return 1;
    }
    if (!ValidExtensions.at(argNumber).contains(fileinfo.suffix())) {
        *errorMessage = QString("Error: \"%1\"\n").arg(fileinfo.filePath()) +
                        QString("Valid extensions for %1: \".%2\".").arg(ArgNames.at(argNumber)).arg(ValidExtensions.at(argNumber).join("\", \"."));
        return 1;
    }
    return 0;
}

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage)
{
    for (int argNumber = 0; argNumber < ArgNames.size(); argNumber++)
        parser.addPositionalArgument(ArgNames.at(argNumber), Descriptions.at(argNumber));

    const QCommandLineOption helpOption(QStringList() << "h" << "help", "Displays help on commandline options.");
    parser.addOption(helpOption);
    const QCommandLineOption versionOption = parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        *errorMessage = parser.errorText();
        return CommandLineError;
    }

    if (parser.isSet(versionOption))
        return CommandLineVersionRequested;

    if (parser.isSet(helpOption))
        return CommandLineHelpRequested;

    const QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        *errorMessage = "No arguments";
        return CommandLineError;
    }
    if (positionalArguments.size() > ArgNames.size()) {
        *errorMessage = QString("More than %1 arguments specified.").arg(ArgNames.size());
        return CommandLineError;
    }
    if (positionalArguments.size() < ArgNames.size()) {
        *errorMessage = QString("Less than %1 arguments specified.").arg(ArgNames.size());
        return CommandLineError;
    }

    if (checkFile(0, positionalArguments, errorMessage))
        return CommandLineError;
    if (checkFile(1, positionalArguments, errorMessage))
        return CommandLineError;
    if (checkFile(2, positionalArguments, errorMessage))
        return CommandLineError;

    return CommandLineOk;
}
