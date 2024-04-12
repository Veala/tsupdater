#ifndef GPHCreator_H
#define GPHCreator_H

#include <QCommandLineParser>
#include <QDomDocument>
#include <QMap>

namespace {

QStringList PosArgNames = { "sdffile", "stffile" };

QList<QStringList> PosArgValidExtensions = { { "sdf" }, { "stf" } };

QStringList PosArgDescriptions = { QString("Default \".%1\" file.").arg(PosArgValidExtensions.at(0).join("\", \".")),
                            QString("Translation \".%1\" file.").arg(PosArgValidExtensions.at(1).join("\", \"."))};

}

enum GPHCreateResult
{
    CreateOk,
    CreateError
};

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
};

class GPHCreator
{
public:
    GPHCreator();

    CommandLineParseResult parseCommandLine();
    virtual GPHCreateResult createGPHFile();

private:
    QString m_SDFFilePath;
    QString m_STFFilePath;
    QString m_GPHFileDir;
    QString m_GPHFileName;
    QString m_languageCode;

    QCommandLineParser m_commandLineParser;

    struct Data
    {
        QString text;
        QString description;
    };

    QMap<QString, Data> m_sdfData;
    QMap<QString, Data> m_stfData;

private:
    int checkPositionalArgument(int argNumber, const QStringList& positionalArguments);
    QString getExtension(QString argument);
    void writeError(QString errorMessage);
    int openDomDocument(QDomDocument& domDocument, QString xmlFilePath);
};


#endif // GPHCreator_H
