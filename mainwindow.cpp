#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QPrintDialog>
#include <QDomDocument>
#include <QScrollArea>
#include <QToolBar>
#include <QtCore>
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDragEnterEvent>
#include <QMimeData>

#include "new_project_or_file.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"
#include "file_tree.h"
#include "file.h"
#include "edit.h"
#include "tools.h"
#include "combobox.h"
#include "jump_file.h"
#include "qsci_editor.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){

    ui->setupUi(this);

    this->setAcceptDrops(true);

    this->addToolBarIcon();

    this->ConnectSignals();

    this->InitView();

}

//create other class
void MainWindow::CreateClass(){

    //1 new status bar
    this->StatusBar = new statusbar();
    this->StatusBar->StatusBar = ui->statusbar;

    //new config file
    this->SettingFile = new configfile();

    //new jump file
    this->CtrlClickJump = new jump_file(this,SettingFile);

    //create reg caclulate
    RegCalc = new reg_calc();

    //project tree
    this->ProjectTree = new treemenu(this,this->EditMenu,this->SettingFile,this->StatusBar,CtrlClickJump);
    this->ProjectTree->CentralWidget = ui->centralwidget;
    this->ProjectTree->editlist = EditList;
    this->ProjectTree->editTab  = tabWidget_2;

    //new edit menu
    this->EditMenu = new edit(this,this->ProjectTree,this->SettingFile,this->StatusBar);
    EditMenu->UnDo = ui->actionUndo;
    EditMenu->ReDo = ui->actionRedo;
    EditMenu->Cut = ui->actionCut;
    EditMenu->Copy = ui->actionCopy;
    EditMenu->Paste = ui->actionPaste;
    EditMenu->Delate = ui->actionDelate;
    EditMenu->SelectAll = ui->actionSelect_All;
    EditMenu->FindOrReplace = ui->actionFind_Replace;
    EditMenu->GoToLine  = ui->actionGo_to_Line;
    EditMenu->SelectEconding = ui->actionSelect_Econding;
    EditMenu->EditWidget = tabWidget_2;
    EditMenu->EditList = this->EditList;
    EditMenu->MessageWidget = ui->tabWidget_3;
    EditMenu->ScrollArea = ui->ScrollFind;
    EditMenu->Output = ui->dockWidget_2;

    //new file menu
    this->FileMenu = new file(this,this->ProjectTree,this->SettingFile,this->StatusBar);
    FileMenu->NewProjectOrFile    = ui->actionnew;
    FileMenu->OpenProjectOrFile   = ui->actionOpen_Project;
    FileMenu->CloseProject        = ui->actionClose_Project;
    FileMenu->CloseAllFiles       = ui->actionClose_All_Files;
    FileMenu->Save                = ui->actionSave;
    FileMenu->SaveAllFiles        = ui->actionSave_All_Files;
    FileMenu->ReloadFiles         = ui->actionReload_File;
    FileMenu->Exit                = ui->actionExit;
    FileMenu->Print               = ui->actionPrint;
    FileMenu->CleanRecentFiles    = ui->actionClean_ALL;
    FileMenu->CleanRecentProjects = ui->actionrecentprojects;
    FileMenu->RecentProjects      = ui->menuRecent_Projects;
    FileMenu->RecentFiles         = ui->menuRecent_Files_3;
    FileMenu->EditWidget          = tabWidget_2;
    FileMenu->EditList            = this->EditList;

    //new tools menu
    this->ToolsMenu = new tools(nullptr,this->FileMenu,this->ProjectTree,this->EditMenu,this->StatusBar,this,RegCalc);
    ToolsMenu->SerialPort = ui->actionSerial_Port_Assistant;
    ToolsMenu->Terminal = ui->dockWidget_3;
    ToolsMenu->CmdDebug = ui->actionCmd_Debug;
    ToolsMenu->Debug = ui->dockWidget_4;
    ToolsMenu->GenerateFlashFile = ui->actionGenerate_Flash_File;
    ToolsMenu->RegCalculator = ui->actionRegister_calculator;
    ToolsMenu->Preference = ui->actionPreference;
    ToolsMenu->DebugMessage = ui->DebugMessage;
    ToolsMenu->Send = ui->Send;
    ToolsMenu->CmdLine = ui->CmdLine;
    ToolsMenu->Link = ui->Link;
    ToolsMenu->Download = ui->Download;
    ToolsMenu->Verify = ui->Verify;
    ToolsMenu->Run = ui->Run;
    ToolsMenu->Stop = ui->Stop;
    ToolsMenu->AddBp = ui->AddBP;
    ToolsMenu->Continue = ui->Continue;
    ToolsMenu->Quit = ui->Quit;
    ToolsMenu->Clear = ui->Clear;
    ToolsMenu->Inst = ui->Inst;
    ToolsMenu->Step = ui->Step;
    ToolsMenu->ShowTerminal = ui->actionTerminal;
    ToolsMenu->ShowDebug = ui->actionCMDDebug;
    ToolsMenu->ShowReg = ui->actionRegister;
    ToolsMenu->CentralWidget = ui->centralwidget;
    ToolsMenu->editTab = tabWidget_2;

    //new compile menu
    CompileMenu = new compile(this,this->FileMenu,this->SettingFile,this->ProjectTree,this->StatusBar);
    CompileMenu->CompileProject = ui->actionCompile_Project;
    CompileMenu->ReCompileAllFiles = ui->actionReCompile_Project;
    CompileMenu->CleanProject = ui->actionClean_Project;
    CompileMenu->ScrollArea = ui->ScrollComplie;
    CompileMenu->EditList = EditList;
    CompileMenu->MessageWidget = ui->tabWidget_3;;
    CompileMenu->Output = ui->dockWidget_2;

    //new edit widget
    this->EditWidget = new editwidget(this,this->FileMenu,this->StatusBar);
    this->EditWidget->EditWidget = tabWidget_2;
    this->EditWidget->EditList = EditList;

    //new serial widget
    SerialTool = new serial(nullptr,this->StatusBar);
    SerialTool->SerialMessage = ui->SerialMessage;
    SerialTool->SendLine = ui->SendDataLine;
    SerialTool->Send = ui->SerialSend;
    SerialTool->Open = ui->Open;
    SerialTool->Clear = ui->UartClear;

    //new window widget
    WindowMenu = new windowset(nullptr,this->StatusBar);
    WindowMenu->ShowResource = ui->actionResource;
    WindowMenu->ShowOutput = ui->actionOutput;
    WindowMenu->ShowTerminal = ui->actionTerminal;
    WindowMenu->ShowDebug = ui->actionCMDDebug;
    WindowMenu->ShowReg = ui->actionRegister;
    WindowMenu->ShowToolBar = ui->actionToolBar;
    WindowMenu->ShowStatusBar = ui->actionStatusbar;
    WindowMenu->LastWindow = ui->actionLast_Window;
    WindowMenu->NextWindow = ui->actionNext_Window;
    WindowMenu->SerialPort = ui->actionSerial_Port_Assistant;
    WindowMenu->CmdDebug = ui->actionCmd_Debug;
    WindowMenu->Resource = ui->dockWidget;
    WindowMenu->Output = ui->dockWidget_2;
    WindowMenu->Terminal = ui->dockWidget_3;
    WindowMenu->Debug = ui->dockWidget_4;
    WindowMenu->CentralWidget = ui->centralwidget;

    WindowMenu->FileToolBar = FileToolBar;
    WindowMenu->EditToolBar = EditToolBar;
    WindowMenu->CompileToolBar = CompileToolBar;
    WindowMenu->ToolsToolBar = ToolsToolBar;
    WindowMenu->WindowBar = WindowBar;

    WindowMenu->Status = ui->statusbar;
    WindowMenu->RegCalcuWidget = RegCalc;
    WindowMenu->RegCalcu = ui->actionRegister_calculator;
    WindowMenu->EditWidget = tabWidget_2;
    WindowMenu->ProjectTree = ProjectTree;
    WindowMenu->EditList = EditList;
    WindowMenu->EditString = ProjectTree->EditString;
    WindowMenu->Position = &ProjectTree->Position;

    //cross use var
    this->ProjectTree->EditWidget = this->EditWidget;

}

