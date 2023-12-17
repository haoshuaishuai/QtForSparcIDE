#include "compile.h"
#include "file.h"
#include "config_file.h"
#include <QDateTime>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QHBoxLayout>

//class lockerbutton
LockerButton2::LockerButton2(QWidget* parent)
    : QPushButton(parent)
{
    m_imageLabel = new QLabel;
    m_imageLabel->setFixedWidth(20);
    m_imageLabel->setScaledContents(true);
    m_imageLabel->setStyleSheet("QLabel{background-color:transparent;}");

    m_textLabel = new QLabel;
    m_textLabel->setStyleSheet("QLabel{background-color:transparent;}");

    QPalette palette = this->palette();
    palette.setColor(QPalette::ButtonText, QColor(255, 255, 255, 0));
    this->setPalette(palette);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_imageLabel);
    mainLayout->addWidget(m_textLabel);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
}

void LockerButton2::SetImageLabel(const QPixmap &pixmap){
    m_imageLabel->setPixmap(pixmap);
}

void LockerButton2::SetTextLabel(QString text){
    m_textLabel->setText(text);
    this->setText("START:"+text);
}

QLabel* LockerButton2::GetImageHandle(){
    return m_imageLabel;
}

QLabel* LockerButton2::GetTextHandle(){

    return m_textLabel;
}

//compile class
compile::compile(QObject *parent,file *FileMenu,configfile *SettingFile,treemenu * ProjectTree,statusbar *StatusBar) : QObject(parent){

    this->FileMenu = FileMenu;
    this->SettingFile = SettingFile;
    this->ProjectTree = ProjectTree;
    this->StatusBar = StatusBar;
    this->Timer = new QTimer();

}

//connect signals
void compile::ConnectSignal(){

    connect(CompileProject,&QAction::triggered,this,[=]{on_CompileProject_triggered();});
    connect(ReCompileAllFiles,&QAction::triggered,this,[=]{on_ReCompileAllFiles_triggered();});
    connect(CleanProject,&QAction::triggered,this,[=]{on_CleanProject_triggered();});
    connect(Timer,SIGNAL(timeout()),this,SLOT(TimeOut()));

};

void compile::TimeOut(){

    if(Timer->isActive()){
        Timer->stop();
    }
    StatusBar->ProgressBar->setVisible(false);
    StatusBar->Label->setVisible(false);
    StatusBar->UpdataProgress("Initialize ",0);

}

void compile::on_ReCompileAllFiles_triggered(){

    if(FileMenu->SdkFilePath == ""){
        StatusBar->DetermineMessage("Project Not Open");
        return;
    }

    StatusBar->ProgressBar->setVisible(true);
    StatusBar->Label->setVisible(true);
    StatusBar->UpdataProgress("Initialize ",0);

    QFileInfo SdkInfo(FileMenu->SdkFilePath);
    QString ProjectIniPath = SdkInfo.path()+"/.Configure/Config.ini";
    QSettings ProjectSetting(ProjectIniPath,QSettings::IniFormat);

    ProjectSetting.beginGroup("BuildSetting");
    QString MakeFileTypeMessage = ProjectSetting.value("MakeFileType").toString();
    ProjectSetting.endGroup();

    ProjectSetting.beginGroup("LinkSetting");
    QString LinkFileTypeMessage = ProjectSetting.value("LinkFileType").toString();
    QString TextAddress = ProjectSetting.value("TextAddress").toString();
    ProjectSetting.endGroup();

    if(LinkFileTypeMessage=="auto"){
        CreateLinkfile(TextAddress);
    }

    if(MakeFileTypeMessage=="auto"){
        CreateMakefile();
    }

    CompileError = new QStringList;
    CompileErrorAll = new QStringList;
    CompileTools = new QStringList;
    CompileOutput = new QStringList;
    StatusBar->UpdataProgress("Compiling ",40);

    QStringList MakeFile = SettingFile->readSdkFile(FileMenu->SdkFilePath,"MakeFile");
    CompileFile(MakeFile.at(0),true);

}

