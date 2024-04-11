#ifndef UPDATER_H
#define UPDATER_H

#include <QFileInfo>
#include <QMap>

class Updater
{
public:
    Updater();
    enum FileUpdateResult
    {
        UpdateOk,
        UpdateError
    };

    void addPath(QString argType, QString path);
    QString getPath(QString argType);
    virtual FileUpdateResult updateFile();

private:
    QMap<QString, QString> m_paths;
    QString getExtension(QString arg);
    void writeError(QString errorMessage);
};


#endif // UPDATER_H
