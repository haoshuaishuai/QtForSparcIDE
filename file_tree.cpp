#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"
#include "file_tree.h"
#include "mainwindow.h"
#include "status_bar.h"
#include "jump_file.h"
#include "qsci_editor.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <Qt>
#include <QDialog>
#include <QVBoxLayout>
#include <QMenu>
#include <QScrollBar>
#include <QHeaderView>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QFileDialog>
#include <QSettings>
#include <QTabBar>

//construct func
treemenu::treemenu(QWidget *parent,class edit *EditMenu,configfile *SettingFile,statusbar *StatusBar,jump_file *CtrlClickJump):QTreeWidget(parent){

    this->setObjectName("TreeMenu");
    this->verticalScrollBar()->setObjectName("TreeMenuVerticl");
    this->horizontalScrollBar()->setObjectName("TreeMenuHorizontal");

    this->setColumnCount(1);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->setAutoScroll(true);
    this->header()->setVisible(false);

    this->setFrameStyle(QFrame::Sunken);
    this->setAnimated(true);

    newFileWidget = new NewBulidFile();

    newDirWidget = new NewBuildfolder();

    newRenameFile = new NewRenameFile();

    //init class
    this->FileWatcher = new QFileSystemWatcher();

    this->EditMenu = EditMenu;
    this->SettingFile = SettingFile;

    this->StatusBar = StatusBar;

    this->CtrlClickJump = CtrlClickJump;

    // create right click menu of file
    dirMenu = new QMenu();
    dirMenu->addAction("New File");
    dirMenu->addAction("Add File");
    dirMenu->addSeparator();
    dirMenu->addAction("New Folder");
    dirMenu->addAction("Add Folder");
    dirMenu->addAction("Delate Folder");

    //create right click menu of file
    fileMenu = new QMenu();
    fileMenu->addAction("Rename");
    fileMenu->addAction("Delate File");
    fileMenu->addAction("Open Dir");
    fileMenu->addAction("Set 1st Compile");

    //set icon path
    this->IconPath = ":/new/prefix1/res/";

    //set position 0
    Position = -1;
    EditString = new QList<QString>();

}

void treemenu::ConnectSignal(){

    connect(newFileWidget->okButton,SIGNAL(clicked(bool)),this,SLOT(bulidNewFileSlot(bool)));
    connect(newFileWidget->cancalButton,SIGNAL(clicked(bool)),this,SLOT(closeBuildFileSlot(bool)));
    connect(newDirWidget->okButton,SIGNAL(clicked(bool)),this,SLOT(buildNewDirSlot(bool)));
    connect(newDirWidget->cancalButton,SIGNAL(clicked(bool)),this,SLOT(cancelDirSlot(bool)));
    connect(newRenameFile->okButton,SIGNAL(clicked(bool)),this,SLOT(renameFileSlot()));
    connect(newRenameFile->cancalButton,SIGNAL(clicked(bool)),this,SLOT(cancelRenameSlot(bool)));
    connect(FileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(FileChange(QString)));

    connect(dirMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));
    connect(fileMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));

    connect(this,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(itemPressedSlot(QTreeWidgetItem*,int)));
    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(DoubleClickpath(QTreeWidgetItem*,int)));
    connect(this,SIGNAL(collapseItemSignal(const QTreeWidgetItem*)),this,SLOT(collapseItem(const QTreeWidgetItem*)));

    connect(editTab->tabBar(), SIGNAL(tabBarClicked(int)), this, SLOT(on_TabBar_triggered(int)));

}

void treemenu::on_TabBar_triggered(int index){

    QWidget *clickedWidget = editTab->widget(index);
    if(EditString->size() == 20){
        EditString->removeFirst();
    }
    else{
        Position++;
    }
    if(EditString->size()!=0){
        if(EditString->at(Position-1)!=clickedWidget->whatsThis()){
            if(EditString->size()>Position){
                if(EditString->at(Position)!=clickedWidget->whatsThis()){
                    EditString->insert(Position, clickedWidget->whatsThis());
                }
            }else{
                EditString->insert(Position, clickedWidget->whatsThis());
            }
        }else{
            Position--;
        }
    }else{
        EditString->insert(Position, clickedWidget->whatsThis());
    }

}

//new file
NewBulidFile::NewBulidFile(QWidget *parent):QDialog(parent){

    QLabel *fileNameLabel = new QLabel("file");
    QLabel *fileNameTypeLabel = new QLabel("filelable");
    fileNameEdit = new QLineEdit();
    fileNameTypeBox = new QComboBox();
    fileNameTypeBox->addItem(".c");
    fileNameTypeBox->addItem(".h");
    fileNameTypeBox->addItem(".s");
    okButton = new QPushButton("Ok");
    cancalButton = new QPushButton("Cancel");

    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(fileNameLabel);
    Hlayout1->addWidget(fileNameEdit);

    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    Hlayout2->addWidget(fileNameTypeLabel);
    Hlayout2->addWidget(fileNameTypeBox);

    QHBoxLayout *Hlayout3 = new QHBoxLayout();
    Hlayout3->addWidget(okButton);
    Hlayout3->addWidget(cancalButton);

    QVBoxLayout *MainVlayout = new QVBoxLayout(this);
    MainVlayout->addLayout(Hlayout1);
    MainVlayout->addLayout(Hlayout2);
    MainVlayout->addLayout(Hlayout3);

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

}

