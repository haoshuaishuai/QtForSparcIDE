#include <QAction>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QScrollBar>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QButtonGroup>
#include <QFileDialog>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSpacerItem>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QDateTime>
#include <QDesktopWidget>
#include <QApplication>

#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QProcess>
#include <windows.h>
#include <tlhelp32.h>
#include <unistd.h>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "tools.h"
#include "tool_preference.h"
#include "mybutton.h"
#include "mywidget.h"
#include "file_tree.h"
#include "edit.h"
#include "status_bar.h"
#include "reg_calc.h"

tools::tools(QWidget *parent,file *FileMenu,treemenu *ProjectTree,class edit *Edit,statusbar *StatusBar,QWidget *MainWindow,reg_calc *RegCalc) : QWidget(parent){

    this->FileMenu = FileMenu;
    this->ProjectTree = ProjectTree;
    this->Edit = Edit;
    this->StatusBar = StatusBar;
    this->MainWindow = MainWindow;
    this->RegCalc = RegCalc;
    desktop = QApplication::desktop();
    this->InitWidget();
    this->InitDebugPara();
    this->setWindowIcon(QIcon(":/new/prefix1/res/SDK"));

}

tools::~tools(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        QString quit = "quit\r\n";
        const char * CmdChar= quit.toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
        Process->kill();
        Process->waitForFinished();
        Process->deleteLater();
        Process = 0;
    }

}

//connect signals
void tools::ConnectSignal(){

    connect(SerialPort,&QAction::triggered,this,[=]{on_SerialPort_triggered();});
    connect(CmdDebug,&QAction::triggered,this,[=]{on_CmdDebug_triggered();});
    connect(GenerateFlashFile,&QAction::triggered,this,[=]{on_GenerateFlashFile_triggered();});
    connect(BootFileSelect,&QPushButton::clicked,this,[=]{on_BootFileSelect_triggered();});
    connect(SourceFileSelect,&QPushButton::clicked,this,[=]{on_SourceFileSelect_triggered();});
    connect(TargetFileSelect,&QPushButton::clicked,this,[=]{on_TargetFileSelect_triggered();});
    connect(Generate,&QPushButton::clicked,this,[=]{on_Generate_triggered();});
    connect(RegCalculator,&QAction::triggered,this,[=]{on_RegCalculator_triggered();});
    connect(Preference,&QAction::triggered,this,[=]{on_Preference_triggered();});
    connect(Link, SIGNAL(leftButtonClicked()),this,SLOT(on_Link_Left_triggered()));
    connect(Link, SIGNAL(rightButtonClicked()),this,SLOT(on_Link_Right_triggered()));
    connect(Download, SIGNAL(leftButtonClicked()),this,SLOT(on_Download_Left_triggered()));
    connect(Download, SIGNAL(rightButtonClicked()),this,SLOT(on_Download_Right_triggered()));
    connect(Verify, SIGNAL(leftButtonClicked()),this,SLOT(on_Verify_Left_triggered()));
    connect(Verify, SIGNAL(rightButtonClicked()),this,SLOT(on_Verify_Right_triggered()));
    connect(Run, SIGNAL(leftButtonClicked()),this,SLOT(on_Run_Left_triggered()));
    connect(Run, SIGNAL(rightButtonClicked()),this,SLOT(on_Run_Right_triggered()));
    connect(Stop, SIGNAL(leftButtonClicked()),this,SLOT(on_Stop_Left_triggered()));
    connect(Stop, SIGNAL(rightButtonClicked()),this,SLOT(on_Stop_Right_triggered()));
    connect(Inst, SIGNAL(leftButtonClicked()),this,SLOT(on_Inst_Left_triggered()));
    connect(Inst, SIGNAL(rightButtonClicked()),this,SLOT(on_Inst_Right_triggered()));
    connect(Step, SIGNAL(leftButtonClicked()),this,SLOT(on_Step_Left_triggered()));
    connect(Step, SIGNAL(rightButtonClicked()),this,SLOT(on_Step_Right_triggered()));
    connect(Continue, SIGNAL(leftButtonClicked()),this,SLOT(on_Continue_Left_triggered()));
    connect(Continue, SIGNAL(rightButtonClicked()),this,SLOT(on_Continue_Right_triggered()));
    connect(AddBp, SIGNAL(leftButtonClicked()),this,SLOT(on_AddBp_Left_triggered()));
    connect(AddBp, SIGNAL(rightButtonClicked()),this,SLOT(on_AddBp_Right_triggered()));
    connect(Quit, SIGNAL(leftButtonClicked()),this,SLOT(on_Quit_Left_triggered()));
    connect(Quit, SIGNAL(rightButtonClicked()),this,SLOT(on_Quit_Right_triggered()));
    connect(Clear, SIGNAL(leftButtonClicked()),this,SLOT(on_Clear_triggered()));
    connect(Send, &QPushButton::clicked,this,[=]{on_Send_triggered();});
    connect(ToolPathOpen, &QPushButton::clicked,this,[=]{on_DebugPathOpen_triggered();});
    connect(InitFilePathOpen, &QPushButton::clicked,this,[=]{on_InitPathOpen_triggered();});
    connect(BaudRate, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](){on_ToolShow_refresh();});
    connect(DsuCom, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](){on_ToolShow_refresh();});
    connect(ToolVariable, &QLineEdit::textChanged, [=](){on_ToolShow_refresh();});
    connect(InitFilePath, &QLineEdit::textChanged, [=](){on_ToolShow_refresh();});
    connect(DownLoadCurrent, SIGNAL(toggled(bool)), this, SLOT(onLoadRadioButtonToggled(bool)));
    connect(VerifyCurrent, SIGNAL(toggled(bool)), this, SLOT(onVerifyRadioButtonToggled(bool)));
    connect(CmdLine->lineEdit(), &QLineEdit::returnPressed, [=](){on_Send_triggered();});

};

void tools::on_SerialPort_triggered(){

    if(SerialPort->isChecked()){
        Terminal->setVisible(true);
        SerialPort->setChecked(true);
        ShowTerminal->setChecked(true);
        StatusBar->DetermineMessage("Display Serial Port");
    }
    else{
        Terminal->setVisible(false);
        SerialPort->setChecked(false);
        ShowTerminal->setChecked(false);
        StatusBar->DetermineMessage("Don't Dispaly Serial Port");
    }

}

void tools::on_CmdDebug_triggered(){

    if(CmdDebug->isChecked()){
        Debug->setVisible(true);
        CmdDebug->setChecked(true);
        ShowDebug->setChecked(true);
        StatusBar->DetermineMessage("Display CMD Debug");
    }
    else{
        Debug->setVisible(false);
        CmdDebug->setChecked(false);
        ShowDebug->setChecked(false);
        StatusBar->DetermineMessage("Don't Dispaly CMD Debug");
    }

}

void tools::on_RegCalculator_triggered(){

    editTab->show();
    CentralWidget->setStyleSheet("");
    if(RegCalculator->isChecked()){
        RegCalculator->setChecked(true);
        RegCalc->setVisible(true);
        ShowReg->setChecked(true);
        StatusBar->DetermineMessage("Display Reg Calculator");
    }
    else{
        RegCalculator->setChecked(false);
        RegCalc->setVisible(false);
        ShowReg->setChecked(false);
        StatusBar->DetermineMessage("Don't Dispaly Reg Calculator");
    }

}