//init sdk gui
void MainWindow::InitView(){

    //create main tabwidget
    tabWidget_2 = new QTabWidget();
    tabWidget_2->setTabsClosable(true);

    //create global var : edit list
    EditList = new QList<qscieditor *>();

    //create class
    this->CreateClass();

    //create edit find widget
    EditMenu->InitWidget();

    //create status bar init widget
    StatusBar->InitWidget();

    //connect signals
    ProjectTree->ConnectSignal();

    //create xml config file
    QString fileName = QCoreApplication::applicationDirPath()+"/config.xml";
    SettingFile->createXmlFile(fileName);

    //connect signals
    ToolsMenu->ConnectSignal();

    //connect signals
    CompileMenu->ConnectSignal();

    //updata recent projects and files
    FileMenu->RefreshRecent(fileName);

    //connect signals
    FileMenu->ConnectSignal();

    //connect signals
    SerialTool->ConnectSignal();

    //add project tree to gui
    ui->tabWidget->clear();
    ui->tabWidget->addTab(ProjectTree,"Project View");

    //edit Widget clear
    EditWidget->init();
    EditWidget->ConnectSignal();

    //edit widget init
    EditMenu->ConnectSignal();

    //windowset widget init
    WindowMenu->ConnectSignals();

    //set central widget
    CentralWidget = ui->centralwidget;
    Splitter = new QSplitter(Qt::Vertical, CentralWidget);
    CentralWidgetLayout = CentralWidget->layout();

    Splitter->addWidget(tabWidget_2);
    Splitter->addWidget(RegCalc);

    CentralWidgetLayout->setAlignment(Qt::AlignBottom);
    CentralWidgetLayout->addWidget(Splitter);

    //set mainwindow default layout
    this->DefaultLayout();

    //set exe icon
    QIcon Icon(":/new/prefix1/res/SDK");
    QIcon scaledIcon = Icon.pixmap(QSize(128, 128));
    this->setWindowIcon(scaledIcon);
    this->setWindowTitle("Sparc Develope Kit");

    //set tabwidget icon
    tabWidget_2->hide();
    QString styleSheet = "background-image: url(:/new/prefix1/res/BMTI.png);background-repeat: no-repeat;background-position: center;";
    ui->centralwidget->setStyleSheet(styleSheet);

    StatusBar->DetermineMessage("Sdk Init Done");

}