//new folder
NewBuildfolder::NewBuildfolder(QWidget *parent):QDialog(parent){

    QLabel *fileNameLabel = new QLabel("Folder");
    fileNameEdit = new QLineEdit();
    okButton = new QPushButton("Ok");
    cancalButton = new QPushButton("Cancel");

    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(fileNameLabel);
    Hlayout1->addWidget(fileNameEdit);

    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    Hlayout2->addWidget(okButton);
    Hlayout2->addWidget(cancalButton);

    QVBoxLayout *MainVlayout = new QVBoxLayout(this);
    MainVlayout->addLayout(Hlayout1);
    MainVlayout->addLayout(Hlayout2);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

}

//rename
NewRenameFile::NewRenameFile(QWidget *parent):QDialog(parent){

    QLabel *fileNameLabel = new QLabel("Rename");
    fileNameEdit = new QLineEdit();
    okButton = new QPushButton("Ok");
    cancalButton = new QPushButton("Cancel");

    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(fileNameLabel);
    Hlayout1->addWidget(fileNameEdit);

    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    Hlayout2->addWidget(okButton);
    Hlayout2->addWidget(cancalButton);

    QVBoxLayout *MainVlayout = new QVBoxLayout(this);
    MainVlayout->addLayout(Hlayout1);
    MainVlayout->addLayout(Hlayout2);

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

}


//create top level
void treemenu::CreateTopItem(QString sdkpath,QString path,bool IsCreate){

    //init watch files & dirs
    if(WatchFiles.size()!=0){
        FileWatcher->removePaths(WatchFiles);
        WatchFiles.clear();
    }

    if(WatchDirs.size()!=0){
        WatchDirs.clear();
    }

    //set root
    QFileInfo info(sdkpath);
    QDir *rootdir = new QDir(path);
    root = new QTreeWidgetItem();
    this->addTopLevelItem(root);
    root->setText(0,info.baseName());
    root->setIcon(0,QIcon(IconPath+"tree.jpg"));

    //sdk file
    SdkFile = sdkpath;

    //create tree
    FindFile(rootdir,root,SdkFile,IsCreate);
    QApplication::processEvents();
    root->setExpanded(true);

    //watch files and dirs
    if(!WatchFiles.isEmpty())
    FileWatcher->addPaths(WatchFiles);

    //find not found dirs
    QStringList IncludeDir = SettingFile->readSdkFile(SdkFile,"IncludeDir");
    QString NotFoundDir;
    for(int i=0;i<IncludeDir.size();i++){
        for(int j=0;j<WatchDirs.size();j++){
           if(WatchDirs.at(j)==IncludeDir.at(i)){
               break;
           }
           if(j==WatchDirs.size()-1){
                NotFoundDir.append(IncludeDir.at(i)+"\n");
           }
        }
    }

    //find not found files
    QStringList IncludeFiles = SettingFile->readSdkAllFile(SdkFile);
    QString NotFoundFiles;
    for(int i=0;i<IncludeFiles.size();i++){
        for(int j=0;j<WatchFiles.size();j++){
           if(WatchFiles.at(j)==IncludeFiles.at(i)){
               break;
           }
           if(j==WatchFiles.size()-1){
                NotFoundFiles.append(IncludeFiles.at(i)+"\n");
           }
        }
    }

    //show notfound message
    if(NotFoundDir.size()!=0 && NotFoundFiles.size()!=0){
        QMessageBox::warning(this, tr("Application"),tr("Can not find:\n"+NotFoundDir.toUtf8()+NotFoundFiles.toUtf8()));
    }else if(NotFoundDir.size()!=0){
        QMessageBox::warning(this, tr("Application"),tr("Can not find:\n"+NotFoundDir.toUtf8()));
    }else if(NotFoundFiles.size()!=0){
        QMessageBox::warning(this, tr("Application"),tr("Can not find:\n"+NotFoundFiles.toUtf8()));
    }

}

//remove top level
void treemenu::RemoveTopItem(){

    if(root != NULL){

        this->clear();
        editTab->clear();
        editlist->clear();

    }

}

