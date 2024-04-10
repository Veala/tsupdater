#ifndef UPDATER_H
#define UPDATER_H

#include <QFileInfo>
#include <QMap>

#include "commonfeatures.h"

class Updater
{
public:
    Updater();
    enum FileUpdateResult
    {
        UpdateOk,
        UpdateError
    };

    void addPath(QString argType, QString path) {
        m_paths.insert(argType, path);
    };

    QString getPath(QString argType) {
        return m_paths.value(argType);
    }

    virtual FileUpdateResult updateFile();

private:
    QMap<QString, QString> m_paths;

    QString getExtension(QString arg) {
        QFileInfo fileInfo(m_paths.value(arg));
        return fileInfo.suffix();
    }

};


#endif // UPDATER_H
