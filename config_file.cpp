#include "config_file.h"
#include <QFileInfo>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomProcessingInstruction>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>
#include <QDateTime>

configfile::configfile(){

}

//if not create config xml file,create it
bool configfile::createXmlFile(const QString &fileName){

    QFileInfo fileInfo(fileName);
    if(fileInfo.isFile()){
        return true;
    }
    QFile XmlFile(fileName);
    if(!XmlFile.open(QIODevice::ReadWrite)){
        return false;
    }
    QDomDocument doc;
    QDomProcessingInstruction inst=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(inst);
    QDomElement root=doc.createElement("ConfigItems");
    doc.appendChild(root);
    QDomElement RecentProjects = doc.createElement("RecentProjects");
    for(int i=0;i<8;i++){
        QDomElement projectsitem = doc.createElement("P"+QString(65+i));
        projectsitem.appendChild(doc.createTextNode(""));
        RecentProjects.appendChild(projectsitem);
    }
    QDomElement RecentFiles = doc.createElement("RecentFiles");
    for(int i=0;i<8;i++){
        QDomElement filesitem = doc.createElement("F"+QString(65+i));
        filesitem.appendChild(doc.createTextNode(""));
        RecentFiles.appendChild(filesitem);
    }
    root.appendChild(RecentProjects);
    root.appendChild(RecentFiles);
    QTextStream out_stream(&XmlFile);
    doc.save(out_stream,2);
    XmlFile.close();

    return true;

}

//clear xml file
bool configfile::clearXmlFile(const QString &fileName,const QString &Item,int index){

    QFile XmlFile(fileName);
    if(!XmlFile.open(QIODevice::ReadOnly)){
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&XmlFile)){
        XmlFile.close();
        return false;
    }
    XmlFile.close();
    QDomElement root = doc.documentElement();
    QDomNodeList rootlist = root.childNodes();

    for(int i=0;i<rootlist.size();i++){
        if(rootlist.at(i).toElement().tagName()==Item){
            QDomNodeList list = rootlist.at(i).toElement().childNodes();
            if(index == -1){
                for(int j=0;j<list.size();j++){
                    list.at(j).toElement().removeChild(list.at(j).toElement().firstChild());
                    list.at(j).toElement().appendChild(doc.createTextNode(""));
                }
            }else{
                for(int j=0;j<list.size()-1;j++){
                    if(j==index){
                        list.at(j).toElement().removeChild(list.at(j).toElement().firstChild());
                        //list.at(j).toElement().appendChild(doc.createTextNode(""));
                    }else if(j>index){
                        list.at(j-1).toElement().appendChild(list.at(j).toElement().firstChild());
                    }
                }
                list.at(list.size()-1).toElement().removeChild(list.at(list.size()-1).toElement().firstChild());
                list.at(list.size()-1).toElement().appendChild(doc.createTextNode(""));
            }
        }
    }
    if(!XmlFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QTextStream out_stream(&XmlFile);
    out_stream.reset();
    out_stream.setCodec("utf-8");
    doc.save(out_stream,4,QDomNode::EncodingFromTextStream);
    XmlFile.close();

    return true;

}

//write Xml File
bool configfile::writeXmlFile(const QString &fileName,const QString &Item,const QString &context){

    if(Item == "RecentProjects"){
        QStringList recentproject = readRecentProject(fileName);
        int index = recentproject.indexOf(context);
        if(index != -1){
            clearXmlFile(fileName,"RecentProjects",index);
        }
    }else{
        QStringList recentfile = readRecentFile(fileName);
        int index = recentfile.indexOf(context);
        if(index != -1){
            clearXmlFile(fileName,"RecentFiles",index);
        }
    }

    QFile XmlFile(fileName);
    if(!XmlFile.open(QIODevice::ReadOnly)){
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&XmlFile)){
        XmlFile.close();
        return false;
    }
    XmlFile.close();
    QDomElement root = doc.documentElement();
    QDomNodeList rootlist = root.childNodes();

    for(int i=0;i<rootlist.size();i++){
        if(rootlist.at(i).toElement().tagName()==Item){
            QDomNodeList list = rootlist.at(i).toElement().childNodes();
            for(int j=list.size()-1;j>0;j--){
                list.at(j).toElement().removeChild(list.at(j).toElement().firstChild());
                list.at(j).toElement().appendChild(list.at(j-1).toElement().firstChild());
            }
            list.at(0).toElement().removeChild(list.at(0).toElement().firstChild());
            list.at(0).toElement().appendChild(doc.createTextNode(context));
        }
    }
    if(!XmlFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QTextStream out_stream(&XmlFile);
    out_stream.reset();
    out_stream.setCodec("utf-8");
    doc.save(out_stream,4,QDomNode::EncodingFromTextStream);
    XmlFile.close();

    return true;
}