//file search
void treemenu::FindFile(QDir *path,QTreeWidgetItem *parent,QString SdkFile,bool IsCreate){

    QApplication::processEvents();
    QStringList AllDirName = path->entryList();
    QString PathContent = path->absolutePath();
    for(int i=2;i<AllDirName.size();i++){

        QFileInfo *FileInfo = new QFileInfo(PathContent+"/"+AllDirName[i]);
        if(FileInfo->isFile()){

            if(!FileInfo->filePath().contains(".sdk") & !FileInfo->filePath().contains(".ini")){

                if(IsCreate){

                    QTreeWidgetItem *child = new QTreeWidgetItem(parent);
                    child->setIcon(0,QIcon(IconPath+"text.jpg"));
                    child->setText(0,AllDirName[i]);
                    child->setToolTip(0,PathContent+"/"+AllDirName[i]);

                    if(FileInfo->filePath().contains("Headers")){
                        SettingFile->writeSdkFile(SdkFile,"IncludeFile",FileInfo->filePath());
                        WatchFiles.append(FileInfo->filePath());
                    }else if(FileInfo->filePath().contains("Sources")){
                        SettingFile->writeSdkFile(SdkFile,"SourceFile",FileInfo->filePath());
                        WatchFiles.append(FileInfo->filePath());
                    }else if(FileInfo->filePath().contains(".ld")){
                        SettingFile->writeSdkFile(SdkFile,"LinkFile",FileInfo->filePath());
                        WatchFiles.append(FileInfo->filePath());
                    }else if(FileInfo->filePath().contains(".win")){
                        SettingFile->writeSdkFile(SdkFile,"MakeFile",FileInfo->filePath());
                        WatchFiles.append(FileInfo->filePath());
                    }else if(FileInfo->filePath().contains(".asm" ) || FileInfo->filePath().contains(".bin") || FileInfo->filePath().contains(".o")
                             || FileInfo->filePath().contains(".exe") || FileInfo->filePath().contains(".a") || FileInfo->filePath().contains(".readelf")){
                        SettingFile->writeSdkFile(SdkFile,"OutputsFile",FileInfo->filePath());
                        WatchFiles.append(FileInfo->filePath());
                    }

                    if(FileInfo->filePath().contains(".S" )){
                        SettingFile->writeSdkFile(SdkFile,"FirstFile",FileInfo->filePath());
                    }

                }else{

                    if(SettingFile->readSdkAllFile(SdkFile).contains(FileInfo->filePath())){

                        QTreeWidgetItem *child = new QTreeWidgetItem(parent);
                        child->setIcon(0,QIcon(IconPath+"text.jpg"));
                        child->setText(0,AllDirName[i]);
                        child->setToolTip(0,PathContent+"/"+AllDirName[i]);

                        WatchFiles.append(FileInfo->filePath());

                    }   

                }

            }

        }
        else if(FileInfo->isDir()){

            if(!FileInfo->filePath().contains(".Configure")){

                if(IsCreate){

                    QDir *NextDir = new QDir(PathContent+"/"+AllDirName[i]);
                    QTreeWidgetItem *child = new QTreeWidgetItem(parent);
                    child->setIcon(0,QIcon(IconPath+"file.jpg"));
                    child->setText(0,AllDirName[i]);
                    child->setToolTip(0,PathContent+"/"+AllDirName[i]);

                    SettingFile->writeSdkFile(SdkFile,"IncludeDir",FileInfo->filePath());
                    FindFile(NextDir,child,SdkFile,IsCreate);

                    WatchDirs.append(FileInfo->filePath());

                }else{

                    if(SettingFile->readSdkFile(SdkFile,"IncludeDir").contains(FileInfo->filePath())){

                        QDir *NextDir = new QDir(PathContent+"/"+AllDirName[i]);
                        QTreeWidgetItem *child = new QTreeWidgetItem(parent);
                        child->setIcon(0,QIcon(IconPath+"file.jpg"));
                        child->setText(0,AllDirName[i]);
                        child->setToolTip(0,PathContent+"/"+AllDirName[i]);
                        FindFile(NextDir,child,SdkFile,IsCreate);

                        WatchDirs.append(FileInfo->filePath());

                    }else{

                        continue;

                    }
                }
            }
        }
    }
}

//listen file changed
void treemenu::FileChange(QString Path){

    for(int i=0;i<editTab->count();i++){

          QString filepath = editTab->widget(i)->whatsThis();

          if(filepath==Path){

              //editTab->setCurrentIndex(i);

              if(ChangedFiles.contains(Path)){
                  return;
              }

              ChangedFiles.append(Path);
              QMessageBox::StandardButton box=QMessageBox::information(this,QString::fromLocal8Bit("Warning"),
                                                                       QString::fromLocal8Bit(Path.toUtf8()+" Changed, Do You Want Reload?"),
                                                                       QMessageBox::Yes|QMessageBox::No);
              ChangedFiles.removeOne(Path);

              switch(box){
                  case QMessageBox::No:{
                    if(editTab->tabText(i)[0]!="*"){
                       editTab->setTabText(i,"*"+(editTab->tabText(i)));
                    }
                    return;
                  }
              }

              QFile file(filepath);
              if (!file.open(QFile::ReadOnly)) {
                  QMessageBox::warning(this, tr("Application"),
                                       tr("Cannot read file %1:\n%2.")
                                       .arg(filepath)
                                       .arg(file.errorString()));
                  return;
              }

              QTextStream in(&file);
              QApplication::setOverrideCursor(Qt::WaitCursor);
              for(int j=0;j<editlist->size();j++){
                  if(editlist->at(j)->whatsThis()==editTab->widget(i)->whatsThis()){
                       editlist->at(j)->setText(in.readAll());
                  }
              }
              QApplication::restoreOverrideCursor();

              if(editTab->tabText(i)[0]=="*"){
                  editTab->setTabText(i,editTab->tabText(i).remove(0,1));
              }

              break;

        }

    }

}

//show file info
void treemenu::DoubleClickpath(QTreeWidgetItem *item, int){

    QString absolute_Path = item->toolTip(0);
    QFileInfo *FileInfo = new QFileInfo(absolute_Path);
    this->resizeColumnToContents(0);

    if(FileInfo->isFile()){

        emit showTextSignal(absolute_Path,FileInfo->baseName(),item);

    }
    else{

    }

}

//show file contex
void treemenu::itemPressedSlot(QTreeWidgetItem * pressedItem, int column){

    this->resizeColumnToContents(0);
    nowItem = pressedItem;
    nowCol = column;

    if(qApp->mouseButtons() == Qt::RightButton){

        if (pressedItem->parent() == nullptr) {
              return;
        }

        QString absolute_Path = pressedItem->toolTip(0);
        QFileInfo *FileInfo = new QFileInfo(absolute_Path);
        if(FileInfo->isFile()){
            fileMenu->exec(QCursor::pos());
        }
        else{
            dirMenu->exec(QCursor::pos());
        }

    }

    if(qApp->mouseButtons() == Qt::LeftButton){

        if(pressedItem!=root){

            QString absolute_Path = pressedItem->toolTip(0);
            QFile file(absolute_Path);
            if (!file.exists()) {
                QMessageBox::warning(this, tr("Application"),tr("%1 Not Exist!\n").arg(absolute_Path));
                return;
            }
            QFileInfo *FileInfo = new QFileInfo(absolute_Path);
            if(FileInfo->isFile()){
                this->loadFile(absolute_Path);
            }

        }
    }

}

