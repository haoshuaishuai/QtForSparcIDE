#include "file.h"
#include "new_project_or_file.h"
#include "status_bar.h"
#include "qsci_editor.h"
#include <QAction>
#include <QWidget>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QPrintDialog>
#include <QMenu>
#include <QThread>
#include <QSettings>

file::file(QWidget *parent,treemenu *ProjectTree,configfile *SettingFile,statusbar *StatusBar){

    //init var
    this->MainWindow  = parent;
    this->ProjectTree = ProjectTree;
    this->SettingFile = SettingFile;
    this->StatusBar = StatusBar;

}

//connect signals
void file::ConnectSignal(){

    connect(NewProjectOrFile,&QAction::triggered,this,[=]{on_NewProjectOrFile_triggered();});
	
	connect(OpenProjectOrFile,&QAction::triggered,this,[=]{on_OpenProjectOrFile_triggered();});

    connect(CloseProject,&QAction::triggered,this,[=]{on_CloseProject_triggered();});

    connect(CloseAllFiles,&QAction::triggered,this,[=]{on_CloseAllFiles_triggered();});

    connect(Save,&QAction::triggered,this,[=]{on_Save_triggered();});

    connect(SaveAllFiles,&QAction::triggered,this,[=]{on_SaveAllFiles_triggered();});

    connect(ReloadFiles,&QAction::triggered,this,[=]{on_ReloadFiles_triggered();});

    connect(Exit,&QAction::triggered,this,[=]{on_Exit_triggered();});

    connect(Print,&QAction::triggered,this,[=]{on_Print_triggered();});

    connect(CleanRecentFiles,&QAction::triggered,this,[=]{on_CleanRecentFiles_triggered();});

    connect(CleanRecentProjects,&QAction::triggered,this,[=]{on_CleanRecentProjects_triggered();});

};

//updata recent projects/files actions
void file::UpdataRecent(){

    QString fileName = QCoreApplication::applicationDirPath()+"/config.xml";
    RecentFiles->clear();
    RecentFiles->addAction(CleanRecentFiles);
    RecentProjects->clear();
    RecentProjects->addAction(CleanRecentProjects);
    RefreshRecent(fileName);

}

//fresh recent projects and files
void file::RefreshRecent(const QString &fileName){

    //recent projects
    QStringList RecentProject = SettingFile->readRecentProject(fileName);
    QList<QAction *> RecentProjectList ;
    for(int i=0;i<RecentProject.size();i++){
        RecentProjectList.append(new QAction(RecentProject.at(i)));
    }
    RecentProjects->addActions(RecentProjectList);
    for(int i=0;i<RecentProjectList.size();i++){
        connect(RecentProjectList.at(i), &QAction::triggered,this,[=]{
            QString ProjectName = RecentProjectList.at(i)->text();
            QFile SdkFile(ProjectName);
            if(!SdkFile.exists()){
                 QMessageBox::warning(this->MainWindow, tr("Application"),tr("Project File Not Exists!"));
                 StatusBar->DetermineMessage("Open Project Failed");
                 return;
            }
            if (!ProjectName.isEmpty()){

                //Updata Global Varibale
                SdkFilePath = ProjectName;

                int Index=0;
                for(int i=0;i<ProjectName.length();i++){
                    if(ProjectName[i]=="/")
                        Index=i;
                }
                QString ProjectPath = ProjectName.mid(0,Index).replace("/","/");
                ProjectTree->RemoveTopItem();
                ProjectTree->CreateTopItem(SdkFilePath,ProjectPath,false);
                QString ConfigFile = QCoreApplication::applicationDirPath()+"/config.xml";
                SettingFile->writeXmlFile(ConfigFile,"RecentProjects",ProjectName);
                UpdataRecent();
                StatusBar->DetermineMessage("Open Project Success");
                ProjectTree->EditString->clear();
                ProjectTree->Position=-1;
                MainWindow->setWindowTitle("Sparc Develope Kit - "+ProjectName);

            }
        });
    }

    //recent files
    QStringList RecentFile = SettingFile->readRecentFile(fileName);
    QList<QAction *> RecentFileList ;
    for(int i=0;i<RecentFile.size();i++){
        RecentFileList.append(new QAction(RecentFile.at(i),this));
    }
    RecentFiles->addActions(RecentFileList);
    for(int i=0;i<RecentFileList.size();i++){
        connect(RecentFileList.at(i), &QAction::triggered,this,[=]{
            QString FileName = RecentFileList.at(i)->text();
            QFile SourceFile(FileName);
            if(!SourceFile.exists()){
                 QMessageBox::warning(this->MainWindow, tr("Application"),tr("File Not Exists!"));
                 StatusBar->DetermineMessage("Open File Failed");
                 return;
            }
            if (!FileName.isEmpty()){
                ProjectTree->loadFile(FileName);
                QString ConfigFile = QCoreApplication::applicationDirPath()+"/config.xml";
                SettingFile->writeXmlFile(ConfigFile,"RecentFiles",FileName);
                UpdataRecent();
                StatusBar->DetermineMessage("Open File Success");
            }
        });
    }

}