void tools::onLoadRadioButtonToggled(bool checked){

    if(DownLoadCurrent->isChecked()) {

        QStringList LoadFileLists = FileMenu->SettingFile->readSdkFile(FileMenu->SdkFilePath,"OutputFile");
        if(!LoadFileLists.isEmpty()){
            for (const QString &item : LoadFileLists) {
                QFileInfo LoadFile = item;
                if(LoadFile.exists() && (LoadFile.suffix()=="exe") || (LoadFile.suffix()=="elf")){
                    DownLoadPath->setText(item);
                    break;
                }else{
                    DownLoadPath->setText("Dont find load file!");
                }
            }
        }else{
            DownLoadPath->setText("Dont find load file!");
        }

    } else{

        QString fileName = QFileDialog::getOpenFileName(this, "Select Load File",QCoreApplication::applicationDirPath(),
                                                      "Load File(*.exe *.elf *.EXE *.ELF)");
        if (!fileName.isEmpty()){
            DownLoadPath->setText(fileName);
        }else{
            DownLoadPath->setText("Dont find load file!");
        }

    }
}

void tools::onVerifyRadioButtonToggled(bool checked){

    if(VerifyCurrent->isChecked()) {

        QStringList VerifyFileLists = FileMenu->SettingFile->readSdkFile(FileMenu->SdkFilePath,"OutputFile");
        if(!VerifyFileLists.isEmpty()){
            for (const QString &item : VerifyFileLists) {
                QFileInfo LoadFile = item;
                if(LoadFile.exists() && (LoadFile.suffix()=="exe") || (LoadFile.suffix()=="elf")){
                    VerifyPath->setText(item);
                    break;
                }else{
                    VerifyPath->setText("Dont find Verify file!");
                }
            }
        }else{
            VerifyPath->setText("Dont find Verify file!");
        }

    } else{

        QString fileName = QFileDialog::getOpenFileName(this, "Select Verify File",QCoreApplication::applicationDirPath(),
                                                      "Verify File(*.exe *.elf *.EXE *.ELF)");
        if (!fileName.isEmpty()){
            VerifyPath->setText(fileName);
        }else{
            VerifyPath->setText("Dont find Verify file!");
        }

    }
}

void tools::on_ToolShow_refresh(){

    QString ToolShowText = "-baud "+BaudRate->currentText()+" -uart "+DsuCom->currentText();
    if(ToolVariable->text()!=NULL){
        ToolShowText += " "+ToolVariable->text();
    }
    ToolShowString = ToolShowText;
    QFileInfo InitFile = InitFilePath->text();
    if(InitFile.exists()){
        ToolShowText += " -c "+InitFile.fileName()+" -echo";
        ToolShowString += " -c "+InitFilePath->text()+" -echo";
    }

    ToolShow->setText(ToolShowText);

}

void tools::on_DebugPathOpen_triggered(){

    QFileInfo dir(ToolPath->text());
    QString InitPath;
    if (dir.isFile()) {
        InitPath = dir.path();
    } else {
        InitPath = QCoreApplication::applicationDirPath();
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Select Debug Tool",InitPath,
                                                    "exe(*.exe);;ALL File(*.*)");
    if (!fileName.isEmpty()){
        ToolPath->setText(fileName);
    }

}

void tools::on_InitPathOpen_triggered(){

    QFileInfo dir(InitFilePath->text());
    QString InitPath;
    if (dir.isFile()) {
        InitPath = dir.path();
    } else {
        InitPath = QCoreApplication::applicationDirPath();
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Select Init File",InitPath,
                                                  "tcl(*.tcl);;ALL File(*.*)");
    if (!fileName.isEmpty()){
        InitFilePath->setText(fileName);
    }

}

void tools::on_GenerateFlashFile_triggered(){

    GenerateFlashFileWidget->setWindowModality(Qt::ApplicationModal);
    GenerateFlashFileWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    GenerateFlashFileWidget->show();

}

void tools::on_BootFileSelect_triggered(){

    QFileInfo dir(BootFilePath->text());
    QString InitPath;
    if (dir.isFile()) {
        InitPath = dir.path();
    } else {
        InitPath = QCoreApplication::applicationDirPath();
    }

    QString fileName = QFileDialog::getOpenFileName(GenerateFlashFileWidget, "Open Boot File",InitPath,
                                                    "bin File(*.bin *.BIN);;ALL File(*.*)");

    if (!fileName.isEmpty()){

        BootFilePath->setText(fileName);

    }

}

void tools::on_SourceFileSelect_triggered(){

    QFileInfo dir(SourceFilePath->text());
    QString InitPath;
    if (dir.isFile()) {
        InitPath = dir.path();
    } else {
        InitPath = QCoreApplication::applicationDirPath();
    }

    QString fileName = QFileDialog::getOpenFileName(GenerateFlashFileWidget, "Open Source File",InitPath,
                                                    "bin File(*.bin *.BIN);;ALL File(*.*)");

    if (!fileName.isEmpty()){

        SourceFilePath->setText(fileName);

    }

}

void tools::on_TargetFileSelect_triggered(){

    QFileInfo dir(TargetFilePath->text());
    QString InitPath;
    if (dir.isFile()) {
        InitPath = dir.path();
    } else {
        InitPath = QCoreApplication::applicationDirPath();
    }

    QString file_path = QFileDialog::getSaveFileName(GenerateFlashFileWidget, "Save Array File",InitPath,
                                                     "c File(*.c *.C);;h File(*.h *.H);;ALL File(*.*)");
    if (!file_path.isEmpty()){

        TargetFilePath->setText(file_path);

    }

}

void tools::on_Generate_triggered(){

    edit->clear();
    QString file_path = TargetFilePath->text();
    if (!file_path.isEmpty()){

        QFileInfo TargetFile_info(file_path);
        QFileInfo BootFile_info(BootFilePath->text());
        QFileInfo SourceFile_info(SourceFilePath->text());
        QProcess CopyProcess;
        CopyProcess.setWorkingDirectory(QCoreApplication::applicationDirPath());
        QString Command = "cmd.exe";
        QStringList Arguments;
        QString arg;

        if (BootFile_info.exists() && SourceFile_info.exists()){

            arg = "copy /b "+BootFile_info.filePath().replace('/', '\\')+"+"+ SourceFile_info.filePath().replace('/', '\\')+" "+TargetFile_info.path().replace('/', '\\')+"\\"+TargetFile_info.baseName()+"_temp.bin";
            Arguments << "/c" << arg;
            CopyProcess.start("cmd",Arguments);
            CopyProcess.waitForStarted();
            CopyProcess.waitForFinished();
            QString strTemp=QString::fromLocal8Bit(CopyProcess.readAllStandardOutput());
            CopyProcess.close();
            convertBinaryFile(TargetFile_info.path().replace('/', '\\')+"\\"+TargetFile_info.baseName()+"_temp.bin",TargetFile_info.filePath(),edit);
            StatusBar->DetermineMessage("Generate BootBin+SourceBin Array Success");

        }else if(BootFile_info.exists() && !SourceFile_info.exists()){

            convertBinaryFile(BootFile_info.filePath().replace('/', '\\'),TargetFile_info.filePath(),edit);
            StatusBar->DetermineMessage("Generate BootBin Array Success");

        }else if(!BootFile_info.exists() && SourceFile_info.exists()){

            convertBinaryFile(SourceFile_info.filePath().replace('/', '\\'),TargetFile_info.filePath(),edit);
            StatusBar->DetermineMessage("Generate SourceBin Array Success");

        }else{

            StatusBar->DetermineMessage("No Transform");

        }

    }

}