//show file in edit widget
void treemenu::loadFile(const QString &FilePath){

    QFile file(FilePath);

    if (!file.open(QFile::ReadOnly)) {

        QMessageBox::warning(this, tr("Application"),tr("Can not read file %1:\n%2.").arg(FilePath).arg(file.errorString()));
        return;

    }

    //load file,check it is or not listen file
    if(WatchFiles.contains(FilePath)){
        WatchFiles.removeOne(FilePath);
    }
    WatchFiles.append(FilePath);
    FileWatcher->removePaths(FileWatcher->files());
    FileWatcher->addPaths(WatchFiles);

    int index=0;
    for(int i=0;i<FilePath.length();i++){

        if(FilePath[i]=="/")
            index=i;

    }
    QString filename = FilePath.mid(index+1,FilePath.length());

    //add last&next window list
    if(EditString->size() == 20){
        EditString->removeFirst();
    }
    else{
        Position++;
    }
    if(EditString->size()!=0){
        if(EditString->at(Position-1)!=FilePath){
            if(EditString->size()>Position){
                if(EditString->at(Position)!=FilePath){
                    EditString->insert(Position, FilePath);
                }
            }else{
                EditString->insert(Position, FilePath);
            }
        }else{
            Position--;
        }
    }else{
        EditString->insert(Position, FilePath);
    }

    //if this file is opened,set it as current file
    for (int i=0;i<editTab->count();i++) {

        if(editTab->widget(i)->whatsThis()==FilePath){
            editTab->setCurrentIndex(i);
            return;
        }

    }

    //if this file not open,create file edit
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    qscieditor *Edit = new qscieditor(this);
    EditMenu->SetEdit(Edit);
    editTab->addTab(Edit,filename);
    Edit->setWhatsThis(FilePath);

    editlist->append(Edit);

    Edit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    SetCurrentFile(FilePath,Edit);

    editTab->setCurrentWidget(Edit);

    editTab->currentWidget()->setWhatsThis(FilePath);

    //add textchange listener
    connect(Edit, &QsciScintilla::textChanged,this,[=]{

        for (int i = 0; i < editTab->count(); ++i) {
            QWidget* widget = editTab->widget(i);
            if(widget->whatsThis()==Edit->whatsThis()){
                if(editTab->tabText(i)[0]!="*"){
                    editTab->setTabText(i,"*"+(editTab->tabText(i)));
                }
            }
        }
        Edit->SendScintilla(QsciScintilla::SCI_INDICATORFILLRANGE, 0, Edit->length());

    });

    //add ctrl+click listener
    Edit->SendScintilla(QsciScintilla::SCI_INDICSETSTYLE, 0, 5);
    Edit->SendScintilla(QsciScintilla::SCI_SETINDICATORCURRENT, 0);
    Edit->SendScintilla(QsciScintilla::SCI_INDICATORFILLRANGE, 0, Edit->length());
    connect(Edit, &QsciScintilla::indicatorReleased,this,[=](int line, int index, Qt::KeyboardModifiers state){

        int position = Edit->positionFromLineIndex(line, index);
        int style = Edit->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, position);
        if (style != 11) {
            return;
        }

        if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)) {

            QString word = Edit->wordAtLineIndex(line, index);
            QPair<QString,QPair<QString, int>> getDefPos = CtrlClickJump->DefinePos(SdkFile,Edit->whatsThis(),word);
            QFileInfo DefFile(getDefPos.first);
            int Line = getDefPos.second.second;
            if(DefFile.exists()){

                loadFile(DefFile.filePath());
                QTimer::singleShot(1, this, [=]() {
                    for (int i = 0; i < editlist->size(); ++i) {
                        if(editlist->at(i)->whatsThis()==DefFile.filePath()){
                            editlist->at(i)->setCursorPosition(Line-1,0);
                        }
                    }
                });

            }else{

                QPair<QString,QPair<QString, int>> getFuncPos = CtrlClickJump->FuncPos(SdkFile,Edit->whatsThis(),word);

                QFileInfo FuncFile(getFuncPos.first);
                Line = getFuncPos.second.second;
                if(FuncFile.exists()){
                    loadFile(FuncFile.filePath());
                    QTimer::singleShot(1, this, [=]() {
                        for (int i = 0; i < editlist->size(); ++i) {
                            if(editlist->at(i)->whatsThis()==FuncFile.filePath()){
                                editlist->at(i)->setCursorPosition(Line-1,0);
                            }
                        }
                    });
                }

            }

        }

        Edit->SendScintilla(QsciScintilla::SCI_STYLESETHOTSPOT, 11, false);

    });

    QObject::connect(Edit->customAction, &QAction::triggered, [=]() {

        int pos, index;
        Edit->getCursorPosition(&pos, &index);
        QString line = Edit->text(pos);
        if (line.trimmed().startsWith("#include")) {
            QRegExp rx("\"(.*)\"");
            if (rx.indexIn(line) != -1) {
                QString includeFile = CtrlClickJump->getIncludeList(SdkFile,rx.cap(1));
                if(includeFile!=""){
                    loadFile(includeFile);
                }
            }
        }

    });

    //load break point for file
    QFileInfo SdkFileInfo(SdkFile);
    QString GlobalConfigFile = SdkFileInfo.path()+"/.Configure/Config.ini";
    QSettings GlobalSettingFile(GlobalConfigFile,QSettings::IniFormat);
    int maxsize = GlobalSettingFile.beginReadArray("BreakInfo");
    for (int h = 0; h < maxsize; h++) {
        GlobalSettingFile.setArrayIndex(h);
        if(GlobalSettingFile.value("BreakPointFile")==Edit->whatsThis()){

            int lineAt  = GlobalSettingFile.value("BreakPointLine").toInt()-1;
            int masknAt=Edit->SendScintilla(QsciScintilla::SCI_MARKERGET,lineAt);
            if(masknAt == 0){
                Edit->SendScintilla(QsciScintilla::SCI_MARKERADD,lineAt,1);
            }

        }

    }
    GlobalSettingFile.endArray();

    //set break point
    connect(Edit, &QsciScintilla::marginClicked,this,[=](int margin, int line, Qt::KeyboardModifiers state){

        QFileInfo FileInfo(Edit->whatsThis());
        QString FileName = FileInfo.fileName();

        if(FileName.contains(".ASM") || FileName.contains(".asm")){

            int maskn=Edit->SendScintilla(QsciScintilla::SCI_MARKERGET,line);
            if(maskn == 0){

                QString BreakAddr = Edit->text(line);
                QRegularExpression regex("[\\da-f]+:.*(\\s*[\\da-f]){8}");
                QRegularExpressionMatch match = regex.match(BreakAddr);
                if (!match.hasMatch()) {
                    return;
                }

                Edit->SendScintilla(QsciScintilla::SCI_MARKERADD,line,margin);
                QFileInfo SdkFileInfo(SdkFile);
                QString GlobalConfig = SdkFileInfo.path()+"/.Configure/Config.ini";
                QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

                int size = GlobalSetting.beginReadArray("BreakInfo");
                GlobalSetting.endArray();
                GlobalSetting.beginWriteArray("BreakInfo");
                GlobalSetting.setArrayIndex(size);
                GlobalSetting.setValue("BreakPointLine",line+1);
                GlobalSetting.setValue("BreakPointFile",Edit->whatsThis());
                GlobalSetting.setValue("BreakPointEnable","Enable");
                QString extractedText = match.captured(0);
                GlobalSetting.setValue("BreakAddr",extractedText.split(":")[0]);
                GlobalSetting.sync();
                GlobalSetting.endArray();

            }
            else{

                Edit->SendScintilla(QsciScintilla::SCI_MARKERDELETE,line,margin);

                //QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
                QFileInfo SdkFileInfo(SdkFile);
                QString GlobalConfig = SdkFileInfo.path()+"/.Configure/Config.ini";
                QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

                int size = GlobalSetting.beginReadArray("BreakInfo");
                QStringList *LineList = new QStringList();
                QStringList *FileList = new QStringList();
                QStringList *AddrList = new QStringList();
                QStringList *EnableList = new QStringList();
                for (int i = 0; i < size; i++) {
                    GlobalSetting.setArrayIndex(i);
                    if(GlobalSetting.value("BreakPointLine")!=(line+1) || GlobalSetting.value("BreakPointFile")!=Edit->whatsThis()){
                        LineList->append(GlobalSetting.value("BreakPointLine").toString());
                        FileList->append(GlobalSetting.value("BreakPointFile").toString());
                        AddrList->append(GlobalSetting.value("BreakAddr").toString());
                        EnableList->append(GlobalSetting.value("BreakPointEnable").toString());
                    }
                }

                GlobalSetting.endArray();
                GlobalSetting.remove("/BreakInfo");
                GlobalSetting.sync();

                if(LineList->size()!=0){
                    GlobalSetting.beginWriteArray("BreakInfo");
                    for (int i = 0; i < LineList->size(); i++) {
                        GlobalSetting.setArrayIndex(i);
                        GlobalSetting.setValue("BreakPointLine",LineList->at(i));
                        GlobalSetting.setValue("BreakPointFile",FileList->at(i));
                        GlobalSetting.setValue("BreakAddr",AddrList->at(i));
                        GlobalSetting.setValue("BreakPointEnable",EnableList->at(i));
                    }
                    GlobalSetting.sync();
                    GlobalSetting.endArray();
                }

            }
        }

    });

    editTab->show();
    CentralWidget->setStyleSheet("");

}