//make a new project or file
void file::on_NewProjectOrFile_triggered(){

    newprojectorfile * newpf = new newprojectorfile(this->MainWindow,ProjectTree,SettingFile,this,StatusBar);
	
    newpf->show();

}

//open a project or file
void file::on_OpenProjectOrFile_triggered(){

    QString fileName = QFileDialog::getOpenFileName(this->MainWindow, "Open Project Or File",QCoreApplication::applicationDirPath(),
                                                  "SDK Project(*.sdk);;SDK File(*.c *.h *.s *.C *.H *.S);;ALL File(*.*)");

    if (!fileName.isEmpty()){

        if(fileName.indexOf(".sdk")!=-1){

            //Updata Global Varibale
            SdkFilePath = fileName;

            //Avoid Project Dir Changed
            UpdataSdkFile(fileName);

            int index=0;
            for(int i=0;i<fileName.length();i++){
                if(fileName[i]=="/")
                    index=i;
            }
            QString filePath = fileName.mid(0,index).replace("/","/");

            ProjectTree->RemoveTopItem();
            ProjectTree->CreateTopItem(fileName,filePath,false);

            //Updata SettingFile
            SettingFile->SettingFileInit(SdkFilePath);

            //Save Recent Files
            QString file = QCoreApplication::applicationDirPath()+"/config.xml";
            SettingFile->writeXmlFile(file,"RecentProjects",fileName);

            UpdataRecent();

            ProjectTree->EditString->clear();
            ProjectTree->Position=-1;

            MainWindow->setWindowTitle("Sparc Develope Kit - "+filePath);

            StatusBar->DetermineMessage("Open Project Success");

        }else{

            //show file text window
            ProjectTree->loadFile(fileName);

            //save file to XML
            QString file = QCoreApplication::applicationDirPath()+"/config.xml";
            SettingFile->writeXmlFile(file,"RecentFiles",fileName);

            //update recent file
            UpdataRecent();

            StatusBar->DetermineMessage("Open File Success");

        }

    }else{

        StatusBar->DetermineMessage("Not Select A File");

    }

}

//open a project or file
void file::ReOpenProjectOrFile(const QString &fileName){

    if (!fileName.isEmpty()){

        if(fileName.indexOf(".sdk")!=-1){

            //Updata Global Varibale
            SdkFilePath = fileName;

            //Avoid Project Dir Changed
            UpdataSdkFile(fileName);

            int index=0;
            for(int i=0;i<fileName.length();i++){
                if(fileName[i]=="/")
                    index=i;
            }
            QString filePath = fileName.mid(0,index).replace("/","/");

            ProjectTree->RemoveTopItem();
            ProjectTree->CreateTopItem(SdkFilePath,filePath,false);

            //Updata SettingFile
            SettingFile->SettingFileInit(SdkFilePath);

            //Save Recent Files
            QString file = QCoreApplication::applicationDirPath()+"/config.xml";
            SettingFile->writeXmlFile(file,"RecentProjects",fileName);

            UpdataRecent();

            ProjectTree->EditString->clear();
            ProjectTree->Position=-1;

            MainWindow->setWindowTitle("Sparc Develope Kit - "+filePath);

            StatusBar->DetermineMessage("Open Project Success");

        }else{

            //show file text window
            ProjectTree->loadFile(fileName);

            //save file to XML
            QString file = QCoreApplication::applicationDirPath()+"/config.xml";
            SettingFile->writeXmlFile(file,"RecentFiles",fileName);

            //update recent file
            UpdataRecent();

            StatusBar->DetermineMessage("Open File Success");

        }

    }else{

        StatusBar->DetermineMessage("Not Select A File");

    }

}

