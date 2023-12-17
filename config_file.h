#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QWidget>
#include <QStringList>

class configfile : public QWidget{

    Q_OBJECT

public:

    configfile();

    bool createXmlFile(const QString &fileName);

    bool writeXmlFile(const QString &fileName,const QString &Item,const QString &context);

    bool clearXmlFile(const QString &fileName,const QString &Item,int index);

    bool sdkFileInit(const QString &FileName);

    bool SettingFileInit(const QString &FileName);

    bool writeSdkFile(const QString &FileName,QString Object,QString Context);

    bool removeSdkFile(const QString &FileName,QString Context);

    bool removeSdk1File(const QString &FileName,QString Object,QString Context);

    QStringList readSdkFile(const QString &fileName,QString Object);

    QStringList readSdkAllFile(const QString &FileName);

    QStringList readAllFile(const QString &FileName);

    QStringList readRecentProject(const QString &fileName);

    QStringList readRecentFile(const QString &fileName);


};

#endif // CONFIGFILE_H