//set current file to current file
void treemenu::SetCurrentFile(const QString &fileName,qscieditor *edit){

    QString curFile = fileName;
    edit->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(curFile).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));

}


//all actions
void treemenu::tempActionInformation(QAction *act){

    if(act->text() == "Delate File"){

        QTreeWidgetItem *item = this->currentItem();

        QString FilePath = item->toolTip(0);
        SettingFile->removeSdkFile(SdkFile,FilePath);

        delete item;

        for(int i=0;i<editTab->count();i++){
            if(editTab->widget(i)->whatsThis()==FilePath){
                EditWidget->CloseTab(i);
                break;
            }
        }

        StatusBar->DetermineMessage("Deleate File Success");

    }

    if(act->text() == "Rename"){

        QString lastName = nowItem->text(0);
        newRenameFile->fileNameEdit->setText(lastName);
        newRenameFile->setModal(true);
        newRenameFile->show();

    }

    if(act->text() == "Open Dir"){

        QString path = nowItem->toolTip(0);
        int index=0;
        for(int i=0;i<path.length();i++){
            if(path[i]=="/")
                index=i;
        }
        QString filePath = path.mid(0,index).replace("/","\\");
        QProcess::startDetached("explorer "+filePath);
        StatusBar->DetermineMessage("Open Dir Success");

    }

    if(act->text() == "Set 1st Compile"){

        QString path = nowItem->toolTip(0);
        QStringList FirstFile = SettingFile->readSdkFile(SdkFile,"FirstFile");
        if(FirstFile.size()!=0){
            SettingFile->removeSdk1File(SdkFile,"FirstFile",FirstFile.at(0));
        }
        SettingFile->writeSdkFile(SdkFile,"FirstFile",path);
        StatusBar->DetermineMessage("Set 1st Compile Success");

    }

    if(act->text() == "New File"){

        newFileWidget->setModal(true);
        newFileWidget->show();

    }

    if(act->text() == "Add File"){

        QString PathName = QFileDialog::getOpenFileName(this, "open project or file",QCoreApplication::applicationDirPath(),
                                                      "SDK File(*.c *.h *.s *.C *.H *.S);;ALL File(*.*)");

        if (!PathName.isEmpty()){

            QFileInfo FileInfo(PathName);
            QString FileName = FileInfo.fileName();
            QString NewPath  = nowItem->toolTip(0);
            NewPath = NewPath+"/"+FileName;

            if(PathName != NewPath){
                if(!QFile::copy(PathName,NewPath)){

                   QMessageBox::warning(this, tr("Warning"),tr("Copy File To Project Failed!"));
                   return;

                }
            }

            //edit sdk file name
            if(FileName.contains(".h")||FileName.contains(".H")){
                SettingFile->writeSdkFile(SdkFile,"IncludeFile",NewPath);
            }else if(FileName.contains(".c")||FileName.contains(".C")||FileName.contains(".s")||FileName.contains(".S")){
                SettingFile->writeSdkFile(SdkFile,"SourceFile",NewPath);
            }else if(FileName.contains(".ld")){
                SettingFile->writeSdkFile(SdkFile,"LinkFile",NewPath);
            }else if(FileName.contains(".win")){
                SettingFile->writeSdkFile(SdkFile,"MakeFile",NewPath);
            }else if(FileName.contains(".asm" ) || FileName.contains(".bin") || FileName.contains(".o")
                     || FileName.contains(".exe") || FileName.contains(".a") || FileName.contains(".readelf")){
                SettingFile->writeSdkFile(SdkFile,"OutputsFile",NewPath);
            }
            else{
                SettingFile->writeSdkFile(SdkFile,"OtherFile",NewPath);
            }

            //make tree child
            QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
            child->setText(0,FileName);
            child->setIcon(0,QIcon(IconPath+"text.jpg"));
            child->setToolTip(0,NewPath);
            StatusBar->DetermineMessage("Add File Success");

        }else{

            StatusBar->DetermineMessage("Don't Add File");

        }

    }

    if(act->text()=="New Folder"){

        newDirWidget->setModal(true);
        newDirWidget->exec();

    }

    if(act->text()=="Add Folder"){

        QString PathName = QFileDialog::getExistingDirectory(this, "add folder",QCoreApplication::applicationDirPath());

        if (PathName!=""){

            int index = PathName.split("/").size()-1;
            QString LastDir = PathName.split("/").at(index);

            QString NewPath  = nowItem->toolTip(0);
            NewPath = NewPath+"/"+LastDir;

            QDir *LastPath = new QDir(PathName);

            if(PathName != NewPath){

                QDir MakeDir;
                if(!MakeDir.mkdir(NewPath)){
                   QMessageBox::warning(this, tr("Warning"),tr("Copy Folder To Project Failed!"));
                   return;
                }

                //create parenet
                QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
                child->setIcon(0,QIcon(IconPath+"file.jpg"));
                child->setText(0,LastDir);
                child->setToolTip(0,NewPath);

                SettingFile->writeSdkFile(SdkFile,"IncludeDir",NewPath);

                NewDir(LastPath,child,NewPath,true);

            }else{

                for(int i=0;i<nowItem->childCount();i++){
                     if(nowItem->child(i)->toolTip(0)==NewPath){
                         return;
                     }
                }

                //create parenet
                QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
                child->setIcon(0,QIcon(IconPath+"file.jpg"));
                child->setText(0,LastDir);
                child->setToolTip(0,NewPath);

                SettingFile->writeSdkFile(SdkFile,"IncludeDir",NewPath);

                NewDir(LastPath,child,NewPath,false);

            }
            StatusBar->DetermineMessage("Add Folder Success");

        }else{

            StatusBar->DetermineMessage("Don't Add Folder");

        }

    }

    if(act->text()=="Delate Folder"){

        QTreeWidgetItem *item = this->currentItem();
        for(int i=0;i<item->childCount();i++){

            QString Path = item->child(i)->toolTip(0);
            SettingFile->removeSdkFile(SdkFile,Path);

            for(int i=0;i<editTab->count();i++){
                if(editTab->widget(i)->whatsThis()==Path){
                    EditWidget->CloseTab(i);
                    break;
                }
            }
         }
        SettingFile->removeSdkFile(SdkFile,item->toolTip(0));

        QTreeWidgetItem* parItem=item->parent();

        if(parItem==0){
            delete item;
        }
        else {
            parItem->removeChild(item) ;
            delete item;
        }

        StatusBar->DetermineMessage("Delate Folder Success");

    }
}