bool file::UpdataSdkFile(const QString &fileName){

    //updata sdk file
    QString SdkFile = fileName;

    //Remove Old Paths
    QString OldCommonFile;
    QStringList OldAllFile =  SettingFile->readAllFile(SdkFile);
    for(int i=0;i<OldAllFile.size();i++){
        if(OldAllFile.at(i).contains(".sdk")){
            QFileInfo OldSdkPath = OldAllFile.at(i);
            OldCommonFile = OldSdkPath.path();
        }
    }
    SettingFile->removeSdkFile(SdkFile,OldCommonFile);

    //Updata New Paths
    QFileInfo NewSdkPath = SdkFile;
    QString NewCommonFile = NewSdkPath.path();
    QStringList NewAllFile;
    for(int i=0;i<OldAllFile.size();i++){
        NewAllFile.append((NewCommonFile+OldAllFile.at(i).split(OldCommonFile).at(1)));
    }

    //Write New Paths to SDK File
    for(int i=0;i<NewAllFile.size();i++){

        QFileInfo EveryFile(NewAllFile.at(i));
        if(EveryFile.isFile()){
            if(NewAllFile.at(i).contains(".sdk")){
                SettingFile->writeSdkFile(SdkFile,"ProjectInfo",NewAllFile.at(i));
            }else if(NewAllFile.at(i).contains("Headers")){
                SettingFile->writeSdkFile(SdkFile,"IncludeFile",NewAllFile.at(i));
            }else if(NewAllFile.at(i).contains("Sources")){
                SettingFile->writeSdkFile(SdkFile,"SourceFile",NewAllFile.at(i));
            }else if(NewAllFile.at(i).contains(".ld")){
                SettingFile->writeSdkFile(SdkFile,"LinkFile",NewAllFile.at(i));
            }else if(NewAllFile.at(i).contains(".win")){
                SettingFile->writeSdkFile(SdkFile,"MakeFile",NewAllFile.at(i));
            }else if(NewAllFile.at(i).contains(".asm" ) || NewAllFile.at(i).contains(".bin") || NewAllFile.at(i).contains(".o")
                     || NewAllFile.at(i).contains(".exe") || NewAllFile.at(i).contains(".a") || NewAllFile.at(i).contains(".readelf")){
                SettingFile->writeSdkFile(SdkFile,"OutputsFile",NewAllFile.at(i));
            }
            if(NewAllFile.at(i).contains(".S")){
                SettingFile->writeSdkFile(SdkFile,"FirstFile",NewAllFile.at(i));
            }

        }else if(EveryFile.isDir()){

            SettingFile->writeSdkFile(SdkFile,"IncludeDir",NewAllFile.at(i));

        }

    }
    return true;

}

//save file
bool file::SaveFile(const QString &fileName,qscieditor * edit){

    ProjectTree->FileWatcher->removePath(fileName);

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this->MainWindow, tr("Application"),tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << edit->text();
    QApplication::restoreOverrideCursor();
    out.flush();
    file.close();

    for(int i=0;i<EditWidget->count();i++){
        if(EditWidget->widget(i)->whatsThis()==edit->whatsThis()){
            if(EditWidget->tabText(i)[0]=="*"){

                EditWidget->setTabText(i,EditWidget->tabText(i).remove(0,1));

            }
        }
    }

    ProjectTree->FileWatcher->addPath(fileName);

    StatusBar->DetermineMessage("Save File Success");

    return true;

}

void file::on_Save_triggered(){
	
    if(EditWidget->currentIndex()!=-1){

        for(int i=0;i<EditList->size();i++){
            if(EditList->at(i)->whatsThis()==EditWidget->currentWidget()->whatsThis()){
                SaveFile(EditList->at(i)->whatsThis(),EditList->at(i));
            }
        }

    }

}

//save all files
void file::on_SaveAllFiles_triggered(){

    if(EditList->size()!=0){
        for(int i=0;i<EditList->size();i++){
            SaveFile(EditList->at(i)->whatsThis(),EditList->at(i));
        }
    }

}

void file::on_CloseProject_triggered(){


    for(int i=0;i<EditWidget->count();i++){

        if(EditWidget->tabText(i)[0]=="*"){

            //QMessageBox::information(this,QString::fromLocal8Bit("Warning"),QString::fromLocal8Bit("file changed has not be saved!"));
            QMessageBox::StandardButton box=QMessageBox::information(this->MainWindow,QString::fromLocal8Bit("Warning"),
                                                                     QString::fromLocal8Bit("Save Change Files?"),
                                                                     QMessageBox::Yes|QMessageBox::No);
            switch(box){
                case QMessageBox::Yes:
                    for(int i=0;i<EditList->size();i++){
                        SaveFile(EditList->at(i)->whatsThis(),EditList->at(i));
                    }

                case QMessageBox::NoButton:
                case QMessageBox::Ok:
                case QMessageBox::Save:
                case QMessageBox::SaveAll:
                case QMessageBox::Open:
                case QMessageBox::YesToAll:
                case QMessageBox::No:
                case QMessageBox::NoToAll:
                case QMessageBox::Abort:
                case QMessageBox::Retry:
                case QMessageBox::Ignore:
                case QMessageBox::Close:
                case QMessageBox::Cancel:
                case QMessageBox::Discard:
                case QMessageBox::Help:
                case QMessageBox::Apply:
                case QMessageBox::Reset:
                case QMessageBox::RestoreDefaults:
                case QMessageBox::Default:
                case QMessageBox::Escape:
                case QMessageBox::FlagMask:
                case QMessageBox::ButtonMask:
                    break;
                }
            break;
        }

    }
    ProjectTree->RemoveTopItem();
    SdkFilePath = "";
    StatusBar->DetermineMessage("Close Project Success");

}