void tools::convertBinaryFile(const QString& binFilePath, const QString& cArrayFilePath,QsciScintilla* edit)
{
        QFile inputFile(binFilePath);
        if (!inputFile.open(QIODevice::ReadOnly)){
            StatusBar->DetermineMessage("Can't Open Bin File");
            return;
        }

        QFile outputFile(cArrayFilePath);
        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            StatusBar->DetermineMessage("Can't Open Array File");
            return;
        }

        // Write header with file information
        QString header = QString("// Converted from: %1\n// Conversion time: %2\n\nunsigned int buffer_len = %3; //bytes\n\n")
                             .arg(binFilePath)
                             .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(inputFile.size());
        outputFile.write(header.toUtf8());
        edit->append(header.toUtf8());

        QByteArray byteArray = inputFile.readAll();
        int arraySize = byteArray.size() / sizeof(int);

        QVector<unsigned int> intArray;

        for(int k=0;k<byteArray.size();k=k+4){

            unsigned int objId = (byteArray[k+0] << 24) & 0xFF000000;
            objId |= (byteArray[k+1] << 16) & 0x00FF0000;
            objId |= (byteArray[k+2] << 8) & 0x0000FF00;
            objId |= (byteArray[k+3] << 0) & 0x000000FF;
            intArray.append(objId);

        }

        // Write array data
        outputFile.write("unsigned int data_input[] = {");
        edit->append("unsigned int data_input[] = {");
        for (int i = 0; i < arraySize; i=i+4){

            // Write address comment
            QString addressComment = QString("\n/*0x%1*/    ").arg(i * sizeof(int), 8, 16, QLatin1Char('0'));
            outputFile.write(addressComment.toUtf8());
            edit->append(addressComment.toUtf8());
            for (int j = 0; j < 4; j++){

                if (i + j < arraySize){

                    QString dataLine = QString("0x%1").arg(intArray.at(i+j), 8, 16, QLatin1Char('0'));
                    outputFile.write(dataLine.toUtf8());
                    edit->append(dataLine.toUtf8());
                    if (j != 3 || i + 4 < arraySize){
                        outputFile.write(", ");
                        edit->append(", ");
                    }

                }
            }

        }
        outputFile.write("\n};");
        edit->append("\n};");

        inputFile.close();
        outputFile.close();

}

void tools::on_Preference_triggered(){

    ToolPreference->ReadIni(FileMenu->SdkFilePath);
    ToolPreference->setWindowModality(Qt::ApplicationModal);
    ToolPreference->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    ToolPreference->show();

}

void tools::on_Link_Left_triggered(){

    this->DebugMessage->append(">>SDK Ttrying to Link Target...\r\n>>");
    QTextCursor cursor = DebugMessage->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->DebugMessage->setTextCursor(cursor);

    if(Process!=NULL && Process->state() == QProcess::Running) {

        QString quit = "quit\r\n";
        const char * CmdChar= quit.toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
        Process->kill();
        Process->waitForFinished();
        Process->deleteLater();
        Process = 0;

    }

    Process=new QProcess();
    Process->setWorkingDirectory(QCoreApplication::applicationDirPath());
    CmdLine->Process = Process;
    Process->setProcessChannelMode(QProcess::MergedChannels);
    connect(Process,SIGNAL(readyReadStandardOutput()),this,SLOT(exec()));

    QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
    QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);
    GlobalSetting.beginGroup("LinkSetting");
    QString DebugToolPath  = GlobalSetting.value("DebugToolPath").toString();
    if(DebugToolPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        DebugToolPath = DebugToolPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
    }
    GlobalSetting.endGroup();

    if(DebugToolPath!=""){
        on_ToolShow_refresh();
        Process->start(ToolPath->text()+" "+ToolShowString);
        Process->waitForStarted();
    }else{
        StatusBar->DetermineMessage("Debug Tool Path Not Exist");
    }

}

void tools::on_Link_Right_triggered(){

    LinkWidget->setWindowModality(Qt::ApplicationModal);
    LinkWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    LinkWidget->show();

}

void tools::on_Download_Left_triggered(){

    if(DownLoadCurrent->isChecked()){
        QStringList LoadFileLists = FileMenu->SettingFile->readSdkFile(FileMenu->SdkFilePath,"OutputFile");
        if(!LoadFileLists.isEmpty()){
            for (const QString &item : LoadFileLists) {
                QFileInfo LoadFile = item;
                if(LoadFile.exists() && (LoadFile.suffix()=="exe")||(LoadFile.suffix()=="elf")){
                    DownLoadPath->setText(item);
                    break;
                }else{
                    DownLoadPath->setText("Dont find load file!");
                }
            }
        }else{
            DownLoadPath->setText("Dont find load file!");
        }
    }
    if(DownLoadPath->text().contains(".exe")||DownLoadPath->text().contains(".elf")){
        if(Process!=NULL && Process->state() == QProcess::Running) {
            const char * CmdChar= (DownLoadCmd->text()+" "+DownLoadPath->text()+"\r\n").toUtf8().data();
            Process->write(CmdChar);
            Process->waitForBytesWritten();
        }
    }else{
        QTextCursor cursor = DebugMessage->textCursor();
        cursor.movePosition(QTextCursor::End);
        DebugMessage->setTextCursor(cursor);
        DebugMessage->insertPlainText(DownLoadPath->text()+"\r\n"+">>");
        cursor.movePosition(QTextCursor::End);
        DebugMessage->setTextCursor(cursor);
    }

}

void tools::on_Download_Right_triggered(){

    if(DownLoadCurrent->isChecked()){
        QStringList LoadFileLists = FileMenu->SettingFile->readSdkFile(FileMenu->SdkFilePath,"OutputFile");
        if(!LoadFileLists.isEmpty()){
            for (const QString &item : LoadFileLists) {
                QFileInfo LoadFile = item;
                if(LoadFile.exists() && (LoadFile.suffix()=="exe")||(LoadFile.suffix()=="elf")){
                    DownLoadPath->setText(item);
                    break;
                }else{
                    DownLoadPath->setText("Dont find load file!");
                }
            }
        }else{
            DownLoadPath->setText("Dont find load file!");
        }
    }
    DownLoadWidget->setWindowModality(Qt::ApplicationModal);
    DownLoadWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    DownLoadWidget->show();

}

void tools::on_Verify_Left_triggered(){

    if(VerifyCurrent->isChecked()){
        QStringList VerifyFileLists = FileMenu->SettingFile->readSdkFile(FileMenu->SdkFilePath,"OutputFile");
        if(!VerifyFileLists.isEmpty()){
            for (const QString &item : VerifyFileLists) {
                QFileInfo LoadFile = item;
                if(LoadFile.exists() && (LoadFile.suffix()=="exe") || (LoadFile.suffix()=="elf")){
                    VerifyPath->setText(item);
                    break;
                }else{
                    VerifyPath->setText("Dont find Verify file!");
                }
            }
        }else{
            VerifyPath->setText("Dont find Verify file!");
        }
    }

    if(VerifyPath->text().contains(".exe")||VerifyPath->text().contains(".elf")){
        if(Process!=NULL && Process->state() == QProcess::Running) {
            const char * CmdChar= (VerifyCmd->text()+" "+VerifyPath->text()+"\r\n").toUtf8().data();
            Process->write(CmdChar);
            Process->waitForBytesWritten();
        }
    }else{
        QTextCursor cursor = DebugMessage->textCursor();
        cursor.movePosition(QTextCursor::End);
        DebugMessage->setTextCursor(cursor);
        DebugMessage->insertPlainText(VerifyPath->text()+"\r\n"+">>");
        cursor.movePosition(QTextCursor::End);
        DebugMessage->setTextCursor(cursor);
    }

}