//file search
void treemenu::NewDir(QDir *path,QTreeWidgetItem *parent,QString NewPath,bool NeedCopy){

    QApplication::processEvents();
    QStringList AllDirName = path->entryList();
    QString PathContent = path->absolutePath();

    for(int i=2;i<AllDirName.size();i++){

        QFileInfo *FileInfo = new QFileInfo(PathContent+"/"+AllDirName[i]);
        if(FileInfo->isFile()){

            QString FilePath = NewPath + "/"+FileInfo->fileName();
            if(NeedCopy){
                if(!QFile::copy(FileInfo->filePath(),FilePath)){
                   QMessageBox::warning(this, tr("Warning"),tr("Copy File To Project Failed!"+FileInfo->filePath().toUtf8()+FilePath.toUtf8()));
                   continue;
                }
            }

            QString Name = FileInfo->fileName();
            if(Name.contains(".h") || Name.contains(".H")){
                SettingFile->writeSdkFile(SdkFile,"IncludeFile",FilePath);
            }else if(Name.contains(".c") || Name.contains(".C") || Name.contains(".S") || Name.contains(".s")){
                SettingFile->writeSdkFile(SdkFile,"SourceFile",FilePath);
            }else if(Name.contains(".ld")){
                SettingFile->writeSdkFile(SdkFile,"LinkFile",FilePath);
            }else if(Name.contains(".win")){
                SettingFile->writeSdkFile(SdkFile,"MakeFile",FilePath);
            }else if(Name.contains(".asm" ) || Name.contains(".bin") || Name.contains(".o")
                     || Name.contains(".exe") || Name.contains(".a")){
                SettingFile->writeSdkFile(SdkFile,"OutputsFile",FilePath);
            }

            QTreeWidgetItem *child = new QTreeWidgetItem(parent);
            child->setIcon(0,QIcon(IconPath+"text.jpg"));
            child->setText(0,AllDirName[i]);
            child->setToolTip(0,FilePath);

        }
        else if(FileInfo->isDir()){

            QDir *NextDir = new QDir(PathContent+"/"+AllDirName[i]);

            QString FilePath = NewPath + +"/"+AllDirName[i];
            if(NeedCopy){

                 QDir MakeDir;
                 if(!MakeDir.mkdir(FilePath)){
                    QMessageBox::warning(this, tr("Warning"),tr("Copy Folder To Project Failed!"));
                    continue;
                 }

            }

            SettingFile->writeSdkFile(SdkFile,"IncludeDir",FilePath);

            QTreeWidgetItem *child = new QTreeWidgetItem(parent);
            child->setIcon(0,QIcon(IconPath+"file.jpg"));
            child->setText(0,AllDirName[i]);
            child->setToolTip(0,PathContent+"/"+AllDirName[i]);

            NewDir(NextDir,child,FilePath,NeedCopy);

        }
    }
}