//close all files
void file::on_CloseAllFiles_triggered(){


    for(int i=0;i<EditWidget->count();i++){

        if(EditWidget->tabText(i)[0]=="*"){

            QMessageBox::StandardButton box=QMessageBox::information(this->MainWindow,QString::fromLocal8Bit("Warning"),
                                                                     QString::fromLocal8Bit("Save Change Files?"),
                                                                     QMessageBox::Yes|QMessageBox::No);
            switch(box){
                case QMessageBox::Yes:
                    for(int i=0;i<EditList->size();i++){
                        SaveFile(EditList->at(i)->whatsThis(),EditList->at(i));
                    }

            }
            break;
        }

    }
    EditWidget->clear();
    EditList->clear();
    StatusBar->DetermineMessage("Close All Files Success");

}

//exit window
void file::on_Exit_triggered(){

    this->MainWindow->close();
}

//reload files
void file::on_ReloadFiles_triggered(){

    for(int i=0;i<EditWidget->count();i++){
        QString filepath = EditWidget->widget(i)->whatsThis();
        QFile file(filepath);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(this->MainWindow, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(filepath)
                                 .arg(file.errorString()));
            StatusBar->DetermineMessage("Reload "+filepath+" File Failed");
            //file not exist,close tab
            for(int i=0;i<EditWidget->count();i++){
               if(EditWidget->widget(i)->whatsThis()==filepath){
                   for(int j=0;j<EditList->size();j++){
                       if(EditList->at(j)->whatsThis()==filepath){
                            EditList->removeAt(j);
                       }
                   }
                   EditWidget->removeTab(i);
               }
            }

            return;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        for(int j=0;j<EditList->size();j++){
            if(EditList->at(j)->whatsThis()==EditWidget->widget(i)->whatsThis()){

                 EditList->at(j)->setText(in.readAll());

                 //load break point for file
                 QFileInfo SdkFileInfo(SdkFilePath);
                 QString GlobalConfigFile = SdkFileInfo.path()+"/.Configure/Config.ini";
                 QSettings GlobalSettingFile(GlobalConfigFile,QSettings::IniFormat);
                 int maxsize = GlobalSettingFile.beginReadArray("BreakInfo");
                 for (int h = 0; h < maxsize; h++) {
                     GlobalSettingFile.setArrayIndex(h);
                     if(GlobalSettingFile.value("BreakPointFile")==EditList->at(j)->whatsThis()){

                        int lineAt  = GlobalSettingFile.value("BreakPointLine").toInt()-1;
                        int masknAt=EditList->at(j)->SendScintilla(QsciScintilla::SCI_MARKERGET,lineAt);
                        if(masknAt == 0){
                            EditList->at(j)->SendScintilla(QsciScintilla::SCI_MARKERADD,lineAt,1);
                        }

                    }

                 }
                 GlobalSettingFile.endArray();

            }
        }
        QApplication::restoreOverrideCursor();
    }

    for(int i=0;i<EditWidget->count();i++){
        if(EditWidget->tabText(i)[0]=="*"){
            EditWidget->setTabText(i,EditWidget->tabText(i).remove(0,1));
        }
    }

    StatusBar->DetermineMessage("Reload All Files Success");

}

//print dialog
void file::on_Print_triggered(){

    QPrintDialog dlg(this);
    dlg.setWindowTitle("Print Dialog");
    dlg.show();

}

//clean recent files
void file::on_CleanRecentFiles_triggered(){

    QString fileName = QCoreApplication::applicationDirPath()+"/config.xml";
    SettingFile->clearXmlFile(fileName,"RecentFiles",-1);
    RecentFiles->clear();
    RecentFiles->addAction(CleanRecentFiles);
    StatusBar->DetermineMessage("Clean Recent Files Success");

}

//clean recent projects
void file::on_CleanRecentProjects_triggered(){

    QString fileName = QCoreApplication::applicationDirPath()+"/config.xml";
    SettingFile->clearXmlFile(fileName,"RecentProjects",-1);
    RecentProjects->clear();
    RecentProjects->addAction(CleanRecentProjects);
    StatusBar->DetermineMessage("Clean Recent Projects Success");

}

//clean file
file::~file(){

}