void tools::on_Verify_Right_triggered(){

    if(VerifyCurrent->isChecked()) {

        QStringList VerifyFileLists = FileMenu->SettingFile->readSdkFile(FileMenu->SdkFilePath,"OutputFile");
        if(!VerifyFileLists.isEmpty()){
            for (const QString &item : VerifyFileLists) {
                QFileInfo LoadFile = item;
                if(LoadFile.exists() && (LoadFile.suffix()=="exe") || (LoadFile.suffix()=="elf")){
                    VerifyPath->setText(item);
                    break;
                }else{
                    VerifyPath->setText("Dont find Verify file!");
                }
            }
        }else{
            VerifyPath->setText("Dont find Verify file!");
        }

    }
    VerifyWidget->setWindowModality(Qt::ApplicationModal);
    VerifyWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    VerifyWidget->show();

}

void tools::on_Run_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        const char * CmdChar= (RunCmd->text()+"\r\n").toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
    }

}

void tools::on_Run_Right_triggered(){

    RunWidget->setWindowModality(Qt::ApplicationModal);
    RunWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    RunWidget->show();

}

void tools::on_Stop_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        qint64 PID =  Process->processId();
        AttachConsole(PID);
        SetConsoleCtrlHandler(NULL, TRUE);
        GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);

        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
        if (INVALID_HANDLE_VALUE == hProcess)
        {
            return ;
        }
        WaitForSingleObject(hProcess, INFINITE);
        CloseHandle(hProcess);
        Sleep(20);

        FreeConsole();
        SetConsoleCtrlHandler(NULL, FALSE);
    }

}

void tools::on_Stop_Right_triggered(){

    StopWidget->setWindowModality(Qt::ApplicationModal);
    StopWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    StopWidget->show();

}

void tools::on_Inst_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        const char * CmdChar= (InstCmd->text()+"\r\n").toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
    }

}

void tools::on_Inst_Right_triggered(){

    InstWidget->setWindowModality(Qt::ApplicationModal);
    InstWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    InstWidget->show();

}

void tools::on_Step_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        const char * CmdChar= (StepCmd->text()+"\r\n").toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
    }

}

void tools::on_Step_Right_triggered(){

    StepWidget->setWindowModality(Qt::ApplicationModal);
    StepWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    StepWidget->show();

}

void tools::on_Continue_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        const char * CmdChar= (ContinueCmd->text()+"\r\n").toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
    }

}

void tools::on_Continue_Right_triggered(){

    ContinueWidget->setWindowModality(Qt::ApplicationModal);
    ContinueWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    ContinueWidget->show();

}

void tools::on_AddBp_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running){

        QFileInfo SdkFileInfo(FileMenu->SdkFilePath);
        QString GlobalConfig = SdkFileInfo.path()+"/.Configure/Config.ini";

        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);
        int size = GlobalSetting.beginReadArray("BreakInfo");
        for (int i = 0; i < size; i++) {

            GlobalSetting.setArrayIndex(i);
            QString BreakAddr = GlobalSetting.value("BreakAddr").toString();
            QString BreakPointEnable = GlobalSetting.value("BreakPointEnable").toString();

            if (BreakPointEnable == "Enable") {

                QRegularExpression re("^[\\da-fA-F]{1,8}");//("\\b[0-9A-Fa-f]{8}\\b");
                QRegularExpressionMatch match = re.match(BreakAddr);
                if(match.hasMatch() && match.captured(0) == BreakAddr) {
                    const char * CmdChar= (BreakCmd->text()+" 0x"+BreakAddr+"\r\n").toUtf8().data();
                    Process->write(CmdChar);
                    Process->waitForBytesWritten();;
                } else {
                    DebugMessage->append(">>Number "+QString::number(i+1)+" Break Point Address Illegal\r\n>>");
                }

            }

        }
        GlobalSetting.endArray();

    }

}

