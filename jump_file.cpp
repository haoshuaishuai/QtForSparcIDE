#include <QMap>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QStringList>

#include "jump_file.h"
#include "config_file.h"
jump_file::jump_file(QWidget *Parent,configfile *SettingFile){

    this->SettingFile = SettingFile;
}

void jump_file::getFileIncludeList(QString SdkPath,QString FileName){

    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().startsWith("#include")) {
            QRegExp rx("\"(.*)\"");
            if (rx.indexIn(line) != -1) {
                int beforeSize = AllIncludeFiles.size();
                QStringList IncludeFile = SettingFile->readSdkFile(SdkPath,"IncludeFile");
                QString IncludePath="";
                for(int i=0;i<IncludeFile.size();i++){
                    if(IncludeFile.at(i).contains(rx.cap(1))){
                        IncludePath=IncludeFile.at(i);
                        AllIncludeFiles.insert(IncludePath);
                        break;
                    }
                }
                //check before other have search this file
                if(AllIncludeFiles.size()!=beforeSize){
                    getFileIncludeList(SdkPath,IncludePath);
                }
            }
        }
    }
    file.close();

}

QSet<QString> jump_file::getOtherSourceFileIncludeList(QString SdkPath,QString FileName){

    QSet<QString> Include;
    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return Include;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().startsWith("#include")) {
            QRegExp rx("\"(.*)\"");
            if (rx.indexIn(line) != -1) {
                QStringList IncludeFile = SettingFile->readSdkFile(SdkPath,"IncludeFile");
                for(int i=0;i<IncludeFile.size();i++){
                    if(IncludeFile.at(i).contains(rx.cap(1))){
                        Include.insert(IncludeFile.at(i));
                        break;
                    }
                }
            }
        }
    }
    file.close();
    return Include;

}

QList<QPair<QString, int>> jump_file::getSourceFuncList(QString FilePath){

    QList<QPair<QString, int>> functions;

    QProcess process;
    process.setWorkingDirectory(QCoreApplication::applicationDirPath());
    QStringList arguments;
    arguments << "-R"<< "--c++-kinds=f" << "--fields=+n -f "<<QCoreApplication::applicationDirPath()+"/tags "<< FilePath;
    process.start("ctags.exe", arguments);
    process.waitForFinished();
    process.close();

    //QString filePath = "E:/work2023/QT_20230619/build-untitled2-Desktop_Qt_5_15_2_MinGW_32_bit-Release/tags";
    QString filePath = QCoreApplication::applicationDirPath() + "/tags";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return functions;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList columns = line.split("\t");
        if (columns.size() >= 4 && columns[3] == "f") {
            QString functionName = columns[0];
            int lineNumber = columns[4].split(":")[1].toInt();
            functions.append(qMakePair(functionName, lineNumber));
        }
    }

    file.close();

    return functions;

}

QList<QPair<QString,QPair<QString, int>>> jump_file::getIncludeFuncList(QString FilePath){

    QList<QPair<QString,QPair<QString, int>>> functions;

    QProcess process;
    QStringList arguments;
    process.setWorkingDirectory(QCoreApplication::applicationDirPath());
    arguments << "-R"<< "--c++-kinds=fp" << "--fields=+n -f "<<QCoreApplication::applicationDirPath()+"/tags "<< FilePath;
    process.start("ctags.exe", arguments);
    process.waitForFinished();
    process.close();

    //QString filePath = "E:/work2023/QT_20230619/build-untitled2-Desktop_Qt_5_15_2_MinGW_32_bit-Release/tags";
    QString filePath =QCoreApplication::applicationDirPath() + "/tags";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return functions;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList columns = line.split("\t");
        if (columns.size() >= 4) {
            if(columns[3] == "f"){
                QString functionName = columns[0];
                int lineNumber = columns[4].split(":")[1].toInt();
                functions.append(qMakePair(QString("f"),qMakePair(functionName, lineNumber)));
            }
            if(columns[3] == "p"){
                QString functionName = columns[0];
                int lineNumber = columns[4].split(":")[1].toInt();
                functions.append(qMakePair(QString("p"),qMakePair(functionName, lineNumber)));
            }
        }
    }

    file.close();

    return functions;

}

QList<QPair<QString, int>> jump_file::getDefineList(QString FilePath){

    QList<QPair<QString, int>> defines;

    QProcess process;
    QStringList arguments;
    process.setWorkingDirectory(QCoreApplication::applicationDirPath());
    arguments << "-R"<< "--c++-kinds=d" << "--fields=+n -f "<<QCoreApplication::applicationDirPath()+"/tags "<< FilePath;
    process.start("ctags.exe", arguments);
    process.waitForFinished();
    process.close();

    //QString filePath = "E:/work2023/QT_20230619/build-untitled2-Desktop_Qt_5_15_2_MinGW_32_bit-Release/tags";
    QString filePath = QCoreApplication::applicationDirPath() + "/tags";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return defines;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList columns = line.split("\t");
        if (columns.size() >= 4 && columns[3] == "d") {
            QString functionName = columns[0];
            int lineNumber = columns[4].split(":")[1].toInt();
            defines.append(qMakePair(functionName, lineNumber));
        }
    }

    file.close();

    return defines;

}

