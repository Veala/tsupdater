#include "QPHCreator.h"
#include <QFileInfo>
#include <QDir>

//#define debug

QPHCreator::QPHCreator()
{

}

int QPHCreator::checkPositionalArgument(int argNumber, const QStringList& positionalArguments)
{
    QString filePath = positionalArguments.at(argNumber);
    QFileInfo fileinfo(filePath);
    if (!fileinfo.exists()) {
        writeError(QString("Error: \"%1\" doesn't exist.").arg(fileinfo.filePath()));
        return 1;
    }
    if (!PosArgValidExtensions.at(argNumber).contains(fileinfo.suffix())) {
        writeError(QString("Error: \"%1\"\n").arg(fileinfo.filePath()) +
                    QString("Valid extensions for \"%1\" argument: \".%2\".").arg(PosArgNames.at(argNumber)).arg(PosArgValidExtensions.at(argNumber).join("\", \".")));
        return 1;
    }
    return 0;
}

CommandLineParseResult QPHCreator::parseCommandLine()
{
    m_commandLineParser.setApplicationDescription("QPHCreator creates Linguist \".qph\" files");

    for (int argNumber = 0; argNumber < PosArgNames.size(); argNumber++)
        m_commandLineParser.addPositionalArgument(PosArgNames.at(argNumber), PosArgDescriptions.at(argNumber));

    const QCommandLineOption directoryOption(QStringList() << "d" << "dir", "Directory to create the \".qph\" file (default is \"./\").", "directory", ".");
    m_commandLineParser.addOption(directoryOption);

    const QCommandLineOption nameOption(QStringList() << "o", "Place the \".qph\" glossary output in file <outfile> (default is \"glossary.qph\").", "outfile", "glossary.qph");
    m_commandLineParser.addOption(nameOption);

    const QCommandLineOption sourceLanguageCodeOption(QStringList() << "s", "Source language code (\"en_US\", \"fr_FR\", \"ru_RU\" etc., default is empty).\n" \
                                                                    "NOTE: QPHCreator doesn't check the language code.", "source language code", "");
    m_commandLineParser.addOption(sourceLanguageCodeOption);

    const QCommandLineOption translationLanguageCodeOption(QStringList() << "t", "Translation language code (\"en_US\", \"fr_FR\", \"ru_RU\" etc., default is empty).\n" \
                                                                    "NOTE: QPHCreator doesn't check the language code.", "translation language code", "");
    m_commandLineParser.addOption(translationLanguageCodeOption);

    const QCommandLineOption helpOption(QStringList() << "h" << "help", "Displays help on commandline options.");
    m_commandLineParser.addOption(helpOption);

    const QCommandLineOption versionOption = m_commandLineParser.addVersionOption();

    if (!m_commandLineParser.parse(QCoreApplication::arguments())) {
        writeError(m_commandLineParser.errorText());
        return CommandLineError;
    }

    if (m_commandLineParser.isSet(versionOption)) {
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
               qPrintable(QCoreApplication::applicationVersion()));
        return CommandLineVersionRequested;
    }

    if (m_commandLineParser.isSet(helpOption)) {
        m_commandLineParser.showHelp();
        Q_UNREACHABLE();
    }

    const QStringList positionalArguments = m_commandLineParser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        writeError("Error: No arguments");
        return CommandLineError;
    }
    if (positionalArguments.size() > PosArgNames.size()) {
        writeError(QString("Error: More than %1 arguments specified.").arg(PosArgNames.size()));
        return CommandLineError;
    }
    if (positionalArguments.size() < PosArgNames.size()) {
        writeError(QString("Error: Less than %1 arguments specified.").arg(PosArgNames.size()));
        return CommandLineError;
    }

    if (checkPositionalArgument(0, positionalArguments))
        return CommandLineError;
    if (checkPositionalArgument(1, positionalArguments))
        return CommandLineError;

    m_SDFFilePath = m_commandLineParser.positionalArguments().at(0);
    m_STFFilePath = m_commandLineParser.positionalArguments().at(1);

    if (m_commandLineParser.isSet(directoryOption)) {
        QDir dir(m_commandLineParser.value(directoryOption));
        if (!dir.exists()) {
            writeError(QString("Error: \"%1\" doesn't exist.").arg(dir.path()));
            return CommandLineError;
        }
    }
    m_QPHFileDir = m_commandLineParser.value(directoryOption);

    if (m_commandLineParser.isSet(nameOption)) {
        QString qphFileName = m_commandLineParser.value(nameOption);
        if (getExtension(qphFileName) != "qph") {
            writeError(QString("Error: \"%1\"\n").arg(qphFileName) +
                        QString("Valid extensions for output file: \".qph\"."));

            return CommandLineError;
        }
    }

    m_QPHFileName = m_commandLineParser.value(nameOption);

    m_sourceLanguageCode = m_commandLineParser.value(sourceLanguageCodeOption);

    m_translationLanguageCode = m_commandLineParser.value(translationLanguageCodeOption);

    return CommandLineOk;
}