void tools::on_AddBp_Right_triggered(){

    BreakSettingTable->setRowCount(0);

    QFileInfo SdkFileInfo(FileMenu->SdkFilePath);
    QString GlobalConfig = SdkFileInfo.path()+"/.Configure/Config.ini";

    QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);
    int size = GlobalSetting.beginReadArray("BreakInfo");
    for (int i = 0; i < size; i++) {

        GlobalSetting.setArrayIndex(i);
        QString BreakPointFile = GlobalSetting.value("BreakPointFile").toString();
        QString BreakPointLine = GlobalSetting.value("BreakPointLine").toString();
        QString BreakAddr = GlobalSetting.value("BreakAddr").toString();
        QString BreakPointEnable = GlobalSetting.value("BreakPointEnable").toString();
        QFileInfo FileInfo(BreakPointFile);
        QString FileName = FileInfo.fileName();
        int CurrentrowMax = BreakSettingTable->rowCount();
        bool addRow = true;

        for(int j = 0;j<CurrentrowMax;j++){

            QString str1 = BreakSettingTable->item(j,2)->text();
            if(FileName == str1.split(":")[0] && BreakPointLine == str1.split(":")[1]){
                addRow = false;
                break;
            }

        }

        if(addRow){

            int rowMax = BreakSettingTable->rowCount();
            BreakSettingTable->insertRow(rowMax);
            BreakSettingTable->setItem(rowMax,0,new QTableWidgetItem("BreakPoint"));
            BreakSettingTable->setItem(rowMax,1,new QTableWidgetItem(BreakAddr));
            BreakSettingTable->setItem(rowMax,2,new QTableWidgetItem(FileName+":"+BreakPointLine));
            QPushButton * Jump = new QPushButton("Jump");
            Jump->setWhatsThis(BreakPointFile+">>"+BreakPointLine);
            connect(Jump, &QPushButton::clicked,this,[=]{

                QFileInfo *FileInfo = new QFileInfo(Jump->whatsThis().split(">>")[0]);
                if(FileInfo->isFile()){
                    ProjectTree->loadFile(Jump->whatsThis().split(">>")[0]);
                }
                for(int j=0;j<ProjectTree->editlist->size();j++){
                    if(Jump->whatsThis().split(">>")[0]==ProjectTree->editlist->at(j)->whatsThis()){

                        int Line = Jump->whatsThis().split(">>")[1].toInt()-1;
                        ProjectTree->editlist->at(j)->setCursorPosition(Line,0);
                        ProjectTree->editlist->at(j)->setSelection(Line,0,Line,ProjectTree->editlist->at(j)->text(Line).size()-1);

                    }
                }

            });
            BreakSettingTable->setCellWidget(rowMax,3,Jump);

            QPushButton * Enb = new QPushButton();
            if(BreakPointEnable=="Enable"){
                Enb->setText("Enable");
            }else{
                Enb->setText("Disable");
            }
            Enb->setWhatsThis(BreakPointFile+">>"+BreakPointLine);
            connect(Enb, &QPushButton::clicked,this,[=]{

                if (Enb->text()=="Enable"){
                    Enb->setText("Disable");
                }else{
                    Enb->setText("Enable");
                }

                QFileInfo SdkFileInfo(FileMenu->SdkFilePath);
                QString GlobalConfig = SdkFileInfo.path()+"/.Configure/Config.ini";
                QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

                QStringList *LineList = new QStringList();
                QStringList *FileList = new QStringList();
                QStringList *AddrList = new QStringList();
                QStringList *EnableList = new QStringList();
                int length = GlobalSetting.beginReadArray("BreakInfo");
                for (int k = 0; k < length; k++) {
                    GlobalSetting.setArrayIndex(k);
                    if((GlobalSetting.value("BreakPointLine")==Enb->whatsThis().split(">>")[1].toInt()) && (GlobalSetting.value("BreakPointFile")==Enb->whatsThis().split(">>")[0])){
                        EnableList->append(Enb->text());
                    }else{
                        EnableList->append(GlobalSetting.value("BreakPointEnable").toString());
                    }
                    LineList->append(GlobalSetting.value("BreakPointLine").toString());
                    FileList->append(GlobalSetting.value("BreakPointFile").toString());
                    AddrList->append(GlobalSetting.value("BreakAddr").toString());
                }
                GlobalSetting.endArray();

                if(LineList->size()!=0){
                    GlobalSetting.beginWriteArray("BreakInfo");
                    for (int m = 0; m < LineList->size(); m++) {
                        GlobalSetting.setArrayIndex(m);
                        GlobalSetting.setValue("BreakPointLine",LineList->at(m));
                        GlobalSetting.setValue("BreakPointFile",FileList->at(m));
                        GlobalSetting.setValue("BreakAddr",AddrList->at(m));
                        GlobalSetting.setValue("BreakPointEnable",EnableList->at(m));
                    }
                    GlobalSetting.sync();
                    GlobalSetting.endArray();
                }

            });
            BreakSettingTable->setCellWidget(rowMax,4,Enb);

            QPushButton * Deleate = new QPushButton("Deleate");
            Deleate->setWhatsThis(BreakPointFile+">>"+BreakPointLine);
            connect(Deleate, &QPushButton::clicked,this,[=]{

                //get Line
                QFileInfo FileInfoDel(Deleate->whatsThis().split(">>")[0]);
                QString FileNameDel = FileInfo.fileName();
                int Line = 0;
                int Currentrow = BreakSettingTable->rowCount();
                for(int k = 0;k<Currentrow;k++){

                    if(BreakSettingTable->item(k,2)->text() == (FileNameDel+":"+Deleate->whatsThis().split(">>")[1])){
                        Line = Deleate->whatsThis().split(">>")[1].toInt()-1;
                        break;
                    }

                }

                //deleate mark
                for(int j=0;j<ProjectTree->editlist->size();j++){

                    if(Deleate->whatsThis().split(">>")[0]==ProjectTree->editlist->at(j)->whatsThis()){

                        int maskn=ProjectTree->editlist->at(j)->SendScintilla(QsciScintilla::SCI_MARKERGET,Line);
                        if(maskn != 0){

                            ProjectTree->editlist->at(j)->SendScintilla(QsciScintilla::SCI_MARKERDELETE,Line,1);
                            break;

                        }

                    }

                }

                //deleate setting file
                QFileInfo SdkFileInfo(FileMenu->SdkFilePath);
                QString GlobalConfig = SdkFileInfo.path()+"/.Configure/Config.ini";
                QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

                int size = GlobalSetting.beginReadArray("BreakInfo");
                QStringList *LineList = new QStringList();
                QStringList *FileList = new QStringList();
                QStringList *AddrList = new QStringList();
                QStringList *EnableList = new QStringList();
                for (int i = 0; i < size; i++) {
                    GlobalSetting.setArrayIndex(i);
                    if(GlobalSetting.value("BreakPointLine")!=(Line+1) || GlobalSetting.value("BreakPointFile")!=Deleate->whatsThis().split(">>")[0]){
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

                //deleate table item
                for(int k = 0;k<Currentrow;k++){
                    if(BreakSettingTable->item(k,2)->text() == (FileNameDel+":"+Deleate->whatsThis().split(">>")[1])){
                        BreakSettingTable->removeRow(k);
                        break;
                    }
                }

            });
            BreakSettingTable->setCellWidget(rowMax,5,Deleate);

        }

    }
    GlobalSetting.endArray();

    BreakWidget->setWindowModality(Qt::ApplicationModal);
    BreakWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    BreakWidget->show();

}

void tools::on_Quit_Left_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {
        const char * CmdChar= (QuitCmd->text()+"\r\n").toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();
        Process->close();
    }

}

void tools::on_Quit_Right_triggered(){

    QuitWidget->setWindowModality(Qt::ApplicationModal);
    QuitWidget->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    QuitWidget->show();

}

void tools::on_Clear_triggered(){

    DebugMessage->clear();
}

void tools::on_Send_triggered(){

    if(Process!=NULL && Process->state() == QProcess::Running) {

        const char * CmdChar= (CmdLine->currentText()+"\r\n").toUtf8().data();
        Process->write(CmdChar);
        Process->waitForBytesWritten();

        QString text = CmdLine->currentText();
        if(text!=""){
            int index = CmdLine->findText(text);
            if (index != -1) {
                index = index + 1;
                if (index >= CmdLine->count()) {
                    index = 0;
                }
                CmdLine->setCurrentIndex(index);
            } else {
                CmdLine->addItem(text);
                CmdLine->setCurrentIndex(0);
            }
        }
        CmdLine->clearEditText();

    }

}

void tools::exec(){

    QString strTemp=QString::fromLocal8Bit(Process->readAllStandardOutput());
    QRegularExpression re("\r{2,}");
    strTemp.replace(re, "\r");
    int index = strTemp.indexOf("\r\n");
    while (index != -1) {
        strTemp.insert(index + 2, ">>");
        index = strTemp.indexOf("\r\n", index + 3);
    }
    QRegularExpression gr("grmon", QRegularExpression::CaseInsensitiveOption);
    strTemp.replace(gr, "bmmon");
    QRegularExpression fr("Frontgrade", QRegularExpression::CaseInsensitiveOption);
    strTemp.replace(fr, "Casc");
    QRegularExpression ga("Gaisler", QRegularExpression::CaseInsensitiveOption);
    strTemp.replace(ga, "Bmti");
    QRegularExpression le("LEON5", QRegularExpression::CaseInsensitiveOption);
    strTemp.replace(le, "BmSoc");

    //move cursor to text finish
    QTextCursor cursor = DebugMessage->textCursor();
    cursor.movePosition(QTextCursor::End);
    DebugMessage->setTextCursor(cursor);
    DebugMessage->insertPlainText(strTemp);
    cursor.movePosition(QTextCursor::End);
    DebugMessage->setTextCursor(cursor);

}

