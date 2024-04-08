#ifndef FEATURES_H
#define FEATURES_H

#include <QCommandLineParser>

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage)
{
    parser.addPositionalArgument("srcfile", QCoreApplication::translate("args", "Source translation file."));
    parser.addPositionalArgument("tsfile", QCoreApplication::translate("args", "TS file to update."));

    const QCommandLineOption helpOption = parser.addHelpOption();
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
    if (positionalArguments.size() > 1) {
        *errorMessage = "Several 'name' arguments specified.";
        return CommandLineError;
    }
    query->name = positionalArguments.first();

    return CommandLineOk;
}

#endif // FEATURES_H