QPHCreateResult QPHCreator::createQPHFile()
{
    printf("SDF File: \"%s\"\n", qPrintable(m_SDFFilePath));
    printf("STF File: \"%s\"\n", qPrintable(m_STFFilePath));
    printf("QPH Dir: \"%s\"\n", qPrintable(m_QPHFileDir));
    printf("QPH File name: \"%s\"\n", qPrintable(m_QPHFileName));

    QDomDocument sdfDoc("SDFDocument");
    if (openDomDocument(sdfDoc, m_SDFFilePath))
        return CreateError;

    QDomDocument stfDoc("STFDocument");
    if (openDomDocument(stfDoc, m_STFFilePath))
        return CreateError;

    QString qphFilePath = m_QPHFileDir + "/" + m_QPHFileName;
    QFile qphFile(qphFilePath);
    if (!qphFile.open(QIODevice::WriteOnly)) {
        writeError(QString("Error: QPHCreator can't open file \"%1\"").arg(qphFilePath));
        return CreateError;
    }

    QDomNodeList sdfTextEnties = sdfDoc.elementsByTagName("TextEntry");
    QDomNodeList stfTextEnties = stfDoc.elementsByTagName("TextEntry");

    for (int i = 0; i < sdfTextEnties.size(); i++) {
        QDomNode textEntry = sdfTextEnties.at(i);
        m_sdfData.insert(textEntry.firstChildElement("Identifier").text(),
                         TextEntry(textEntry.firstChildElement("Text").text(),
                                   textEntry.firstChildElement("Description").text()));
    }

    for (int i = 0; i < stfTextEnties.size(); i++) {
        QDomNode textEntry = stfTextEnties.at(i);
        m_stfData.insert(textEntry.firstChildElement("Identifier").text(),
                         TextEntry(textEntry.firstChildElement("Text").text(),
                                   textEntry.firstChildElement("Description").text()));

    }

    if (sdfTextEnties.length() != stfTextEnties.length())
        printf("WARNING: The number of TextEntry tags in SDF(%d) and STF(%d) documents is not equal.\n",
               sdfTextEnties.length(), stfTextEnties.length());

    QDomDocument qphDoc("QPH");

    QDomElement root = qphDoc.createElement("QPH");

    if (!m_sourceLanguageCode.isEmpty())
        root.setAttribute("sourcelanguage", m_sourceLanguageCode);

    if (!m_translationLanguageCode.isEmpty())
        root.setAttribute("language", m_translationLanguageCode);

    qphDoc.appendChild(root);

    QMap<QString, TextEntry>::const_iterator sdf_i = m_sdfData.constBegin();
    while (sdf_i != m_sdfData.constEnd()) {
        QDomElement phrase = qphDoc.createElement("phrase");
        root.appendChild(phrase);
        QDomElement source = qphDoc.createElement("source");
        source.appendChild(qphDoc.createTextNode(sdf_i.value().m_text));
        QDomElement target = qphDoc.createElement("target");
        QString testText = m_stfData.take(sdf_i.key()).m_text;
        target.appendChild(qphDoc.createTextNode(testText));
        QDomElement definition = qphDoc.createElement("definition");
        definition.appendChild(qphDoc.createTextNode(sdf_i.value().m_description));
        phrase.appendChild(source);
        phrase.appendChild(target);
        phrase.appendChild(definition);
        ++sdf_i;
    }

    QMap<QString, TextEntry>::const_iterator stf_i = m_stfData.constBegin();
    while (stf_i != m_stfData.constEnd()) {
#ifdef debug
        printf("STF: Additional parse.\n");
#endif
        QDomElement phrase = qphDoc.createElement("phrase");
        root.appendChild(phrase);
        QDomElement source = qphDoc.createElement("source");
        source.appendChild(qphDoc.createTextNode(""));
        QDomElement target = qphDoc.createElement("target");
#ifdef debug
        QString stfAdditionalData = stf_i.value().m_text;
        printf("STF: Additional data: %s\n", qPrintable(stfAdditionalData));
#endif
        target.appendChild(qphDoc.createTextNode(stf_i.value().m_text));
        QDomElement definition = qphDoc.createElement("definition");
        definition.appendChild(qphDoc.createTextNode(stf_i.value().m_description));
        phrase.appendChild(source);
        phrase.appendChild(target);
        phrase.appendChild(definition);
        ++stf_i;
    }

    qphFile.write(qphDoc.toByteArray());
    qphFile.close();

    printf("File \"%s\" is created\n", qPrintable(m_QPHFileName));
    return CreateOk;
}

QString QPHCreator::getExtension(QString argument)
{
    QFileInfo fileInfo(argument);
    return fileInfo.suffix();
}

void QPHCreator::writeError(QString errorMessage)
{
    fputs(qPrintable(errorMessage), stderr);
    fputs("\n\n", stderr);
}

int QPHCreator::openDomDocument(QDomDocument &domDocument, QString xmlFilePath)
{
    QString errorMessage;
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        printf("Error: QPHCreator can't open file \"%s\"", qPrintable(xmlFilePath));
        return 1;
    }
    if (!domDocument.setContent(&file, &errorMessage)) {
        printf("Error: \"%s\"", qPrintable(errorMessage));
        file.close();
        return 1;
    }
    file.close();
    return 0;
}