void MainWindow::ConnectSignals(){

    connect(ui->actionDefault_Window_Layout,&QAction::triggered,this,[=]{DefaultLayout();});
    connect(ui->actionAbout,&QAction::triggered,this,[=]{SetAbout();});

}

void MainWindow::DefaultLayout(){

    ui->dockWidget->setVisible(true);
    ui->dockWidget_2->setVisible(true);
    ui->dockWidget_3->setVisible(false);
    ui->dockWidget_4->setVisible(false);
    RegCalc->setVisible(false);
    this->addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, ui->dockWidget_2);
    this->addDockWidget(Qt::RightDockWidgetArea, ui->dockWidget_3);
    this->addDockWidget(Qt::BottomDockWidgetArea, ui->dockWidget_4);
    resizeDocks({ui->dockWidget}, {200}, Qt::Horizontal);
    resizeDocks({ui->dockWidget_2}, {200}, Qt::Vertical);
    resizeDocks({ui->dockWidget_4}, {200}, Qt::Vertical);
    this->tabifyDockWidget (ui->dockWidget_2, ui->dockWidget_4);
    ui->dockWidget_2->raise();

    FileToolBar->setVisible(true);
    EditToolBar->setVisible(true);
    CompileToolBar->setVisible(true);
    ToolsToolBar->setVisible(true);
    WindowBar->setVisible(true);
    ui->statusbar->setVisible(true);

    ui->actionResource->setChecked(true);
    ui->actionOutput->setChecked(true);
    ui->actionTerminal->setChecked(false);
    ui->actionCMDDebug->setChecked(false);
    ui->actionToolBar->setChecked(true);
    ui->actionStatusbar->setChecked(true);
    ui->actionRegister->setChecked(false);

    ui->actionSerial_Port_Assistant->setChecked(false);
    ui->actionCmd_Debug->setChecked(false);
    ui->actionRegister_calculator->setChecked(false);

    ui->tabWidget_3->setCurrentIndex(0);
    this->setContextMenuPolicy(Qt::NoContextMenu);

}

void MainWindow::SetAbout(){

    QString SoftwareName = "Sparc Develope Kit(SDK)";
    QString SoftwareVersion = "Ver2.0";
    QString Company = "Copyright 2022-2023 @ BMTI.\r\n";
    QString VersionHistory = "Click Details Show Version History.";
    QString Ver1_0 = "  1.Base Version\r\n";
    QString Ver2_0 = "  1.Add Debug\r\n  2.Add Terminal\r\n  3.Add Func/Def/Inc Jump\r\n  4.Add Register Calculator\r\n";
    QString VersionCorrection =
        "Ver1.0\r\n"+Ver1_0+
        "Ver2.0\r\n"+Ver2_0
        ;

    QMessageBox *AboutMessageBox = new QMessageBox();
    AboutMessageBox->setFixedSize(800, AboutMessageBox->sizeHint().height());
    AboutMessageBox->setWindowTitle("About");
    AboutMessageBox->setWindowIcon(QIcon(":/new/prefix1/res/aboutSDK"));
    AboutMessageBox->setText(SoftwareName + " " + SoftwareVersion);
    AboutMessageBox->setInformativeText(Company+VersionHistory);
    AboutMessageBox->setDetailedText(VersionCorrection);
    AboutMessageBox->exec();

}