//read recent projects
QStringList configfile::readRecentProject(const QString &fileName){

    QStringList stringlist;
    QFile XmlFile(fileName);

    if(!XmlFile.open(QIODevice::ReadOnly)){
        return stringlist;
    }
    QDomDocument doc;
    if(!doc.setContent(&XmlFile)){
        XmlFile.close();
        return stringlist;
    }
    XmlFile.close();
    QDomElement root = doc.documentElement();
    QDomNodeList rootlist = root.childNodes();

    for(int i=0;i<rootlist.size();i++){
        if(rootlist.at(i).toElement().tagName()=="RecentProjects"){
            QDomNodeList list = rootlist.at(i).toElement().childNodes();
            for(int j=0;j<list.size();j++){
                if(list.at(j).toElement().text()!="")
                stringlist<<list.at(j).toElement().text();
            }
        }
    }
    return stringlist;

}

//read recent files
QStringList configfile::readRecentFile(const QString &fileName){

    QStringList stringlist;
    QFile XmlFile(fileName);

    if(!XmlFile.open(QIODevice::ReadOnly)){
        return stringlist;
    }
    QDomDocument doc;
    if(!doc.setContent(&XmlFile)){
        XmlFile.close();
        return stringlist;
    }
    XmlFile.close();
    QDomElement root = doc.documentElement();
    QDomNodeList rootlist = root.childNodes();

    for(int i=0;i<rootlist.size();i++){
        if(rootlist.at(i).toElement().tagName()=="RecentFiles"){
            QDomNodeList list = rootlist.at(i).toElement().childNodes();
            for(int j=0;j<list.size();j++){
                if(list.at(j).toElement().text()!="")
                stringlist<<list.at(j).toElement().text();
            }
        }
    }
    return stringlist;

}

//init sdk file
bool configfile::sdkFileInit(const QString &FileName){

    QFile SdkFile(FileName);
    if(!SdkFile.open(QIODevice::Append|QIODevice::Text)){
        return false;
    }

    SdkFile.write("[ProjectInfo]\n");
    QString ProjectInfo = FileName+"\n\n";
    SdkFile.write(ProjectInfo.toUtf8());
    SdkFile.write("[IncludeDir]\n\n");
    SdkFile.write("[MakeFile]\n\n");
    SdkFile.write("[LinkFile]\n\n");
    SdkFile.write("[SourceFile]\n\n");
    SdkFile.write("[IncludeFile]\n\n");
    SdkFile.write("[FirstFile]\n\n");
    SdkFile.write("[OutputFile]\n\n");
    SdkFile.write("[OtherFile]\n\n");
    SdkFile.write("[LibsDir]\n\n");
    SdkFile.close();

    return true;

}