//emit signals
void treemenu::buttonCollapseItem(){

    QTreeWidgetItem *t = this->currentItem();
    if(t==0)
        return;

    if(t->childCount()==0){

        QTreeWidgetItem *p = t->parent();
        if(p==0){
            return;
        }
        else{
            emit collapseItemSignal(p);
        }

    }
    else{
        emit collapseItemSignal(t);
    }

}

void treemenu::bulidNewFileSlot(bool){

    QString fileName = newFileWidget->fileNameEdit->text();
    QString filetype = newFileWidget->fileNameTypeBox->currentText();

    if(fileName==""){

        QMessageBox::warning(this, tr("Warning"),tr("File Name Null!"));
        StatusBar->DetermineMessage("Create New File Failed");
        return;

    }

    QString tempName = fileName+filetype;
    QString path = nowItem->toolTip(0)+"/"+tempName;
    QFile file(path);

    if(file.exists()){

        QMessageBox::warning(this, tr("Warning"),tr("File Exists!"));
        StatusBar->DetermineMessage("Create New File Failed");
        return;

    }

    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.close();

    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
    child->setText(0,tempName);
    child->setIcon(0,QIcon(IconPath+"text.jpg"));
    child->setToolTip(0,path);

    //edit sdk file name
    if(tempName.contains(".h")||tempName.contains(".H")){
        SettingFile->writeSdkFile(SdkFile,"IncludeFile",path);
    }else if(tempName.contains(".c")||tempName.contains(".C")||tempName.contains(".s")||tempName.contains(".S")){
        SettingFile->writeSdkFile(SdkFile,"SourceFile",path);
    }else if(tempName.contains(".ld")){
        SettingFile->writeSdkFile(SdkFile,"LinkFile",path);
    }else if(tempName.contains(".win")){
        SettingFile->writeSdkFile(SdkFile,"MakeFile",path);
    }else if(tempName.contains(".asm" ) || tempName.contains(".bin") || tempName.contains(".o")
             || tempName.contains(".exe") || tempName.contains(".a") || tempName.contains(".readelf")){
        SettingFile->writeSdkFile(SdkFile,"OutputsFile",path);
    }
    else{
        SettingFile->writeSdkFile(SdkFile,"OtherFile",path);
    }

    newFileWidget->close();

    StatusBar->DetermineMessage("Create New File Success");

}