void compile::on_CleanProject_triggered(){

    if(FileMenu->SdkFilePath == ""){
        StatusBar->DetermineMessage("Project Not Open");
        return;
    }

    //delate old outputfile
    QTreeWidgetItemIterator iterator(ProjectTree->root);
    while (*iterator) {
        if ((*iterator)->text(0) == "Outputs") {
            nowItem = (*iterator);
            break;
        }
        ++iterator;
    }

    //close window
    for(int i=0;i<nowItem->childCount();i++){

        QString Path = nowItem->child(i)->toolTip(0);
        SettingFile->removeSdkFile(ProjectTree->SdkFile,Path);

        for(int i=0;i<ProjectTree->editTab->count();i++){
            if(ProjectTree->editTab->widget(i)->whatsThis()==Path){
                ProjectTree->EditWidget->CloseTab(i);
                break;
            }
        }

    }

    //deleate tree items
    int count = nowItem->childCount();
    if(count != 0){
        for(int i=0; i<count; i++){
            nowItem->removeChild(nowItem->child(0));
        }
    }

    CompileError = new QStringList;
    ExecMakefile=new QProcess(0);
    ExecMakefile->setWorkingDirectory(QCoreApplication::applicationDirPath());
    QStringList MakeFile = SettingFile->readSdkFile(FileMenu->SdkFilePath,"MakeFile");

    if(!FindButtonLayout){
        delete FindButtonLayout;
    }
    if(!FindButtonWidget){
        delete FindButtonWidget;
    }
    FindButtonLayout = new QVBoxLayout();
    FindButtonLayout->setMargin(0);
    FindButtonLayout->setSpacing(0);
    FindButtonWidget = new QWidget();

    ExecMakefile->setProcessChannelMode(QProcess::MergedChannels);
    ExecMakefile->start("cmd",QStringList()<<"/c"<<"make -f "+MakeFile.at(0)+" clean");
    ExecMakefile->waitForStarted();
    connect(ExecMakefile,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(ExecClean()));

}

void compile::ExecClean(){

    CompileError->append("Clean All OutputFiles Success!");
    SetUpUI();
    StatusBar->DetermineMessage("Clean Success");

}

void compile::on_CompileProject_triggered(){

    if(FileMenu->SdkFilePath == ""){
        StatusBar->DetermineMessage("Project Not Open");
        return;
    }

    MessageWidget->setCurrentIndex(0);

    StatusBar->ProgressBar->setVisible(true);
    StatusBar->Label->setVisible(true);
    StatusBar->UpdataProgress("Initialize ",0);

    QFileInfo SdkInfo(FileMenu->SdkFilePath);
    QString ProjectIniPath = SdkInfo.path()+"/.Configure/Config.ini";
    QSettings ProjectSetting(ProjectIniPath,QSettings::IniFormat);

    ProjectSetting.beginGroup("BuildSetting");
    QString MakeFileTypeMessage = ProjectSetting.value("MakeFileType").toString();
    ProjectSetting.endGroup();

    ProjectSetting.beginGroup("LinkSetting");
    QString LinkFileTypeMessage = ProjectSetting.value("LinkFileType").toString();
    QString TextAddress = ProjectSetting.value("TextAddress").toString();
    ProjectSetting.endGroup();

    if(LinkFileTypeMessage=="auto"){
        CreateLinkfile(TextAddress);
    }

    if(MakeFileTypeMessage=="auto"){
        CreateMakefile();
    }

    CompileError = new QStringList;
    CompileErrorAll = new QStringList;
    CompileTools = new QStringList;
    CompileOutput = new QStringList;
    StatusBar->UpdataProgress("Compiling ",40);

    QStringList MakeFile = SettingFile->readSdkFile(FileMenu->SdkFilePath,"MakeFile");
    CompileFile(MakeFile.at(0),false);

}