//write sdk file
bool configfile::writeSdkFile(const QString &FileName,QString Object,QString Context){

    int Insert = 0;

    QFile SdkFile(FileName);
    if(!SdkFile.open(QIODevice::ReadOnly)){
        return false;
    }

    QString FileContex = "";
    QString AllContext = "";
    while(!SdkFile.atEnd()){

        FileContex = SdkFile.readLine();
        AllContext = AllContext + FileContex;
        if(FileContex.contains(Object)){
            break;
        }

    }
    while(!SdkFile.atEnd()){

        FileContex = SdkFile.readLine();
        if(FileContex=="\r\n" && Insert==0){
            FileContex = Context;
            FileContex = FileContex+"\r\n\r\n";
            Insert = 1;
        }
        AllContext = AllContext + FileContex;

    }

    SdkFile.close();

    if(!SdkFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QFile::remove(FileName);
    SdkFile.write(AllContext.toUtf8());
    SdkFile.close();

    return true;

}

//remove sdk file
bool configfile::removeSdkFile(const QString &FileName,QString Context){

    QFile SdkFile(FileName);
    if(!SdkFile.open(QIODevice::ReadOnly)){
        return false;
    }

    QString FileContex = "";
    QString AllContext = "";
    while(!SdkFile.atEnd()){

        FileContex = SdkFile.readLine();
        if(FileContex.contains(Context)){
            continue;
        }
        AllContext = AllContext + FileContex;

    }

    SdkFile.close();

    if(!SdkFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QFile::remove(FileName);
    SdkFile.write(AllContext.toUtf8());
    SdkFile.close();

    return true;

}

//remove sdk file
bool configfile::removeSdk1File(const QString &FileName,QString Object,QString Context){

    QFile SdkFile(FileName);
    if(!SdkFile.open(QIODevice::ReadOnly)){
        return false;
    }

    QString FileContex = "";
    QString AllContext = "";
    bool FindObject = false;
    while(!SdkFile.atEnd()){

        FileContex = SdkFile.readLine();

        if(FindObject){

           if(FileContex.contains(Context)){
                continue;
           }
           else if(FileContex.isEmpty()){
                FindObject = false;
           }

        }

        if(FileContex.contains(Object)){
            FindObject = true;
        }

        AllContext = AllContext + FileContex;

    }

    SdkFile.close();

    if(!SdkFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QFile::remove(FileName);
    SdkFile.write(AllContext.toUtf8());
    SdkFile.close();

    return true;
}

//read sdk file
QStringList configfile::readSdkFile(const QString &FileName,QString Object){

    QStringList AllContext;

    QFile SdkFile(FileName);
    if(!SdkFile.open(QIODevice::ReadOnly)){
        return AllContext;
    }

    QString FileContex = "";
    while(!SdkFile.atEnd()){

        FileContex = SdkFile.readLine();
        if(FileContex.contains(Object)){
            break;
        }

    }
    while(!SdkFile.atEnd()){

        FileContex = SdkFile.readLine();
        if(FileContex=="\r\n"){
            break;
        }
        AllContext.append(FileContex.split("\r\n").at(0));

    }
    SdkFile.close();

    return AllContext;

}

//read all sdk file
QStringList configfile::readSdkAllFile(const QString &FileName){

    QStringList AllContext;

    AllContext.append(readSdkFile(FileName,"MakeFile"));
    AllContext.append(readSdkFile(FileName,"LinkFile"));
    AllContext.append(readSdkFile(FileName,"SourceFile"));
    AllContext.append(readSdkFile(FileName,"IncludeFile"));

    return AllContext;

}

//read all file
QStringList configfile::readAllFile(const QString &FileName){

    QStringList AllContext;

    AllContext.append(readSdkFile(FileName,"ProjectInfo"));
    AllContext.append(readSdkFile(FileName,"IncludeDir"));
    AllContext.append(readSdkFile(FileName,"MakeFile"));
    AllContext.append(readSdkFile(FileName,"LinkFile"));
    AllContext.append(readSdkFile(FileName,"SourceFile"));
    AllContext.append(readSdkFile(FileName,"IncludeFile"));
    //AllContext.append(readSdkFile(FileName,"FirstFile"));
    AllContext.append(readSdkFile(FileName,"OutputFile"));
    return AllContext;

}

bool configfile::SettingFileInit(const QString &SdkFile){

    QFileInfo SdkFileInfo(SdkFile);
    QString IniFile = SdkFileInfo.path()+"/.Configure/Config.ini";
    QSettings setting(IniFile,QSettings::IniFormat);
    setting.beginGroup("Information");

    setting.setValue("ProjectName", SdkFileInfo.fileName());
    setting.setValue("ProjectLocation", SdkFileInfo.path());
    setting.setValue("LastModified", SdkFileInfo.lastModified().toString("yyyy.MM.dd hh:mm:ss"));

    QString FileMessage = "MakeFile:" +QString::number(readSdkFile(SdkFile,"MakeFile").size());
    FileMessage += " LinkFile:" +QString::number(readSdkFile(SdkFile,"LinkFile").size());
    FileMessage += " SourceFile:" +QString::number(readSdkFile(SdkFile,"LinkFile").size());
    FileMessage += " IncludeFile:" +QString::number(readSdkFile(SdkFile,"LinkFile").size());

    setting.setValue("File", FileMessage);

    setting.endGroup();

    return true;
}