bool treemenu::CreateNewFile(QString FilePath){

    QFileInfo FileInfo(FilePath);
    QString tempName = FileInfo.fileName();
    QString path = FileInfo.filePath();
    QString Style = "";

    //edit sdk file name
    if(tempName.contains(".h")||tempName.contains(".H")){
        Style = "Headers";
        SettingFile->writeSdkFile(SdkFile,"IncludeFile",path);
    }else if(tempName.contains(".c")||tempName.contains(".C")||tempName.contains(".s")||tempName.contains(".S")){
        Style = "Sources";
        SettingFile->writeSdkFile(SdkFile,"SourceFile",path);
    }else if(tempName.contains(".ld")){
        Style = "Scripts";
        SettingFile->writeSdkFile(SdkFile,"LinkFile",path);
    }else if(tempName.contains(".win")){
        Style = "Scripts";
        SettingFile->writeSdkFile(SdkFile,"MakeFile",path);
    }else if(tempName.contains(".asm" ) || tempName.contains(".bin") || tempName.contains(".o")
               || tempName.contains(".exe") || tempName.contains(".a") || tempName.contains(".readelf")){
        SettingFile->writeSdkFile(SdkFile,"OutputsFile",path);
    }
    else{
        SettingFile->writeSdkFile(SdkFile,"OtherFile",path);
    }

    QTreeWidgetItemIterator iterator(root);
    QTreeWidgetItem *FileItem = nullptr;
    while (*iterator) {
        if ((*iterator)->text(0) == Style) {
            FileItem = (*iterator);
            break;
        }
        ++iterator;
    }

    if(FileItem!=nullptr){
        QTreeWidgetItem *child = new QTreeWidgetItem(FileItem);
        child->setText(0,tempName);
        child->setIcon(0,QIcon(IconPath+"text.jpg"));
        child->setToolTip(0,path);
        return true;
    }else{
        return false;
    }

}

void treemenu::renameFileSlot(){

    QString fileName = newRenameFile->fileNameEdit->text();
    if(fileName==""){
        QMessageBox::warning(this, tr("Warning"),tr("File Name Null!"));
        StatusBar->DetermineMessage("Rename Failed");
        return;
    }

    FileWatcher->removePaths(FileWatcher->files());

    QString path = nowItem->toolTip(0);
    int indexOfItem = nowItem->parent()->indexOfChild(nowItem);
    QFile file(path);
    int index=0;
    for(int i=0;i<path.length();i++){
        if(path[i]=="/")
            index=i;
    }

    QString filePath = path.mid(0,index).replace("/","/");
    filePath += "/"+fileName;
    bool isEdit=file.rename(filePath);
    if(!isEdit){
        QMessageBox::warning(this, tr("Warning"),tr("Rename Failed,Please Close File And Retry！"));
        StatusBar->DetermineMessage("Rename Failed");
        return;
    }

    QTreeWidgetItem *child = new QTreeWidgetItem();
    nowItem->parent()->insertChild(indexOfItem,child);
    delete nowItem;

    child->setText(0,fileName);
    child->setIcon(0,QIcon(IconPath+"text.jpg"));
    child->setToolTip(0,filePath);
    child->setSelected(true);

    if(indexOfItem !=(child->parent()->childCount()-1))
        child->parent()->child(indexOfItem+1)->setSelected(false);

    //load file,check it is or not listen file
    if(WatchFiles.contains(path)){
        WatchFiles.removeOne(path);
    }
    if(WatchFiles.contains(filePath)){
        WatchFiles.removeOne(filePath);
    }
    WatchFiles.append(filePath);
    FileWatcher->addPaths(WatchFiles);

    //rename edit tab name
    int Index = path.split("/").size()-1;
    QString LastFile = path.split("/").at(Index);
    for (int i=0;i<editTab->count();i++) {
        if(editTab->tabText(i)==LastFile){
            editTab->setCurrentIndex(i);
            editTab->setTabText(editTab->currentIndex(),fileName);
            editTab->widget(i)->setWhatsThis(filePath);
            break;
        }
    }

    //edit sdk file name
    SettingFile->removeSdkFile(SdkFile,path);
    if(fileName.contains(".h")||fileName.contains(".H")){
        SettingFile->writeSdkFile(SdkFile,"IncludeFile",filePath);
    }else if(fileName.contains(".c")||fileName.contains(".C")||fileName.contains(".s")||fileName.contains(".S")){
        SettingFile->writeSdkFile(SdkFile,"SourceFile",filePath);
    }else if(fileName.contains(".ld")){
        SettingFile->writeSdkFile(SdkFile,"LinkFile",filePath);
    }else if(fileName.contains(".win")){
        SettingFile->writeSdkFile(SdkFile,"MakeFile",filePath);
    }else if(fileName.contains(".asm" ) || fileName.contains(".bin") || fileName.contains(".o")
             || fileName.contains(".exe") || fileName.contains(".a") || fileName.contains(".readelf")){
        SettingFile->writeSdkFile(SdkFile,"OutputsFile",filePath);
    }
    else{
        SettingFile->writeSdkFile(SdkFile,"OtherFile",filePath);
    }

    newRenameFile->close();

    StatusBar->DetermineMessage("Rename Success");

}

void treemenu::closeBuildFileSlot(bool){

    newFileWidget->close();

}

void treemenu::buildNewDirSlot(bool){

    QString dirName = newDirWidget->fileNameEdit->text();
    if(dirName==""){
        QMessageBox::warning(this, tr("Application"),tr("File Name is Null!"));
        StatusBar->DetermineMessage("Create Dir Failed");
        return;
    }

    QString path = nowItem->toolTip(0)+"/"+dirName;
    QDir *temp = new QDir;
    bool exist = temp->exists(path);

    if(exist){
        QMessageBox::warning(this,tr("Create Folder"),tr("Folder Exists！"));
        StatusBar->DetermineMessage("Create Dir Failed");
        return;
    }

    temp->mkdir(path);

    SettingFile->writeSdkFile(SdkFile,"IncludeDir",path);

    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
    child->setText(0,dirName);
    child->setIcon(0,QIcon(IconPath+"file.jpg"));
    child->setToolTip(0,path);
    newDirWidget->close();
    StatusBar->DetermineMessage("Create Dir Success");

}

void treemenu::cancelDirSlot(bool){

    newDirWidget->close();

}

void treemenu::cancelRenameSlot(bool){

    newRenameFile->close();

}