void tools::InitWidget(){

    //Generate Flash File
    GenerateFlashFileWidget = new mywidget("GenerateFlash",this,this);
    GenerateFlashFileWidget->setWindowTitle("Generate Flash File");
    GenerateFlashFileWidget->setWindowIcon(QIcon(":/new/prefix1/res/flashFile"));
    GenerateFlashFileWidget->setWindowFlags(Qt::Dialog);
    GenerateFlashFileWidget->setWindowModality(Qt::WindowModal);
    GenerateFlashFileLayout = new QVBoxLayout();

    BootFileGroup = new QGroupBox("Boot File(*.bin)");
    BootFileLayout = new QHBoxLayout();
    BootFilePath = new QLineEdit();
    BootFilePath->setMinimumSize(480,20);
    BootFileSelect = new QPushButton("Load");
    BootFileLayout->addWidget(BootFilePath);
    BootFileLayout->addWidget(BootFileSelect);
    BootFileGroup->setLayout(BootFileLayout);

    SourceFileGroup = new QGroupBox("Source File(*.bin)");
    SourceFileLayout = new QHBoxLayout();
    SourceFileGroup->setLayout(SourceFileLayout);
    SourceFilePath = new QLineEdit();
    SourceFileSelect = new QPushButton("Load");
    SourceFileLayout->addWidget(SourceFilePath);
    SourceFileLayout->addWidget(SourceFileSelect);

    TargetFileGroup = new QGroupBox("Target File(*.c/*.h)");
    TargetFileLayout = new QHBoxLayout();
    TargetFileGroup->setLayout(TargetFileLayout);
    TargetFilePath = new QLineEdit();
    TargetFileSelect = new QPushButton("Select");
    TargetFileLayout->addWidget(TargetFilePath);
    TargetFileLayout->addWidget(TargetFileSelect);

    Generate = new QPushButton("Generate");
    GenerateLayout = new QHBoxLayout();
    GenerateSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    GenerateSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    GenerateLayout->addItem(GenerateSpacer1);
    GenerateLayout->addWidget(Generate);
    GenerateLayout->addItem(GenerateSpacer2);
    GenerateLayout->setMargin(0);
    GenerateWidget = new QWidget();
    GenerateWidget->setLayout(GenerateLayout);

    TextWidgetLayout = new QHBoxLayout();
    edit = new QsciScintilla();
    QsciLexerCPP *textLexer = new QsciLexerCPP();
    edit->setLexer(textLexer);
    edit->setMarginType(0,QsciScintilla::NumberMargin);
    edit->setMarginWidth(0,40);
    edit->setMarginLineNumbers(0,true);
    edit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
    edit->setAutoIndent(true);
    edit->setIndentationGuides(QsciScintilla::SC_IV_LOOKBOTH);
    edit->setCaretLineVisible(true);
    edit->setCaretLineBackgroundColor(Qt::lightGray);
    edit->setMarginsBackgroundColor(Qt::gray);
    edit->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    TextWidgetLayout->addWidget(edit);
    TextWidget = new QWidget();
    TextWidget->setLayout(TextWidgetLayout);

    GenerateFlashFileLayout->addWidget(BootFileGroup);
    GenerateFlashFileLayout->addWidget(SourceFileGroup);
    GenerateFlashFileLayout->addWidget(TargetFileGroup);
    GenerateFlashFileLayout->addWidget(GenerateWidget);
    GenerateFlashFileLayout->addWidget(TextWidget);

    GenerateFlashFileWidget->setLayout(GenerateFlashFileLayout);

    //new tool preference
    ToolPreference = new tool_preference(nullptr,FileMenu,Edit,StatusBar,this->MainWindow);

    //Link BUtton Widget
    LinkWidget = new mywidget("LinkWidget",this,this);
    LinkWidget->setWindowIcon(QIcon(":/new/prefix1/res/linkTarget"));
    LinkWidget->setWindowTitle("Link Setting");
    LinkWidget ->setWindowFlags(Qt::Dialog);
    LinkWidget->setWindowModality(Qt::WindowModal);
    LinkWidgetLayout = new QVBoxLayout();
    DsuSetting = new QGroupBox("DSU Setting");
    LayoutDsuSetting = new QGridLayout();
    BaudRateLabel = new QLabel    ("BaudRate:    ");
    BaudRate = new QComboBox();
    BaudRate->addItem("9600");
    BaudRate->addItem("19200");
    BaudRate->addItem("38400");
    BaudRate->addItem("115200");
    BaudRate->addItem("460800");
    BaudRate->setCurrentText("115200");
    BaudRate->setMinimumSize(320,20);
    BaudRate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutDsuSetting->addWidget(BaudRateLabel,0,0);
    LayoutDsuSetting->addWidget(BaudRate,0,1);
    DsuComLabel= new QLabel("DsuCom:      ");
    DsuCom = new QComboBox();
    DsuCom->addItem("COM1");
    DsuCom->addItem("COM2");
    DsuCom->addItem("COM3");
    DsuCom->addItem("COM4");
    DsuCom->addItem("COM5");
    DsuCom->addItem("COM6");
    DsuCom->addItem("COM7");
    DsuCom->addItem("COM8");
    DsuCom->addItem("COM9");
    DsuCom->setMinimumSize(320,20);
    DsuCom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutDsuSetting->addWidget(DsuComLabel,1,0);
    LayoutDsuSetting->addWidget(DsuCom,1,1);
    DsuSetting->setLayout(LayoutDsuSetting);
    ToolSetting = new QGroupBox("Debug Setting");
    LayoutToolSetting = new QGridLayout();
    ToolPathLabel = new QLabel    ("Debug Tool:  ");
    ToolPath = new QLineEdit();
    ToolPathOpen = new QPushButton("Select");
    InitFilePathLabel = new QLabel("Init File:   ");
    InitFilePath = new QLineEdit();
    InitFilePathOpen = new QPushButton("Select");

    LayoutToolSetting->addWidget(ToolPathLabel,0,0);
    LayoutToolSetting->addWidget(ToolPath,0,1);
    LayoutToolSetting->addWidget(ToolPathOpen,0,2);
    LayoutToolSetting->addWidget(InitFilePathLabel,1,0);
    LayoutToolSetting->addWidget(InitFilePath,1,1);
    LayoutToolSetting->addWidget(InitFilePathOpen,1,2);

    ToolSetting->setLayout(LayoutToolSetting);
    ParaSetting = new QGroupBox("Para Setting");
    LayoutParaSetting = new QGridLayout();
    ToolVariableLabel = new QLabel("Other Param: ");
    ToolVariable = new QLineEdit();
    ToolVariable->setText("-ni -echo");
    ToolShowLabel = new QLabel    ("Link CMD:    ");
    ToolShow = new QLineEdit();
    ToolShow->setReadOnly(true);
    LayoutParaSetting->addWidget(ToolVariableLabel,0,0);
    LayoutParaSetting->addWidget(ToolVariable,0,1);
    LayoutParaSetting->addWidget(ToolShowLabel,1,0);
    LayoutParaSetting->addWidget(ToolShow,1,1);
    ParaSetting->setLayout(LayoutParaSetting);
    LinkWidgetLayout->addWidget(DsuSetting);
    LinkWidgetLayout->addWidget(ToolSetting);
    LinkWidgetLayout->addWidget(ParaSetting);
    LinkWidget->setLayout(LinkWidgetLayout);

    //DownLoad Button Widget
    DownLoadWidget = new mywidget("DownLoadWidget",this,this);
    DownLoadWidget->setWindowIcon(QIcon(":/new/prefix1/res/downLoad"));
    DownLoadWidget->setWindowTitle("Download Setting");
    DownLoadWidget ->setWindowFlags(Qt::Dialog);
    DownLoadWidget->setWindowModality(Qt::WindowModal);
    DownLoadWidgetLayout = new QVBoxLayout();
    DownLoadSelect = new QGroupBox("Download File");
    DownLoadSettingGroup = new QHBoxLayout();
    DownLoadCurrent = new QRadioButton("Current Project");
    DownLoadCurrent->setChecked(true);
    DownLoadOther = new QRadioButton("Other Project");
    DownLoadSettingGroup->addWidget(DownLoadCurrent);
    DownLoadSettingGroup->addWidget(DownLoadOther);
    DownLoadSelect->setLayout(DownLoadSettingGroup);
    DownLoadSetting = new QGroupBox("DownLoad Seting");
    LayoutDownLoadSetting = new QGridLayout();
    DownLoadPathLabel = new QLabel("DownLoad Path:");
    DownLoadPath = new QLineEdit();
    DownLoadPath->setText("Dont find load file!");

    DownLoadCmdEditLabel = new QLabel("DownLoad CMD:");
    DownLoadCmd = new QLineEdit("load");
    LayoutDownLoadSetting->addWidget(DownLoadPathLabel,0,0);
    LayoutDownLoadSetting->addWidget(DownLoadPath,0,1);
    LayoutDownLoadSetting->addWidget(DownLoadCmdEditLabel,1,0);
    LayoutDownLoadSetting->addWidget(DownLoadCmd,1,1);
    DownLoadSetting->setLayout(LayoutDownLoadSetting);
    DownLoadWidgetLayout->addWidget(DownLoadSelect);
    DownLoadWidgetLayout->addWidget(DownLoadSetting);
    DownLoadWidget->setLayout(DownLoadWidgetLayout);

    //Verify Button Widget
    VerifyWidget = new mywidget("VerifyWidget",this,this);
    VerifyWidget->setWindowIcon(QIcon(":/new/prefix1/res/verifyFile"));
    VerifyWidget->setWindowTitle("Verify Setting");
    VerifyWidget->setWindowFlags(Qt::Dialog);
    VerifyWidget->setWindowModality(Qt::WindowModal);
    VerifyWidgetLayout = new QVBoxLayout();
    VerifySelect = new QGroupBox("Verify File");
    VerifySettingGroup = new QHBoxLayout();
    VerifyCurrent = new QRadioButton("Current Project");
    VerifyCurrent->setChecked(true);
    VerifyOther = new QRadioButton("Other Project");
    VerifySettingGroup->addWidget(VerifyCurrent);
    VerifySettingGroup->addWidget(VerifyOther);
    VerifySelect->setLayout(VerifySettingGroup);
    VerifySetting = new QGroupBox("DownLoad Seting");
    VerifySettingLayout = new QGridLayout();
    VerifyPathLabel = new QLabel("Verify Path:");
    VerifyPath = new QLineEdit();
    VerifyPath->setText("Dont find verify file!");

    VerifyCmdEditLabel = new QLabel("Verify CMD:");
    VerifyCmd = new QLineEdit("verify");
    VerifySettingLayout->addWidget(VerifyPathLabel,0,0);
    VerifySettingLayout->addWidget(VerifyPath,0,1);
    VerifySettingLayout->addWidget(VerifyCmdEditLabel,1,0);
    VerifySettingLayout->addWidget(VerifyCmd,1,1);
    VerifySetting->setLayout(VerifySettingLayout);
    VerifyWidgetLayout->addWidget(VerifySelect);
    VerifyWidgetLayout->addWidget(VerifySetting);
    VerifyWidget->setLayout(VerifyWidgetLayout);

    //Run Button Widget
    RunWidget = new mywidget("RunWidget",this,this);
    RunWidget->setWindowIcon(QIcon(":/new/prefix1/res/runFile"));
    RunWidget->setWindowTitle("Run Setting");
    RunWidget->setWindowFlags(Qt::Dialog);
    RunWidget->setWindowModality(Qt::WindowModal);
    RunWidgetLayout = new QVBoxLayout();
    RunSetting = new QGroupBox("CMD Setting");
    RunSettingLayout = new QGridLayout();
    RunCmdEditLabel = new QLabel("Run CMD:");
    RunCmd = new QLineEdit("run");
    RunSettingLayout->addWidget(RunCmdEditLabel,0,0);
    RunSettingLayout->addWidget(RunCmd,0,1);
    RunSetting->setLayout(RunSettingLayout);
    RunWidgetLayout->addWidget(RunSetting);
    RunWidget->setLayout(RunWidgetLayout);

    //Stop Button Widget
    StopWidget = new mywidget("StopWidget",this,this);
    StopWidget->setWindowIcon(QIcon(":/new/prefix1/res/stopDebug"));
    StopWidget->setWindowTitle("Stop Setting");
    StopWidget->setWindowFlags(Qt::Dialog);
    StopWidget->setWindowModality(Qt::WindowModal);
    StopWidgetLayout = new QVBoxLayout();
    StopSetting = new QGroupBox("CMD Setting");
    StopSettingLayout = new QGridLayout();
    StopCmdEditLabel = new QLabel("Stop CMD:");
    StopCmd = new QLineEdit("ctrl+c");
    StopSettingLayout->addWidget(StopCmdEditLabel,0,0);
    StopSettingLayout->addWidget(StopCmd,0,1);
    StopSetting->setLayout(StopSettingLayout);
    StopWidgetLayout->addWidget(StopSetting);
    StopWidget->setLayout(StopWidgetLayout);

    //Inst Button Widget
    InstWidget = new mywidget("InstWidget",this,this);
    InstWidget->setWindowIcon(QIcon(":/new/prefix1/res/instDebug"));
    InstWidget->setWindowTitle("Inst Setting");
    InstWidget->setWindowFlags(Qt::Dialog);
    InstWidget->setWindowModality(Qt::WindowModal);
    InstWidgetLayout = new QVBoxLayout();
    InstSetting = new QGroupBox("CMD Setting");
    InstSettingLayout = new QGridLayout();
    InstCmdEditLabel = new QLabel("Inst CMD:");
    InstCmd = new QLineEdit("inst 50 cpu0");
    InstSettingLayout->addWidget(InstCmdEditLabel,0,0);
    InstSettingLayout->addWidget(InstCmd,0,1);
    InstSetting->setLayout(InstSettingLayout);
    InstWidgetLayout->addWidget(InstSetting);
    InstWidget->setLayout(InstWidgetLayout);

    //Step Button Widget
    StepWidget = new mywidget("StepWidget",this,this);
    StepWidget->setWindowIcon(QIcon(":/new/prefix1/res/stepDebug"));
    StepWidget->setWindowTitle("Step Setting");
    StepWidget->setWindowFlags(Qt::Dialog);
    StepWidget->setWindowModality(Qt::WindowModal);
    StepWidgetLayout = new QVBoxLayout();
    StepSetting = new QGroupBox("CMD Setting");
    StepSettingLayout = new QGridLayout();
    StepCmdEditLabel = new QLabel("Step CMD:");
    StepCmd = new QLineEdit("step 1 cpu0");
    StepSettingLayout->addWidget(StepCmdEditLabel,0,0);
    StepSettingLayout->addWidget(StepCmd,0,1);
    StepSetting->setLayout(StepSettingLayout);
    StepWidgetLayout->addWidget(StepSetting);
    StepWidget->setLayout(StepWidgetLayout);

    //Continue Button Widget
    ContinueWidget = new mywidget("ContinueWidget",this,this);
    ContinueWidget->setWindowIcon(QIcon(":/new/prefix1/res/continueDebug"));
    ContinueWidget->setWindowTitle("Continue Setting");
    ContinueWidget->setWindowFlags(Qt::Dialog);
    ContinueWidget->setWindowModality(Qt::WindowModal);
    ContinueWidgetLayout = new QVBoxLayout();
    ContinueSetting = new QGroupBox("CMD Setting");
    ContinueSettingLayout = new QGridLayout();
    ContinueCmdEditLabel = new QLabel("Continue CMD:");
    ContinueCmd = new QLineEdit("cont");
    ContinueSettingLayout->addWidget(ContinueCmdEditLabel,0,0);
    ContinueSettingLayout->addWidget(ContinueCmd,0,1);
    ContinueSetting->setLayout(ContinueSettingLayout);
    ContinueWidgetLayout->addWidget(ContinueSetting);
    ContinueWidget->setLayout(ContinueWidgetLayout);

    //AddBp Button Widget
    BreakWidget = new mywidget("BreakWidget",this,this);
    BreakWidget->setWindowIcon(QIcon(":/new/prefix1/res/bpDebug"));
    BreakWidget->setWindowTitle("BreakPoint Setting");
    BreakWidget->setWindowFlags(Qt::Dialog);
    BreakWidget->setWindowModality(Qt::WindowModal);
    BreakWidget->setMinimumWidth(500);
    BreakWidgetLayout = new QVBoxLayout();
    BreakMap = new QGroupBox("Break Point Map");
    BreakMapLayout = new QGridLayout();
    BreakSettingTable = new QTableWidget(0,6);
    BreakSettingTable->setWordWrap(true);
    RowHeaders << "1";
    BreakSettingTable->setVerticalHeaderLabels(RowHeaders);
    ColumnHeaders << "Type" << "Address" << "What" << "Jump" << "State" << "Deleate";
    BreakSettingTable->setHorizontalHeaderLabels(ColumnHeaders);
    BreakSettingTable->resizeColumnsToContents();
    BreakSettingTable->resizeRowsToContents();
    Header = BreakSettingTable->horizontalHeader();
    Header->setSectionResizeMode(QHeaderView::Stretch);
    BreakSettingTable->setShowGrid(true);

    BreakMapLayout->addWidget(BreakSettingTable,0,0);
    BreakMap->setLayout(BreakMapLayout);
    BreakSetting = new QGroupBox("CMD Setting");
    BreakSettingLayout = new QGridLayout();
    BreakCmdEditLabel = new QLabel("AddBp CMD:");
    BreakCmd = new QLineEdit("bp");
    BreakSettingLayout->addWidget(BreakCmdEditLabel,0,0);
    BreakSettingLayout->addWidget(BreakCmd,0,1);
    BreakSetting->setLayout(BreakSettingLayout);
    BreakWidgetLayout->addWidget(BreakMap);
    BreakWidgetLayout->addWidget(BreakSetting);
    BreakWidget->setLayout(BreakWidgetLayout);

    //Quit Button Widget
    QuitWidget = new mywidget("QuitWidget",this,this);
    QuitWidget->setWindowIcon(QIcon(":/new/prefix1/res/quitDebug"));
    QuitWidget->setWindowTitle("Quit Setting");
    QuitWidget->setWindowFlags(Qt::Dialog);
    QuitWidget->setWindowModality(Qt::WindowModal);
    QuitWidgetLayout = new QVBoxLayout();
    QuitSetting = new QGroupBox("CMD Setting");
    QuitSettingLayout = new QGridLayout();
    QuitCmdEditLabel = new QLabel("Quit CMD:");
    QuitCmd = new QLineEdit("quit");
    QuitSettingLayout->addWidget(QuitCmdEditLabel,0,0);
    QuitSettingLayout->addWidget(QuitCmd,0,1);
    QuitSetting->setLayout(QuitSettingLayout);
    QuitWidgetLayout->addWidget(QuitSetting);
    QuitWidget->setLayout(QuitWidgetLayout);

}