void MainWindow::addToolBarIcon(){

    FileToolBar = new QToolBar("FileToolBar",this);
    FileToolBar->addAction(ui->actionnew);
    FileToolBar->addAction(ui->actionOpen_Project);
    FileToolBar->addSeparator();
    FileToolBar->addAction(ui->actionClose_Project);
    FileToolBar->addAction(ui->actionClose_All_Files);
    FileToolBar->addSeparator();
    FileToolBar->addAction(ui->actionSave);
    ui->actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    FileToolBar->addAction(ui->actionSave_All_Files);
    FileToolBar->addAction(ui->actionReload_File);
    FileToolBar->setIconSize(QSize(19, 19));
    FileToolBar->setStyleSheet("QToolBar{spacing:1px;}");
    this->addToolBar(FileToolBar);

    EditToolBar = new QToolBar("EditToolBar",this);
    EditToolBar->addAction(ui->actionUndo);
    EditToolBar->addAction(ui->actionRedo);
    EditToolBar->addSeparator();
    EditToolBar->addAction(ui->actionFind_Replace);
    ui->actionFind_Replace->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    EditToolBar->addAction(ui->actionGo_to_Line);
    EditToolBar->addAction(ui->actionSelect_Econding);
    EditToolBar->setIconSize(QSize(19, 19));
    EditToolBar->setStyleSheet("QToolBar{spacing:1px;}");
    this->addToolBar(EditToolBar);

    CompileToolBar = new QToolBar("CompileToolBar",this);
    CompileToolBar->addAction(ui->actionCompile_Project);
    ui->actionCompile_Project->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    CompileToolBar->addAction(ui->actionReCompile_Project);
    CompileToolBar->addAction(ui->actionClean_Project);
    CompileToolBar->setIconSize(QSize(19, 19));
    CompileToolBar->setStyleSheet("QToolBar{spacing:1px;}");
    this->addToolBar(CompileToolBar);

    ToolsToolBar = new QToolBar("ToolsToolBar",this);
    ToolsToolBar->addAction(ui->actionSerial_Port_Assistant);
    ToolsToolBar->addSeparator();
    ToolsToolBar->addAction(ui->actionCmd_Debug);
    ToolsToolBar->addSeparator();
    ToolsToolBar->addAction(ui->actionGenerate_Flash_File);
    ToolsToolBar->addSeparator();
    ToolsToolBar->addAction(ui->actionRegister_calculator);
    ToolsToolBar->addSeparator();
    ToolsToolBar->addAction(ui->actionPreference);
    ToolsToolBar->setIconSize(QSize(19, 19));
    ToolsToolBar->setStyleSheet("QToolBar{spacing:1px;}");
    this->addToolBar(ToolsToolBar);

    WindowBar = new QToolBar("WindowBar",this);
    WindowBar->addAction(ui->actionLast_Window);
    WindowBar->addAction(ui->actionNext_Window);
    WindowBar->addSeparator();
    WindowBar->addAction(ui->actionDefault_Window_Layout);
    WindowBar->addSeparator();
    WindowBar->addAction(ui->actionAbout);
    WindowBar->setIconSize(QSize(19, 19));
    WindowBar->setStyleSheet("QToolBar{spacing:1px;}");
    this->addToolBar(WindowBar);

}

void MainWindow::dropEvent(QDropEvent* event){

    const QList<QUrl> urls = event->mimeData()->urls();
    for (const QUrl& url : urls){

        QString filePath = url.toLocalFile();
        QFileInfo DragFile(filePath);
        if(DragFile.isFile()){
            FileMenu->ReOpenProjectOrFile(filePath);
        }else{
            StatusBar->DetermineMessage("Not Accept File");
        }

    }

    event->accept();

}

void MainWindow::dragEnterEvent(QDragEnterEvent* event){

    if (event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }

}

MainWindow::~MainWindow(){

    if(ToolsMenu->Process!=NULL && ToolsMenu->Process->state() == QProcess::Running) {
        QString quit = "quit\r\n";
        const char * CmdChar= quit.toUtf8().data();
        ToolsMenu->Process->write(CmdChar);
        ToolsMenu->Process->waitForBytesWritten();
        ToolsMenu->Process->kill();
        ToolsMenu->Process->waitForFinished();
        ToolsMenu->Process->deleteLater();
        ToolsMenu->Process = 0;
    }
    delete ui;

}
