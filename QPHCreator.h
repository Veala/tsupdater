#ifndef QPHCreator_H
#define QPHCreator_H

#include <QCommandLineParser>
#include <QDomDocument>
#include <QMap>

namespace {

QStringList PosArgNames = { "sdffile", "stffile" };

QList<QStringList> PosArgValidExtensions = { { "sdf" }, { "stf" } };

QStringList PosArgDescriptions = { QString("Default \".%1\" file.").arg(PosArgValidExtensions.at(0).join("\", \".")),
                            QString("Translation \".%1\" file.").arg(PosArgValidExtensions.at(1).join("\", \"."))};

}

enum QPHCreateResult
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

class QPHCreator
{
public:
    QPHCreator();

    CommandLineParseResult parseCommandLine();
    virtual QPHCreateResult createQPHFile();

private:
    QString m_SDFFilePath;
    QString m_STFFilePath;
    QString m_QPHFileDir;
    QString m_QPHFileName;
    QString m_sourceLanguageCode;
    QString m_translationLanguageCode;

    QCommandLineParser m_commandLineParser;

    struct TextEntry
    {
    public:
        TextEntry(QString text = "", QString description = "") : m_text(text), m_description(description) { }
        QString m_text;
        QString m_description;
    };

    QMap<QString, TextEntry> m_sdfData;
    QMap<QString, TextEntry> m_stfData;

private:
    int checkPositionalArgument(int argNumber, const QStringList& positionalArguments);
    QString getExtension(QString argument);
    void writeError(QString errorMessage);
    int openDomDocument(QDomDocument& domDocument, QString xmlFilePath);
};


#endif // QPHCreator_H
