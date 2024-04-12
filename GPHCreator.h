#ifndef GPHCreator_H
#define GPHCreator_H

#include <QFileInfo>
#include <QMap>

class GPHCreator
{
public:
    GPHCreator();

    enum GPHCreateResult
    {
        CreateOk,
        CreateError
    };

    void addPath(QString argType, QString path);
    QString getPath(QString argType);
    virtual GPHCreateResult createGPHFile();

private:
    QMap<QString, QString> m_paths;
    QString getExtension(QString arg);
    void writeError(QString errorMessage);
};


#endif // GPHCreator_H