bool compile::CompileFile(QString FilePath,bool ReCompileAllFiles){

    //delate old outputfile
    QTreeWidgetItemIterator iterator(ProjectTree->root);
    while (*iterator) {
        if ((*iterator)->text(0) == "Outputs") {
            nowItem = (*iterator);
            break;
        }
        ++iterator;
    }

    //close window & remove files in sdkfile
    for(int i=0;i<nowItem->childCount();i++){

        QString Path = nowItem->child(i)->toolTip(0);
        SettingFile->removeSdkFile(ProjectTree->SdkFile,Path);

        for(int i=0;i<ProjectTree->editTab->count();i++){
            if(ProjectTree->editTab->widget(i)->whatsThis()==Path){
                ProjectTree->EditWidget->CloseTab(i);
                break;
            }
        }

    }

    //deleate tree items
    int count = nowItem->childCount();
    if(count != 0){
        for(int i=0; i<count; i++){
            nowItem->removeChild(nowItem->child(0));
        }
    }

    //Do Make
    ExecMakefile=new QProcess(0);
    ExecMakefile->setWorkingDirectory(QCoreApplication::applicationDirPath());
    ExecMakefile->setProcessChannelMode(QProcess::MergedChannels);

    if(ReCompileAllFiles){
        ExecMakefile->start("cmd",QStringList()<<"/c"<<"make -f "+FilePath+" clean");
        ExecMakefile->waitForFinished();
    }
    ExecMakefile->start("cmd",QStringList()<<"/c"<<"make -f "+FilePath+" all");

    FindButtonLayout = new QVBoxLayout();
    FindButtonLayout->setMargin(0);
    FindButtonLayout->setSpacing(0);
    FindButtonWidget = new QWidget();
    ExecMakefile->waitForStarted();

    QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
    QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);
    GlobalSetting.beginGroup("BuildTools");
    QString GccPath =GlobalSetting.value("GccPath").toString();
    if(GccPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        GccPath = GccPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString GPlusPlusPath =GlobalSetting.value("GPlusPlusPath").toString();
    if(GPlusPlusPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        GPlusPlusPath = GPlusPlusPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString GdbPath =GlobalSetting.value("GdbPath").toString();
    if(GdbPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        GdbPath = GdbPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString LinkPath = GlobalSetting.value("LinkPath").toString();
    if(LinkPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        LinkPath = LinkPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString ObjCopyPath = GlobalSetting.value("ObjCopyPath").toString();
    if(ObjCopyPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ObjCopyPath = ObjCopyPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString ObjDumpPath = GlobalSetting.value("ObjDumpPath").toString();
    if(ObjDumpPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ObjDumpPath = ObjDumpPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString ArPath = GlobalSetting.value("ArPath").toString();
    if(ArPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ArPath = ArPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString ReadElfPath = GlobalSetting.value("ReadElfPath").toString();
    if(ReadElfPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ReadElfPath = ReadElfPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    QString SizePath = GlobalSetting.value("SizePath").toString();
    if(SizePath.contains("CurrentDir:", Qt::CaseInsensitive)){
        SizePath = SizePath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    CompileTools->append(GccPath);
    CompileTools->append(GPlusPlusPath);
    CompileTools->append(LinkPath);
    CompileTools->append(ObjCopyPath);
    CompileTools->append(ObjDumpPath);
    CompileTools->append(ArPath);
    CompileTools->append(ReadElfPath);
    CompileTools->append(SizePath);
    CompileTools->append("CK = \r\n\r\n");
    LdFile = GlobalSetting.value("LinkPath").toString()+":";
    GlobalSetting.endGroup();

    connect(ExecMakefile,SIGNAL(readyReadStandardOutput()),this,SLOT(Exec()));
    connect(ExecMakefile,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(Finished()));

    return true;

}

void compile::Exec(){

    QString StrTemp=QString::fromLocal8Bit(ExecMakefile->readAllStandardOutput());
    CompileOutput->append(StrTemp);

}

void compile::Finished(){

    unsigned int number = 0;
    for(int k=0;k<CompileOutput->size();k++){

        QStringList Str =  CompileOutput->at(k).split("\r\n");
        bool IsError = true;
        QString ErrorMessage = "";
        for(int i=0;i<Str.size();i++){

            for(int j=0;j<CompileTools->size();j++){

                if(Str.at(i).contains(CompileTools->at(j))||Str.at(i).isEmpty()){
                    IsError = false;
                }
                if(Str.at(i).contains(LdFile)){
                    IsError = true;
                }

            }

            if(IsError){

                number++;
                if(Str.at(i).indexOf("text\t   data\t    bss\t    dec\t    hex\t")!=-1){

                    ErrorMessage = "["+QString::number(number)+"] "+"Memory Used(Bytes):";
                    CompileError->append(ErrorMessage);
                    ErrorMessage = "["+QString::number(number+1)+"] "+Str.at(i);
                    CompileError->append(ErrorMessage);
                    i++;
                    if(i<Str.size()){
                        ErrorMessage ="["+QString::number(number+2)+"] "+ Str.at(i);
                        CompileError->append(ErrorMessage);

                    }
                    continue;

                }else{

                    ErrorMessage = "["+QString::number(number)+"] "+Str.at(i);
                }

                CompileError->append(ErrorMessage);
                CompileErrorAll->append(ErrorMessage);
                IsError = true;

            }

            QRegExp DetectFinish;
            DetectFinish.setPattern("make: \\*\\*\\*");
            if(DetectFinish.indexIn(Str.at(i))!=-1){
                StatusBar->UpdataProgress("Failed ",100);
            }

        }

    }

    if(StatusBar->Label->text()!="Failed "){

        CompileError->append("Project Build Success!");
        StatusBar->DetermineMessage("Compile Success");
        StatusBar->UpdataProgress("Success ",100);

    }else{

        MessageWidget->setCurrentIndex(0);
        Output->raise();
        CompileError->append("Project Build Failed!");
        StatusBar->DetermineMessage("Compile Failed");
        StatusBar->UpdataProgress("Failed ",100);

    }

    SetUpUI();

    if(Timer->isActive()){
        Timer->stop();
    }
    Timer->start(6000);

    //add outputfile to filetree list
    QDir SdkFilePath =  QFileInfo(ProjectTree->SdkFile).path() + "/Outputs";
    QStringList AllDirName = SdkFilePath.entryList();
    QString PathName = SdkFilePath.absolutePath();

    for(int i=2;i<AllDirName.size();i++){

        QFileInfo *FileInfo = new QFileInfo(PathName+"/"+AllDirName[i]);
        QString FilePath = FileInfo->path() + "/"+FileInfo->fileName();

        if(FileInfo->isFile()){

            if(FileInfo->fileName().contains(".asm" ) || FileInfo->fileName().contains(".bin") || FileInfo->fileName().contains(".o")
               || FileInfo->fileName().contains(".exe") || FileInfo->fileName().contains(".a") || FileInfo->fileName().contains(".readelf")){

                SettingFile->writeSdkFile(ProjectTree->SdkFile,"OutputFile",FilePath);
                //make tree child
                QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
                child->setText(0,FileInfo->fileName());
                child->setIcon(0,QIcon(ProjectTree->IconPath+"text.jpg"));
                child->setToolTip(0,FilePath);

            }

        }

    }

}

void compile::SetUpUI(){

    //read all project files
    FileList = SettingFile->readSdkAllFile(this->FileMenu->SdkFilePath);
    //set use layout
    for(int j=0;j<CompileError->size();j++){

        //set use button
        FindStringButton = new LockerButton2;
        FindStringButton->setObjectName("LockerButton");
        QString Text = CompileError->at(j);
        FindStringButton->SetTextLabel(Text);
        FindStringButton->setStyleSheet("#LockerButton{background-color:transparent}"
        "#LockerButton:hover{background-color:rgba(195,195,195,0.4)}"
        "#LockerButton:pressed{background-color:rgba(127,127,127,0.4)}");

        QLabel* FindLabel = FindStringButton->GetTextHandle();
        //FindLabel->setStyleSheet("QLabel{color:rgba(183,71,42,1)}");
        FindLabel->setFont(QFont("Microsoft YaHei", 9, QFont::Normal));
        FindStringButton->setFont(QFont("Microsoft YaHei", 9, QFont::Normal));
        FindButtonLayout->addWidget(FindStringButton);

        connect(FindStringButton, &LockerButton2::clicked, [=](bool) {

           for(int i = 0;i<FileList.size();i++ ){

                QFileInfo FileInfo(FileList.at(i));
                if(Text.contains(FileInfo.fileName())){
                    ProjectTree->loadFile(FileInfo.filePath());
                    for(int t=0;t<EditList->size();t++){
                        if(EditList->at(t)->whatsThis() == FileInfo.filePath()){
                            QRegExp DetectLine;
                            DetectLine.setPattern("\\d{1,}:\\d{1,}");
                              if(Text.indexOf(DetectLine)!=-1){
                                 QStringList LineCursion = DetectLine.capturedTexts().at(0).split(":");
                                 int Line = LineCursion.at(0).toInt();
                                 int Cursion = LineCursion.at(1).toInt();
                                 EditList->at(t)->setCursorPosition(Line-1,Cursion);
                                 EditList->at(t)->setSelection(Line-1,0,Line-1,EditList->at(t)->text(Line-1).size()-1);
                              }
                        }
                    }
                }

            }

        });

    }

    //set all
    for(int i = 0;i<FindButtonLayout->count();i++){
        QLayoutItem *LayoutItem = FindButtonLayout->itemAt(i);
        if(LayoutItem->spacerItem()){
            FindButtonLayout->removeItem(LayoutItem);
        }
    }

    FindButtonLayout->addStretch();
    FindButtonWidget->setLayout(FindButtonLayout);

    this->ScrollArea->setFrameStyle(QFrame::NoFrame);
    this->ScrollArea->setWidget(FindButtonWidget);

}

bool compile::CreateLinkfile(QString TextAddress){

     if(!this->FileMenu->SdkFilePath.isEmpty()){

         QString SdkFilePath = this->FileMenu->SdkFilePath;
         QString LinkFilePath = SettingFile->readSdkFile(SdkFilePath,"LinkFile").at(0);
         QFile LinkFile(LinkFilePath);
         if(!LinkFile.open(QIODevice::WriteOnly)){
             return false;
         }
         QFile::remove(LinkFilePath);

         QFileInfo SdkInfo(FileMenu->SdkFilePath);
         QString CommentName = "/* Project Name:"+SdkInfo.fileName()+" */\r\n";
         QString ProjectName = SdkInfo.fileName().split(".").at(0);
         QDateTime CurrentTime =QDateTime::currentDateTime();
         QString CommentTime = "/* Create Time:"+CurrentTime.toString("yyyy.MM.dd hh:mm:ss") +" */\r\n\r\n";
         LinkFile.write(CommentName.toUtf8());
         LinkFile.write(CommentTime.toUtf8());

         QString LinkComment = "OUTPUT_ARCH(sparc)\r\n";
                 LinkComment +="__DYNAMIC  =  0;\r\n\r\n";
                 LinkComment +="MEMORY\r\n";
                 LinkComment +="{\r\n";
                 LinkComment +="  rom     : ORIGIN = 0x00000000, LENGTH = 1024M\r\n";
                 LinkComment +="  ram     : ORIGIN = "+TextAddress+", LENGTH = 1024M\r\n";
                 LinkComment +="}\r\n\r\n";
                 LinkComment +="SECTIONS\r\n";
                 LinkComment +="{\r\n";
                 LinkComment +="  .text :\r\n";
                 LinkComment +="  {\r\n";
                 LinkComment +="    CREATE_OBJECT_SYMBOLS\r\n";
                 LinkComment +="    text_start = .;\r\n";
                 LinkComment +="    _text_start = .;\r\n";
                 LinkComment +="    *(.text)\r\n";
                 LinkComment +="    . = ALIGN (16);\r\n\r\n";
                 LinkComment +="    *(.eh_frame)\r\n";
                 LinkComment +="    . = ALIGN (16);\r\n";
                 LinkComment +="    __CTOR_LIST__ = .;\r\n";
                 LinkComment +="    LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)\r\n";
                 LinkComment +="    *(.ctors)\r\n";
                 LinkComment +="    LONG(0)\r\n";
                 LinkComment +="    __CTOR_END__ = .;\r\n";
                 LinkComment +="    __DTOR_LIST__ = .;\r\n";
                 LinkComment +="    LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)\r\n";
                 LinkComment +="    *(.dtors)\r\n";
                 LinkComment +="    LONG(0)\r\n";
                 LinkComment +="    __DTOR_END__ = .;\r\n";
                 LinkComment +="    _rodata_start = . ;\r\n";
                 LinkComment +="    *(.rodata*)\r\n";
                 LinkComment +="    _erodata = ALIGN( 0x10 ) ;\r\n";
                 LinkComment +="    etext = ALIGN(0x10);\r\n";
                 LinkComment +="    _etext = .;\r\n";
                 LinkComment +="    *(.init)\r\n";
                 LinkComment +="    *(.fini)\r\n";
                 LinkComment +="    *(.lit)\r\n";
                 LinkComment +="    *(.shdata)\r\n";
                 LinkComment +="    . = ALIGN (16);\r\n";
                 LinkComment +="    _endtext = .;\r\n";
                 LinkComment +="  } >ram\r\n\r\n";
                 LinkComment +="  .data :\r\n";
                 LinkComment +="  {\r\n";
                 LinkComment +="    data_start = .;\r\n";
                 LinkComment +="    _data_start = .;\r\n";
                 LinkComment +="    _sdata = . ;\r\n";
                 LinkComment +="    *(.data)\r\n";
                 LinkComment +="    . = ALIGN(0x10);\r\n";
                 LinkComment +="    edata = .;\r\n";
                 LinkComment +="    _edata = .;\r\n";
                 LinkComment +="  } > ram\r\n\r\n";
                 LinkComment +="  .shbss :\r\n";
                 LinkComment +="  {\r\n";
                 LinkComment +="    *(.shbss)\r\n";
                 LinkComment +="  }\r\n\r\n";
                 LinkComment +="  .bss :\r\n";
                 LinkComment +="  {\r\n";
                 LinkComment +="    __bss_start = ALIGN(0x8);\r\n";
                 LinkComment +="    _bss_start = .;\r\n";
                 LinkComment +="    bss_start = .;\r\n";
                 LinkComment +="    *(.bss)\r\n";
                 LinkComment +="    *(COMMON)\r\n";
                 LinkComment +="    end = .;\r\n";
                 LinkComment +="    _end = ALIGN(0x8);\r\n";
                 LinkComment +="    __end = ALIGN(0x8);\r\n";
                 LinkComment +="  }  > ram\r\n\r\n";
                 LinkComment +="   .jcr . (NOLOAD)  : { *(.jcr)}\r\n\r\n";
                 LinkComment +="  .stab . (NOLOAD) :\r\n";
                 LinkComment +="  {\r\n";
                 LinkComment +="    [ .stab ]\r\n";
                 LinkComment +="  }\r\n";
                 LinkComment +="  .stabstr . (NOLOAD) :\r\n";
                 LinkComment +="  {\r\n";
                 LinkComment +="    [ .stabstr ]\r\n";
                 LinkComment +="  }\r\n";
                 LinkComment +="}\r\n";

         LinkFile.write(LinkComment.toUtf8());
         LinkFile.close();

     }

     return true;
}

bool compile::CreateMakefile(){

    if(!this->FileMenu->SdkFilePath.isEmpty()){

        QString SdkFilePath = this->FileMenu->SdkFilePath;
        QString MakeFilePath = SettingFile->readSdkFile(SdkFilePath,"MakeFile").at(0);
        QFile MakeFile(MakeFilePath);
        if(!MakeFile.open(QIODevice::WriteOnly)){
            return false;
        }
        QFile::remove(MakeFilePath);

        //MakeFile Comment
        QFileInfo SdkInfo(this->FileMenu->SdkFilePath);
        QString CommentName = "# Project Name:"+SdkInfo.fileName()+"\r\n";
        QString ProjectName = SdkInfo.fileName().split(".").at(0);
        QDateTime CurrentTime =QDateTime::currentDateTime();
        QString CommentTime = "# Create Time:"+CurrentTime.toString("yyyy.MM.dd hh:mm:ss") +"\r\n\r\n";
        MakeFile.write(CommentName.toUtf8());
        MakeFile.write(CommentTime.toUtf8());

        //Global Var
        QString OutputPath = "OutPath = " + SdkInfo.path() + "/Outputs/\r\n\r\n";
        MakeFile.write(OutputPath.toUtf8());

        //MakeFile Tools
        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);
        GlobalSetting.beginGroup("BuildTools");

        QString GccPath =GlobalSetting.value("GccPath").toString();
        if(GccPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            GccPath = "CC = "+GccPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            GccPath = "CC = "+GccPath+"\r\n";
        }
        QString GPlusPlusPath =GlobalSetting.value("GPlusPlusPath").toString();
        if(GPlusPlusPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            GPlusPlusPath = "CPP = "+GPlusPlusPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            GPlusPlusPath = "CPP = "+GPlusPlusPath+"\r\n";
        }
        QString LinkPath = GlobalSetting.value("LinkPath").toString();
        if(LinkPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            LinkPath = "LD = "+LinkPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            LinkPath = "LD = "+LinkPath+"\r\n";
        }
        QString ObjCopyPath = GlobalSetting.value("ObjCopyPath").toString();
        if(ObjCopyPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            ObjCopyPath = "OBJCOPY = "+ObjCopyPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            ObjCopyPath = "OBJCOPY = "+ObjCopyPath+"\r\n";
        }
        QString ObjDumpPath = GlobalSetting.value("ObjDumpPath").toString();
        if(ObjDumpPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            ObjDumpPath = "OBJDUMP = "+ObjDumpPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            ObjDumpPath = "OBJDUMP = "+ObjDumpPath+"\r\n";
        }
        QString ArPath = GlobalSetting.value("ArPath").toString();
        if(ArPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            ArPath = "AR = "+ArPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            ArPath = "AR = "+ArPath+"\r\n";
        }
        QString ReadElfPath = GlobalSetting.value("ReadElfPath").toString();
        if(ReadElfPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            ReadElfPath = "READELF = "+ReadElfPath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            ReadElfPath = "READELF = "+ReadElfPath+"\r\n";
        }
        QString SizePath = GlobalSetting.value("SizePath").toString();
        if(SizePath.contains("CurrentDir:", Qt::CaseInsensitive)){
            SizePath = "SIZE = "+SizePath.replace("CurrentDir:",QCoreApplication::applicationDirPath())+"\r\n";
        }else{
            SizePath = "SIZE = "+SizePath+"\r\n";
        }
        QString CkPath = "CK = \r\n\r\n";
        GlobalSetting.endGroup();
        MakeFile.write(GccPath.toUtf8());
        MakeFile.write(GPlusPlusPath.toUtf8());
        MakeFile.write(LinkPath.toUtf8());
        MakeFile.write(ObjCopyPath.toUtf8());
        MakeFile.write(ObjDumpPath.toUtf8());
        MakeFile.write(ArPath.toUtf8());
        MakeFile.write(ReadElfPath.toUtf8());
        MakeFile.write(SizePath.toUtf8());
        MakeFile.write(CkPath.toUtf8());

        //MakeFile Lib & Output
        QString ProjectIniPath = SdkInfo.path()+"/.Configure/Config.ini";
        QSettings ProjectSetting(ProjectIniPath,QSettings::IniFormat);

        QString WarningMessage="";
        QString V8ExtensionMessage="";
        QString NotEnableFpuMessage = "";
        QString SoftFloatMessage = "";
        QString FlatRegisterMessage = "";
        QString OptimizeSelectMessage = "";
        QString OtherOptmizationMessage = "";
        QString OtherBuildParmatersMessage = "";

        ProjectSetting.beginGroup("BuildSetting");
        WarningMessage = ProjectSetting.value("WarningMessage").toString();
        V8ExtensionMessage = ProjectSetting.value("V8Extension").toString();
        NotEnableFpuMessage = ProjectSetting.value("NotEnableFpu").toString();
        SoftFloatMessage = ProjectSetting.value("SoftFloat").toString();
        FlatRegisterMessage = ProjectSetting.value("FlatRegister").toString();
        OptimizeSelectMessage = ProjectSetting.value("OptimizeSelect").toString();
        OtherOptmizationMessage = ProjectSetting.value("OtherOptmization").toString();
        OtherBuildParmatersMessage = ProjectSetting.value("OtherBuildParmaters").toString();
        ProjectSetting.endGroup();

        QString DontUseDefaultLibMessage = "";
        QString NoSharedLibMessage = "";
        QString DontPageAlignDataMessage = "";
        QString OmitAllSymboMessage = "";
        QString TextAddress = "";
        QString DataAddress = "";
        QString BssAddress = "";
        QString StackAddress = "";
        QString UseCkExeMessage = "";
        QString ReadElfMessage = "";
        QString OtherLinkParmatersMessage = "";

        ProjectSetting.beginGroup("LinkSetting");
        TextAddress = ProjectSetting.value("TextAddress").toString();
        DataAddress = ProjectSetting.value("DataAddress").toString();
        BssAddress = ProjectSetting.value("BssAddress").toString();
        StackAddress = ProjectSetting.value("StackAddress").toString();
        DontUseDefaultLibMessage = ProjectSetting.value("DontUseDefaultLib").toString();
        NoSharedLibMessage = ProjectSetting.value("NoSharedLib").toString();
        DontPageAlignDataMessage = ProjectSetting.value("DontPageAlignData").toString();
        OmitAllSymboMessage = ProjectSetting.value("OmitAllSymbo").toString();
        UseCkExeMessage = ProjectSetting.value("UseCkExe").toString();
        ReadElfMessage = ProjectSetting.value("ReadElf").toString();
        OtherLinkParmatersMessage = ProjectSetting.value("OtherLinkParmaters").toString();
        ProjectSetting.endGroup();

        QStringList SourceFile = SettingFile->readSdkFile(FileMenu->SdkFilePath,"SourceFile");
        QStringList LinkFile = SettingFile->readSdkFile(FileMenu->SdkFilePath,"LinkFile");
        QStringList IncludeDir = SettingFile->readSdkFile(FileMenu->SdkFilePath,"IncludeDir");
        QStringList FirstFile = SettingFile->readSdkFile(FileMenu->SdkFilePath,"FirstFile");

        QString OBJ = "OBJ = ";
        QString LINKOBJ = "LINKOBJ = ";
        QFileInfo FirstFileInfo(FirstFile.at(0));
        OBJ += "$(OutPath)"+FirstFileInfo.fileName().split(".").at(0)+".o ";
        LINKOBJ += "$(OutPath)"+FirstFileInfo.fileName().split(".").at(0)+".o ";
        for(int i = 0;i<SourceFile.size();i++){
            QFileInfo SourceInfo(SourceFile.at(i));
            if(SourceFile.at(i)!=FirstFile.at(0)){
                OBJ += "$(OutPath)"+SourceInfo.fileName().split(".").at(0)+".o ";
                LINKOBJ += "$(OutPath)"+SourceInfo.fileName().split(".").at(0)+".o ";
            }
        }
        OBJ += "\r\n\r\n";
        LINKOBJ += "-Ttext=" + TextAddress + " ";
        if(!DataAddress.isEmpty())
        LINKOBJ += "-Tdata=" + DataAddress + " ";
        if(!BssAddress.isEmpty())
        LINKOBJ += "-Tbss=" + BssAddress + " ";
        LINKOBJ += DontUseDefaultLibMessage + NoSharedLibMessage + DontPageAlignDataMessage + OmitAllSymboMessage + OtherLinkParmatersMessage;
        LINKOBJ += " "+LinkFile.at(0);
        LINKOBJ += "\r\n";

        QString LIBS = "LIBS = -L\""+QCoreApplication::applicationDirPath()+"/bcc-2.0.8-gcc/sparc-gaisler-elf/lib\" -L\""+QCoreApplication::applicationDirPath()+"/bcc-2.0.8-gcc/lib/gcc/sparc-gaisler-elf/7.2.0\" -lm  -lc  -lgcc";
        LIBS += "\r\n";

        QString INCS = "INCS = ";
        INCS += "-I\""+QCoreApplication::applicationDirPath()+"/bcc-2.0.8-gcc/include\" ";
        for(int i = 0;i<IncludeDir.size();i++){
            if(IncludeDir.at(i).indexOf("Headers")!=-1){
                INCS += "-I\""+IncludeDir.at(i)+"\" ";
            }
        }
        ProjectSetting.beginGroup("LinkLibary");
        QString LinkIncludeMessage =  "";
        LinkIncludeMessage = ProjectSetting.value("LinkLibary").toString();
        ProjectSetting.endGroup();
        QStringList LinkIncludes = LinkIncludeMessage.split(";");
        for(int i = 0;i<LinkIncludes.size()-1;i++){
            INCS += "-I\""+LinkIncludes.at(i)+"\" ";
        }
        INCS += "\r\n";

        QString BIN = "BIN = $(OutPath)"+ProjectName+".exe";
        BIN += "\r\n";

        QString CFLAGS = "CFLAGS = $(INCS) ";
        CFLAGS += WarningMessage+V8ExtensionMessage+NotEnableFpuMessage+SoftFloatMessage+FlatRegisterMessage ;
        CFLAGS += OptimizeSelectMessage + OtherOptmizationMessage + OtherBuildParmatersMessage;
        CFLAGS += "-g";
        CFLAGS += "\r\n\r\n";

        QString PHONY = ".PHONY: all clean\r\n\r\n";

        MakeFile.write(OBJ.toUtf8());
        MakeFile.write(LINKOBJ.toUtf8());
        MakeFile.write(LIBS.toUtf8());
        MakeFile.write(INCS.toUtf8());
        MakeFile.write(BIN.toUtf8());
        MakeFile.write(CFLAGS.toUtf8());
        MakeFile.write(PHONY.toUtf8());

        //MakeFile all & clean
        QString All = "all : $(BIN)\r\n\r\n";
        QString Clean1 = "clean :\r\n";
        QString Clean2 = "\trm -f $(OBJ) $(BIN) $(OutPath)"+ProjectName+"_lib.a "+"$(OutPath)*.asm $(OutPath)*_src.asm $(OutPath)*.bin\r\n\r\n";
        MakeFile.write(All.toUtf8());
        MakeFile.write(Clean1.toUtf8());
        MakeFile.write(Clean2.toUtf8());

        //MakeFile output file
        QString MakeExe = "$(BIN): $(OBJ)\r\n";
        if(UseCkExeMessage=="true"){
             MakeExe += "\t$(LD) $(LINKOBJ) -o \"$(OutPath)source.exe\" $(LIBS)\r\n";
             MakeExe += "\t$(CK) $(OutPath)source.exe $(OutPath)"+ProjectName+".exe\r\n";
        }else{
             MakeExe += "\t$(LD) $(LINKOBJ) -o \"$(OutPath)"+ProjectName+".exe\" $(LIBS)\r\n";
        }
        MakeExe += "\t$(AR) -r $(OutPath)"+ProjectName+"_lib.a $(OBJ)\r\n";
        MakeExe += "\t$(SIZE) $(OutPath)"+ProjectName+".exe\r\n";
        if(ReadElfMessage == "true"){
            MakeExe += "\t$(READELF) -a $(OutPath)"+ProjectName+".exe > $(OutPath)"+ProjectName+".readelf\r\n";
        }
        MakeExe += "\t$(OBJDUMP) -D $(OutPath)"+ProjectName+".exe > $(OutPath)"+ProjectName+".asm\r\n";
        MakeExe += "\t$(OBJCOPY) -O binary $(OutPath)"+ProjectName+".exe $(OutPath)"+ProjectName+".bin\r\n";
        MakeExe += "\t$(OBJDUMP) -S $(OutPath)"+ProjectName+".exe > $(OutPath)"+ProjectName+"_src.asm\r\n\r\n";

        //MakeFile .o file
        QString MakeOutput = "";
        for(int i = 0;i<SourceFile.size();i++){
            QFileInfo SourceInfo(SourceFile.at(i));
            MakeOutput += "$(OutPath)"+SourceInfo.fileName().split(".").at(0)+".o : "+SourceInfo.filePath()+"\r\n";
            MakeOutput += "\t$(CC) -c "+SourceInfo.filePath()+" -o "+"$(OutPath)"+SourceInfo.fileName().split(".").at(0)+".o $(CFLAGS)\r\n\r\n";
        }

        MakeFile.write(MakeExe.toUtf8());
        MakeFile.write(MakeOutput.toUtf8());

        MakeFile.close();
        return true;

    }else{
        return false;
    }
}
