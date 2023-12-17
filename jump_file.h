#ifndef JUMP_FILE_H
#define JUMP_FILE_H

#include <QObject>
#include <QWidget>
#include <QSet>
#include "config_file.h"

class jump_file
{

public:
    configfile *SettingFile;

public:

    QSet<QString> AllIncludeFiles;

    jump_file(QWidget *parent = nullptr,configfile *SettingFile = nullptr);

    QList<QPair<QString,QPair<QString, int>>> getIncludeFuncList(QString SdkPath);
    QList<QPair<QString, int>> getSourceFuncList(QString SdkPath);
    QList<QPair<QString, int>> getDefineList(QString SdkPath);

    QString getIncludeList(QString SdkPath,QString FileName);

    QPair<QString,QPair<QString, int>> FuncPos(QString SdkPath,QString ClickFile,QString Line);

    void getFileIncludeList(QString SdkPath,QString FileName);

    QSet<QString> getOtherSourceFileIncludeList(QString SdkPath,QString FileName);

    QPair<QString,QPair<QString, int>> DefinePos(QString SdkPath,QString ClickFile,QString Line);

};

#endif // JUMP_FILE_H
