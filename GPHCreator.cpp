#include "GPHCreator.h"
#include <QDomDocument>
#include <iostream>

GPHCreator::GPHCreator()
{

}

void GPHCreator::addPath(QString argType, QString path)
{
    m_paths.insert(argType, path);
}

QString GPHCreator::getPath(QString argType)
{
    return m_paths.value(argType);
}

GPHCreator::GPHCreateResult GPHCreator::createGPHFile()
{
    if (getExtension("sdffile") == "sdf" && getExtension("stffile") == "stf" && getExtension("tsfile") == "ts") {
        QDomDocument doc("sdfdocument");
        QFile file(m_paths.value("sdffile"));
        QString errorMessage;
        if (!file.open(QIODevice::ReadOnly)) {
            printf("Error: Updater can't open file \"%s\"", qPrintable(m_paths.value("sdffile")));
            return CreateError;
        }
        if (!doc.setContent(&file, &errorMessage)) {
            printf("Error: \"%s\"", qPrintable(errorMessage));
            file.close();
            return CreateError;
        }
        file.close();

        // print out the element names of all elements that are direct children
        // of the outermost element.
        QDomElement docElem = doc.documentElement();

        QDomNode n = docElem.firstChild();
        while(!n.isNull()) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(!e.isNull()) {
                std::cout << qPrintable(e.tagName()) << std::endl; // the node really is an element.
            }
            n = n.nextSibling();
        }

        // Here we append a new element to the end of the document
        QDomElement elem = doc.createElement("img");
        elem.setAttribute("src", "myimage.png");
        docElem.appendChild(elem);


        writeError("");

        printf("File \"%s\" is updated\n", qPrintable(getPath("tsfile")));
        return GPHCreator::CreateOk;
    } else {
        fputs(qPrintable(QString("Unknown algorithm")), stderr);
        fputs("\n\n", stderr);
        return GPHCreator::CreateError;
    }
}

QString GPHCreator::getExtension(QString arg)
{
    QFileInfo fileInfo(m_paths.value(arg));
    return fileInfo.suffix();
}

void GPHCreator::writeError(QString errorMessage)
{
    fputs(qPrintable(errorMessage), stderr);
    fputs("\n\n", stderr);
};
