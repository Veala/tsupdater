#include "commandlinefeatures.h"

int checkPositionalArgument(int argNumber, const QStringList& positionalArguments, QString *errorMessage)
{
    QString filePath = positionalArguments.at(argNumber);
    QFileInfo fileinfo(filePath);
    if (!fileinfo.exists()) {
        *errorMessage = QString("Error: \"%1\" does not exist.").arg(fileinfo.filePath());
        return 1;
    }
    if (!PosArgValidExtensions.at(argNumber).contains(fileinfo.suffix())) {
        *errorMessage = QString("Error: \"%1\"\n").arg(fileinfo.filePath()) +
                        QString("Valid extensions for %1: \".%2\".").arg(PosArgNames.at(argNumber)).arg(PosArgValidExtensions.at(argNumber).join("\", \"."));
        return 1;
    }
    return 0;
}

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage, InputData *inputData)
{
    for (int argNumber = 0; argNumber < PosArgNames.size(); argNumber++)
        parser.addPositionalArgument(PosArgNames.at(argNumber), PosArgDescriptions.at(argNumber));

    const QCommandLineOption directoryOption(QStringList() << "d" << "dir", "Directory to create the \".gph\" file (default is \"./\").", "directory", "./");
    parser.addOption(directoryOption);

    const QCommandLineOption nameOption(QStringList() << "o", "Place the \".gph\" glossary output in file \"outfile\"", "outfile", "glossary.gph");
    parser.addOption(nameOption);

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
    if (positionalArguments.size() > PosArgNames.size()) {
        *errorMessage = QString("More than %1 arguments specified.").arg(PosArgNames.size());
        return CommandLineError;
    }
    if (positionalArguments.size() < PosArgNames.size()) {
        *errorMessage = QString("Less than %1 arguments specified.").arg(PosArgNames.size());
        return CommandLineError;
    }

    if (checkPositionalArgument(0, positionalArguments, errorMessage))
        return CommandLineError;
    if (checkPositionalArgument(1, positionalArguments, errorMessage))
        return CommandLineError;

    inputData->sdfFilePath = parser.positionalArguments().at(0);
    inputData->stfFilePath = parser.positionalArguments().at(1);

    if (parser.isSet(directoryOption))
        return CommandLineVersionRequested;

    return CommandLineOk;
}
