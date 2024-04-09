#ifndef FEATURES_H
#define FEATURES_H

#include <QCommandLineParser>
#include <QFileInfo>
#include <QList>

namespace  {

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

enum UpdateTsFileResult
{
    UpdateOk,
    UpdateError
};

QStringList ValidSourceFileExtensions  = { "stf", "sdf" };

}

CommandLineParseResult parseCommandLine(QCommandLineParser &parser, QString *errorMessage)
{
    parser.addPositionalArgument("srcfile", QString("Source translation file (%1).").arg(ValidSourceFileExtensions.join(", ")));
    parser.addPositionalArgument("tsfile", "TS file to update.");

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
    if (positionalArguments.size() > 2) {
        *errorMessage = "More than two arguments specified.";
        return CommandLineError;
    }
    if (positionalArguments.size() < 2) {
        *errorMessage = "Less than two arguments specified.";
        return CommandLineError;
    }

    QFileInfo srcfileinfo(positionalArguments.first());
    if (!srcfileinfo.exists()) {
        *errorMessage = QString("File %1 is not exists.").arg(srcfileinfo.filePath());
        return CommandLineError;
    }
    if (!ValidSourceFileExtensions.contains(srcfileinfo.suffix())) {
        *errorMessage = QString("Source file \"%1\" extension is not supported.\n").arg(srcfileinfo.suffix()) +
                QString("Source files with the extensions \"%1\" are processed.").arg(ValidSourceFileExtensions.join(", "));
        return CommandLineError;
    }

    QFileInfo tsfileinfo(positionalArguments.last());
    if (!tsfileinfo.exists()) {
        *errorMessage = QString("File %1 is not exists.").arg(tsfileinfo.filePath());
        return CommandLineError;
    }
    if (tsfileinfo.suffix() != "ts") {
        *errorMessage = "File extension is not \"ts\": " + tsfileinfo.filePath();
        return CommandLineError;
    }

    return CommandLineOk;
}

UpdateTsFileResult updateTsFile(QStringList srcTsPaths, QString *errorMessage)
{
    QString srcFilePath = srcTsPaths.first();
    QString tsFilePath = srcTsPaths.last();

}

#endif // FEATURES_H