QString jump_file::getIncludeList(QString SdkPath,QString FileName){

    QStringList IncludeFile = SettingFile->readSdkFile(SdkPath,"IncludeFile");

    foreach (const QString &str, IncludeFile) {
        QFileInfo includeFile(str);
        if(includeFile.isFile() && includeFile.fileName()==FileName){
            return includeFile.filePath();
        }
    }

    return "";

}

QPair<QString,QPair<QString, int>> jump_file::DefinePos(QString SdkPath,QString ClickFile,QString Line){

    //1.Check Click File Whether Have This Define
    QList<QPair<QString, int>> DefineList = getDefineList(ClickFile);
    for (int i = 0; i < DefineList.size(); ++i) {
        QString DefineName = DefineList[i].first;
        if(DefineName == Line){
            return qMakePair(ClickFile,DefineList[i]);
        }
    }

    //2.Check Include File Whether Have This Define
    AllIncludeFiles.clear();
    this->getFileIncludeList(SdkPath,ClickFile);
    for(QString value : AllIncludeFiles) {
        QList<QPair<QString, int>> IncludeDefineList = getDefineList(value);
        for (int i = 0; i < IncludeDefineList.size(); ++i) {
            QString DefineName = IncludeDefineList[i].first;
            if(DefineName == Line){
                return qMakePair(value,IncludeDefineList[i]);
            }
        }
    }

    QPair<QString,QPair<QString, int>> NullFound;
    return NullFound;

}

QPair<QString,QPair<QString, int>> jump_file::FuncPos(QString SdkPath,QString ClickFile,QString Line){

    //1.Check Click File Whether Have This Func
    QList<QPair<QString, int>> FuncList = getSourceFuncList(ClickFile);
    for (int i = 0; i < FuncList.size(); ++i) {
        QString funcName = FuncList[i].first;
        if(funcName == Line){
            return qMakePair(ClickFile,FuncList[i]);
        }
    }

    //2.Check Other Source File Whether Have This Func
    //2.1Check Include File Whether Have This Func
    AllIncludeFiles.clear();
    this->getFileIncludeList(SdkPath,ClickFile);
    for(QString value : AllIncludeFiles) {
        QList<QPair<QString,QPair<QString, int>>> IncludeFuncList = getIncludeFuncList(value);
        for (int i = 0; i < IncludeFuncList.size(); ++i) {
            QString funcName = IncludeFuncList[i].second.first;
            if(funcName == Line){
                //2.2Check Include File Whether Have This Func
                if(IncludeFuncList[i].first == "f"){
                    return qMakePair(value,IncludeFuncList[i].second);
                }
                else{
                    //2.3Check Source File Which Include This Include File Whether Have This Func
                    QStringList SourceFile = SettingFile->readSdkFile(SdkPath,"SourceFile");
                    for(int j=0;j<SourceFile.size();j++){
                        if(SourceFile.at(j)!=ClickFile){
                            QSet<QString> Include = this->getOtherSourceFileIncludeList(SdkPath,SourceFile.at(j));
                            for(QString str : Include){
                                if(str == value){
                                    QList<QPair<QString, int>> OtherSourceFuncList = getSourceFuncList(SourceFile.at(j));
                                    for (int k = 0; k < OtherSourceFuncList.size(); ++k) {
                                        QString OtherSourceFuncName = OtherSourceFuncList[k].first;
                                        if(OtherSourceFuncName == Line){
                                            return qMakePair(SourceFile.at(j),OtherSourceFuncList[k]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //3.If Self is Include File,Check Source File Which Include This Include File Whether Have This Func
    QStringList SourceFile = SettingFile->readSdkFile(SdkPath,"SourceFile");
    for(int j=0;j<SourceFile.size();j++){
        if(SourceFile.at(j)!=ClickFile){
            QSet<QString> Include = this->getOtherSourceFileIncludeList(SdkPath,SourceFile.at(j));
            for(QString str : Include){
                if(str == ClickFile){
                    QList<QPair<QString, int>> OtherSourceFuncList = getSourceFuncList(SourceFile.at(j));
                    for (int k = 0; k < OtherSourceFuncList.size(); ++k) {
                        QString OtherSourceFuncName = OtherSourceFuncList[k].first;
                        if(OtherSourceFuncName == Line){
                            return qMakePair(SourceFile.at(j),OtherSourceFuncList[k]);
                        }
                    }
                }
            }
        }
    }

    QPair<QString,QPair<QString, int>> NullFound;
    return NullFound;

}
