#include "GPHCreator.h"
#include <QFileInfo>
#include <QDir>
#include <iostream>

GPHCreator::GPHCreator()
{

}

int GPHCreator::checkPositionalArgument(int argNumber, const QStringList& positionalArguments)
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

CommandLineParseResult GPHCreator::parseCommandLine()
{
    m_commandLineParser.setApplicationDescription("GPHCreator creates Linguist \".gph\" files");

    for (int argNumber = 0; argNumber < PosArgNames.size(); argNumber++)
        m_commandLineParser.addPositionalArgument(PosArgNames.at(argNumber), PosArgDescriptions.at(argNumber));

    const QCommandLineOption directoryOption(QStringList() << "d" << "dir", "Directory to create the \".gph\" file (default is \"./\").", "directory", ".");
    m_commandLineParser.addOption(directoryOption);

    const QCommandLineOption nameOption(QStringList() << "o", "Place the \".gph\" glossary output in file <outfile> (default is \"glossary.gph\").", "outfile", "glossary.gph");
    m_commandLineParser.addOption(nameOption);

    const QCommandLineOption languageCodeOption(QStringList() << "l", "Translation language code (\"en_US\", \"fr_FR\", \"ru_RU\" etc., default is \"en_US\").\n" \
                                                                    "NOTE: GPHCreator doesn't check the code.", "language code", "en_US");
    m_commandLineParser.addOption(languageCodeOption);


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
    m_GPHFileDir = m_commandLineParser.value(directoryOption);

    if (m_commandLineParser.isSet(nameOption)) {
        QString gphFileName = m_commandLineParser.value(nameOption);
        if (getExtension(gphFileName) != "gph") {
            writeError(QString("Error: \"%1\"\n").arg(gphFileName) +
                        QString("Valid extensions for output file: \".gph\"."));

            return CommandLineError;
        }
    }

    m_GPHFileName = m_commandLineParser.value(nameOption);

    m_languageCode = m_commandLineParser.value(languageCodeOption);


    return CommandLineOk;
}

GPHCreateResult GPHCreator::createGPHFile()
{
    printf("SDF File \"%s\"\n", qPrintable(m_SDFFilePath));
    printf("STF File \"%s\"\n", qPrintable(m_STFFilePath));
    printf("GPH Dir \"%s\"\n", qPrintable(m_GPHFileDir));
    printf("GPH File name \"%s\"\n", qPrintable(m_GPHFileName));

    QDomDocument sdfDoc("SDFDocument");
    if (openDomDocument(sdfDoc, m_SDFFilePath))
        return CreateError;

    QDomDocument stfDoc("STFDocument");
    if (openDomDocument(stfDoc, m_STFFilePath))
        return CreateError;


    QDomElement docElem = sdfDoc.documentElement(); // <TranslationTool>

    QDomNode textEntry = docElem.firstChild();
    while(!textEntry.isNull()) {
        QDomElement te = textEntry.toElement();
        if(!te.isNull()) {
            if (te.tagName() == "TextEntry") {
                QDomNodeList n = te.childNodes();
            }
        }
        textEntry = textEntry.nextSibling();
    }

    // Here we append a new element to the end of the document
    QDomElement elem = sdfDoc.createElement("img");
    elem.setAttribute("src", "myimage.png");
    docElem.appendChild(elem);

    printf("File \"%s\" is created\n", qPrintable(m_GPHFileName));
    return CreateOk;
}

QString GPHCreator::getExtension(QString argument)
{
    QFileInfo fileInfo(argument);
    return fileInfo.suffix();
}

void GPHCreator::writeError(QString errorMessage)
{
    fputs(qPrintable(errorMessage), stderr);
    fputs("\n\n", stderr);
}

int GPHCreator::openDomDocument(QDomDocument &domDocument, QString xmlFilePath)
{
    QString errorMessage;
    QFile file(m_SDFFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        printf("Error: GPHCreator can't open file \"%s\"", qPrintable(xmlFilePath));
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