void tools::InitDebugPara() {

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        GlobalSetting.beginGroup("LinkSetting");
        QString BaudRateSelect = GlobalSetting.value("BaudRateSelect").toString();
        if(BaudRateSelect!="")
            BaudRate->setCurrentText(BaudRateSelect);
        QString DsuComSelect   = GlobalSetting.value("DsuComSelect").toString();
        if(DsuComSelect!="")
            DsuCom->setCurrentText(DsuComSelect);
        QString DebugToolPath  = GlobalSetting.value("DebugToolPath").toString();
        if(DebugToolPath.contains("CurrentDir:", Qt::CaseInsensitive)){
            DebugToolPath = DebugToolPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
        }
        if(DebugToolPath!=""){
            ToolPath->setText(DebugToolPath);
        }
        QString InitFile = GlobalSetting.value("InitFile").toString();
        if(InitFile!="")
            InitFilePath->setText(InitFile);
        QString OtherPar = GlobalSetting.value("OtherPar").toString();
        if(OtherPar!="")
            ToolVariable->setText(OtherPar);
        QString LinkCmd = GlobalSetting.value("LinkCmd").toString();
        if(LinkCmd!="")
            ToolShow->setText(LinkCmd);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("DownloadSetting");
        QString DownLoadFileSelect = GlobalSetting.value("DownLoadFileSelect").toString();
        if(DownLoadFileSelect=="OtherProject"){
            DownLoadOther->setChecked(true);
        }else{
            DownLoadCurrent->setChecked(true);
        }
        QString DownLoadFilePath = GlobalSetting.value("DownLoadFilePath").toString();
        if(DownLoadFilePath != "")
            DownLoadPath->setText(DownLoadFilePath);
        QString DownLoadCMD = GlobalSetting.value("DownLoadCMD").toString();
        if(DownLoadCMD != "")
            DownLoadCmd->setText(DownLoadCMD);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("VerifySetting");
        QString VerifyFileSelect = GlobalSetting.value("VerifyFileSelect").toString();
        if(VerifyFileSelect=="OtherProject"){
            VerifyOther->setChecked(true);
        }else{
            VerifyCurrent->setChecked(true);
        }
        QString VerifyFilePath = GlobalSetting.value("VerifyFilePath").toString();
        if(VerifyFilePath != "")
            VerifyPath->setText(VerifyFilePath);
        QString VerifyCMDSelect = GlobalSetting.value("VerifyCMDSelect").toString();
        if(VerifyCMDSelect != "")
            VerifyCmd->setText(VerifyCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("RunSetting");
        QString RunCMDSelect = GlobalSetting.value("RunCMDSelect").toString();
        if(RunCMDSelect != "")
            RunCmd->setText(RunCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("StopSetting");
        QString StopCMDSelect = GlobalSetting.value("StopCMDSelect").toString();
        if(StopCMDSelect != "")
            StopCmd->setText(StopCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("InstSetting");
        QString InstCMDSelect = GlobalSetting.value("InstCMDSelect").toString();
        if(InstCMDSelect != "")
            InstCmd->setText(InstCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("StepSetting");
        QString StepCMDSelect = GlobalSetting.value("StepCMDSelect").toString();
        if(StepCMDSelect != "")
            StepCmd->setText(StepCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("ContinueSetting");
        QString ContinueCMDSelect = GlobalSetting.value("ContinueCMDSelect").toString();
        if(ContinueCMDSelect != "")
            ContinueCmd->setText(ContinueCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("BreakSetting");
        QString BreakCMDSelect = GlobalSetting.value("BreakCMDSelect").toString();
        if(BreakCMDSelect != "")
            BreakCmd->setText(BreakCMDSelect);
        GlobalSetting.endGroup();

        GlobalSetting.beginGroup("QuitSetting");
        QString QuitCMDSelect = GlobalSetting.value("QuitCMDSelect").toString();
        if(QuitCMDSelect != "")
            QuitCmd->setText(QuitCMDSelect);
        GlobalSetting.endGroup();

}
