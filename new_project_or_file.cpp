#include "new_project_or_file.h"
#include "new_project_or_file_message.h"
#include "ui_new_project_or_file.h"
#include "mainwindow.h"
#include "config_file.h"
#include "status_bar.h"

#include <QDebug>
#include <QTextEdit>
#include <QDateTime>
#include <QFileDialog>
#include <QString>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QDir>

newprojectorfile::newprojectorfile(QWidget *parent,treemenu *ProjectTree,
                                   configfile *SettingFile,file *FileMenu,statusbar *StatusBar)
    : QWidget(parent), ui(new Ui::newprojectorfile){

    ui->setupUi(this);

    this->ProjectTree = ProjectTree;
    this->SettingFile = SettingFile;
    this->FileMenu = FileMenu;
    this->StatusBar = StatusBar;
    this->MainWindow = parent;

    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);

    ui->newback->setEnabled(false);
    ui->lineEdit_2->setText(QCoreApplication::applicationDirPath());

    this->setWindowIcon(QIcon(":/new/prefix1/res/newProject"));

    connect(ui->NewFile, SIGNAL(toggled(bool)), this, SLOT(onNewFileRadioButtonToggled(bool)));

}

newprojectorfile::~newprojectorfile(){

    delete ui;

}

void newprojectorfile::InitNewProject(){

    if(newProrfi == "Project"){

        if(ProjectName.isEmpty()){

            QMessageBox::warning(this, tr("Application"),tr("Project Name Can Not Empty!"));
            StatusBar->DetermineMessage("Create Project Failed");

        }else if(ProjectLocation.isEmpty()){

            QMessageBox::warning(this, tr("Application"),tr("Project Location Can Not Empty!"));
            StatusBar->DetermineMessage("Create Project Failed");

        }else if(ProjectIndex.isEmpty()){

            QMessageBox::warning(this, tr("Application"),tr("Project Index Can Not Empty!"));
            StatusBar->DetermineMessage("Create Project Failed");

        }else if(ProjectName.indexOf(".sdk")!=-1){

            QMessageBox::warning(this, tr("Application"),tr("Project Name Suffix Can Not Be \"sdk\""));
            StatusBar->DetermineMessage("Create Project Failed");

        }
        else{

            QString dirpath = (ProjectLocation+"/"+ProjectName);
            QDir qDir(dirpath);
            if(qDir.exists()){
                QMessageBox::StandardButton box=QMessageBox::information(this,QString::fromLocal8Bit("Warning"),
                                                                         QString::fromLocal8Bit("Project Exit,Do You Want to Recover This Project?"),
                                                                         QMessageBox::Yes|QMessageBox::No);
                switch(box){
                    case QMessageBox::No:return;
                    case QMessageBox::Yes:{
                         qDir.removeRecursively();
                    }
                }
            }

            QDir ProjectDir;
            ProjectDir.mkdir(ProjectLocation+"/"+ProjectName);

            QDir SrouceDir;
            ProjectDir.mkdir(ProjectLocation+"/"+ProjectName+"/Sources");

            QDir HeaderDir;
            ProjectDir.mkdir(ProjectLocation+"/"+ProjectName+"/Headers");

            QDir ScriptDir;
            ProjectDir.mkdir(ProjectLocation+"/"+ProjectName+"/Scripts");

            QDir OutputDir;
            OutputDir.mkdir(ProjectLocation+"/"+ProjectName+"/"+"Outputs");

            QDir InitDir;
            InitDir.mkdir(ProjectLocation+"/"+ProjectName+"/"+".Configure");

            QString BaseFiles = "BaseFiles"+ui->listWidget->selectedItems().at(0)->text();

            if(!QFile::exists(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Sources/main.c") ||
               !QFile::exists(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Sources/trap.S") ||
               !QFile::exists(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Headers/main.h")||
               !QFile::exists(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Scripts/linksparc.ld")||
               !QFile::exists(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Scripts/Makefile.win") ||
               !QFile::exists(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/.Configure/Config.ini")){

                QMessageBox::warning(this, tr("Application"),tr("SDK Source File Have Encourted A Problem,Please Restore SDK Software!"));
                return;

            }

            if(!QFile::copy(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Sources/main.c",ProjectLocation+"/"+ProjectName+"/Sources/main.c")|
               !QFile::copy(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Sources/trap.S",ProjectLocation+"/"+ProjectName+"/Sources/trap.S")|
               !QFile::copy(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Headers/main.h",ProjectLocation+"/"+ProjectName+"/Headers/main.h")|
               !QFile::copy(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Scripts/linksparc.ld",ProjectLocation+"/"+ProjectName+"/Scripts/linksparc.ld")|
               !QFile::copy(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/Scripts/Makefile.win",ProjectLocation+"/"+ProjectName+"/Scripts/Makefile.win")|
               !QFile::copy(QCoreApplication::applicationDirPath()+"/"+"SourceFile/"+BaseFiles+"/.Configure/Config.ini",ProjectLocation+"/"+ProjectName+"/.Configure/Config.ini")){

                QMessageBox::warning(this, tr("Application"),tr("Error!"));
                return;

            }

            QString filepath = (ProjectLocation+"/"+ProjectName+"/"+ProjectName+".sdk");

            //Updata Global Varibale
            FileMenu->SdkFilePath = filepath;

            //Create SdkFile
            SettingFile->sdkFileInit(filepath);
            QFile file(filepath);
            file.open(QIODevice::ReadWrite|QIODevice::Text);
            file.close();

            //Create File Project Tree
            int index=0;
            for(int i=0;i<filepath.length();i++){
                if(filepath[i]=="/")
                    index=i;
            }
            QString filePath = filepath.mid(0,index).replace("/","/");
            ProjectTree->RemoveTopItem();
            ProjectTree->CreateTopItem(FileMenu->SdkFilePath,filePath,true);

            //Updata SettingFile
            SettingFile->SettingFileInit(filepath);

            //Save Recent Projects
            QString ConfigFile = QCoreApplication::applicationDirPath()+"/config.xml";
            SettingFile->writeXmlFile(ConfigFile,"RecentProjects",filepath);

            //Updata Recent Projects
            FileMenu->UpdataRecent();
            ProjectTree->EditString->clear();
            ProjectTree->Position=-1;
            MainWindow->setWindowTitle("Sparc Develope Kit - "+filepath);
            StatusBar->DetermineMessage("Create Project Success");

        }

    }else{

        if(FiletName.isEmpty()){

            QMessageBox::warning(this, tr("Application"),tr("File Name Can Not Empty!"));
            StatusBar->DetermineMessage("Create File Failed");

        }else if(FileLocation.isEmpty()){

            QMessageBox::warning(this, tr("Application"),tr("File Location Can Not Empty!"));
            StatusBar->DetermineMessage("Create File Failed");

        }else if(FiletName.indexOf(".")==-1){

            QMessageBox::warning(this, tr("Application"),tr("Project Name Suffix Can Not Be Empty"));
            StatusBar->DetermineMessage("Create File Failed");

        }else{

            QString filepath = (FileLocation+"/"+FiletName);
            QFile file(filepath);
            if(file.exists()){
                QMessageBox::StandardButton box=QMessageBox::information(this,QString::fromLocal8Bit("Warning"),
                                                                         QString::fromLocal8Bit("File Exit,Do You Want to Recover This File?"),
                                                                         QMessageBox::Yes|QMessageBox::No);
                switch(box){

                    case QMessageBox::No:return;

                    case QMessageBox::Yes:{
                         QFile::remove(filepath);
                    }

                }
            }
            file.open(QIODevice::ReadWrite|QIODevice::Text);
            file.close();

            if(FileMenu->SdkFilePath != ""){

                //create this file in project
                if(!ProjectTree->CreateNewFile(filepath)){

                     //save recent files
                     QString ConfigFile = QCoreApplication::applicationDirPath()+"/config.xml";
                     SettingFile->writeXmlFile(ConfigFile,"RecentFiles",filepath);

                     //updata recent files
                     FileMenu->UpdataRecent();

                }

            }else {

                //save recent files
                QString ConfigFile = QCoreApplication::applicationDirPath()+"/config.xml";
                SettingFile->writeXmlFile(ConfigFile,"RecentFiles",filepath);

                //updata recent files
                FileMenu->UpdataRecent();

            }

            //show file text window
            ProjectTree->loadFile(filepath);

            StatusBar->DetermineMessage("Create File Success");

        }

    }

}

void newprojectorfile::onNewFileRadioButtonToggled(bool Show){

    if(Show){
        ui->listWidget->setCurrentRow(-1);
        ui->listWidget->setEnabled(false);
        ui->lineEdit->setText("NewFile.c");
        if(FileMenu->SdkFilePath != ""){
            QFileInfo fileinfo(FileMenu->SdkFilePath);
            ui->lineEdit_2->setText(fileinfo.path());
        }
    }else{
        ui->listWidget->setCurrentRow(0);
        ui->listWidget->setEnabled(true);
        ui->lineEdit->setText("NewProject");
        if(FileMenu->SdkFilePath != ""){
            ui->lineEdit_2->setText(QCoreApplication::applicationDirPath());
        }
    }

}

void newprojectorfile::SetParmater(){

    if(ui->NewProject->isChecked()){

        newProrfi = "Project";
        baseInformation = "Create an Project";
        if(ui->listWidget->selectedItems().count()!=0){
            ProjectIndex = ui->listWidget->selectedItems().at(0)->text();
            baseInformation += "\r\nSelect Device    :" + ui->listWidget->selectedItems().at(0)->text();
        }else{
            ProjectIndex = "";
            baseInformation += "\r\nSelect Device    :";
        }

        ProjectName = ui->lineEdit->text();
        baseInformation += "\r\nProject Name     :" + ui->lineEdit->text();
        ProjectLocation = ui->lineEdit_2->text();
        baseInformation += "\r\nProject Location :" + ui->lineEdit_2->text();
        baseInformation += "\r\nUse Version      :Ver2.0";
        baseInformation += "\r\nUse Tools        :Sparc Develope Kit";
        QDateTime current_time = QDateTime::currentDateTime();
        baseInformation += "\r\nCreate Time      :" + current_time.toString("yyyy.MM.dd hh:ss.zzz ddd");

    }
    //if check NewFile
    else if(ui->NewFile->isChecked()){

        newProrfi = "File";
        baseInformation = "Create an File";
        FiletName = ui->lineEdit->text();
        baseInformation += "\r\nFile Name        :" + ui->lineEdit->text();
        if(ui->lineEdit->text().split(".").length()>1){
            baseInformation += "\r\nFile Type        :" + ui->lineEdit->text().split(".")[1] + " File";
        }else{
            baseInformation += "\r\nFile Type        :No Formatting";
        }
        FileLocation = ui->lineEdit_2->text();
        baseInformation += "\r\nFile Location    :" + ui->lineEdit_2->text();
        QDateTime current_time = QDateTime::currentDateTime();
        baseInformation += "\r\nCreate Time      :" + current_time.toString("yyyy.MM.dd hh:ss.zzz ddd");

    }


}

void newprojectorfile::on_newnext_clicked(){

    this->SetParmater();

    message = new newprojectorfilemessage(this,this);
    message->setGeometry(this->frameGeometry().x()+9,this->frameGeometry().y()+30,this->width(),this->height());
    message->show();

    this->hide();

}

void newprojectorfile::on_newfinish_clicked(){

    this->SetParmater();

    this->InitNewProject();

    this->close();

}

void newprojectorfile::on_newcancle_clicked(){

    this->close();

    StatusBar->DetermineMessage("Cancle Create Project/File");

}

void newprojectorfile::on_newbrowse_clicked(){

    QDir dir(ui->lineEdit_2->text());
    QString InitPath;
    if (dir.exists()) {
        InitPath = ui->lineEdit_2->text();
    } else {
        InitPath = QCoreApplication::applicationDirPath();
    }

    QString filePath = QFileDialog::getExistingDirectory(this, "Select Location",InitPath);

    if(filePath!=""){

        ui->lineEdit_2->setText(filePath);

    }

}
