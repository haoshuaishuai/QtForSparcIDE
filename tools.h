#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>
#include <QAction>
#include <QTextEdit>
#include <QLineEdit>
#include <QProcess>
#include <QVBoxLayout>
#include <QMenu>
#include <QTableWidget>
#include <QDockWidget>

#include "tool_preference.h"
#include "file.h"
#include "mybutton.h"
#include "combobox.h"
#include "edit.h"
#include "status_bar.h"
#include "reg_calc.h"

class mywidget;

class tools : public QWidget
{
    Q_OBJECT

public:

    explicit tools(QWidget *parent = nullptr,file *FileMenu = nullptr,treemenu *ProjectTree = nullptr,edit *Edit = nullptr,statusbar *StatusBar = nullptr,QWidget *MainWindow = nullptr,reg_calc *RegCalc = nullptr);

    ~tools();

    void InitWidget();

    void ConnectSignal();

    void on_GenerateFlashFile_triggered();

    void on_Preference_triggered();

    void on_Send_triggered();

    void InitDebugPara();

    void convertBinaryFile(const QString& binFileName, const QString& arrayFileName,QsciScintilla* edit);

public:

    //SerialPort QAction
    QDockWidget *Terminal;
    QAction *SerialPort;
    QAction *ShowTerminal;
    QAction *ShowReg;

    //CmdDebug QAction
    QDockWidget *Debug;
    QAction *CmdDebug;
    QAction *ShowDebug;

    //RegCalculator
    QAction *RegCalculator;

    //Tools QAction
    mywidget * GenerateFlashFileWidget;
    QVBoxLayout *GenerateFlashFileLayout;
    QGroupBox *BootFileGroup;
    QHBoxLayout *BootFileLayout;
    QLineEdit *BootFilePath;
    QPushButton *BootFileSelect;
    QGroupBox *SourceFileGroup;
    QHBoxLayout *SourceFileLayout;
    QLineEdit *SourceFilePath;
    QPushButton *SourceFileSelect;
    QGroupBox *TargetFileGroup;
    QHBoxLayout *TargetFileLayout;
    QLineEdit *TargetFilePath;
    QPushButton *TargetFileSelect;
    QHBoxLayout* GenerateLayout;
    QSpacerItem* GenerateSpacer1;
    QSpacerItem* GenerateSpacer2;
    QWidget *GenerateWidget;
    QPushButton *Generate;
    QWidget *TextWidget;
    QHBoxLayout *TextWidgetLayout;
    QsciScintilla* edit;
    QAction *GenerateFlashFile;

    QAction *Preference;

    //Other Class
    tool_preference *ToolPreference;
    file *FileMenu;
    treemenu *ProjectTree;
    class edit *Edit;
    QDesktopWidget *desktop;
    statusbar *StatusBar;
    reg_calc *RegCalc;

    //Other Widgets
    QTextEdit *DebugMessage;
    combobox * CmdLine;
    QPushButton *Send;
    MyButton *Link;
    MyButton *Download;
    MyButton *Verify;
    MyButton *Run;
    MyButton *Stop;
    MyButton *Inst;
    MyButton *Step;
    MyButton *Continue;
    MyButton *AddBp;
    MyButton *Quit;
    MyButton *Clear;

    //Class
    QProcess *Process;
    QWidget *MainWindow;
    QWidget *CentralWidget;
    QTabWidget *editTab;

    //Link Widget
    mywidget *LinkWidget;
    QVBoxLayout *LinkWidgetLayout;

    QGroupBox *DsuSetting;
    QGridLayout *LayoutDsuSetting;
    QLabel *BaudRateLabel;
    QComboBox *BaudRate;
    QLabel *DsuComLabel;
    QComboBox *DsuCom;

    QGroupBox *ToolSetting;
    QGridLayout *LayoutToolSetting;
    QLabel *ToolPathLabel;
    QLineEdit *ToolPath;
    QPushButton *ToolPathOpen;
    QLabel *InitFilePathLabel;
    QLineEdit *InitFilePath;
    QPushButton *InitFilePathOpen;

    QGroupBox *ParaSetting;
    QGridLayout *LayoutParaSetting;
    QLabel *ToolVariableLabel;
    QLineEdit *ToolVariable;
    QLabel *ToolShowLabel;
    QString ToolShowString;
    QLineEdit *ToolShow;

    //DownLoad Widget
    mywidget *DownLoadWidget;
    QVBoxLayout *DownLoadWidgetLayout;

    QGroupBox *DownLoadSelect;
    QHBoxLayout *DownLoadSettingGroup;
    QRadioButton *DownLoadCurrent;
    QRadioButton *DownLoadOther;

    QGroupBox *DownLoadSetting;
    QGridLayout *LayoutDownLoadSetting;
    QLabel *DownLoadPathLabel;
    QLineEdit *DownLoadPath;
    QLabel *DownLoadCmdEditLabel;
    QLineEdit *DownLoadCmd;

    //Verify Widget
    mywidget *VerifyWidget;
    QVBoxLayout *VerifyWidgetLayout;

    QGroupBox *VerifySelect;
    QHBoxLayout *VerifySettingGroup;
    QRadioButton *VerifyCurrent;
    QRadioButton *VerifyOther;

    QGroupBox *VerifySetting;
    QGridLayout *VerifySettingLayout;
    QLabel *VerifyPathLabel;
    QLineEdit *VerifyPath;
    QLabel *VerifyCmdEditLabel;
    QLineEdit *VerifyCmd;

    //Run Widget
    mywidget *RunWidget;
    QVBoxLayout *RunWidgetLayout;

    QGroupBox *RunSetting;
    QGridLayout *RunSettingLayout;
    QLabel *RunCmdEditLabel;
    QLineEdit *RunCmd;

    //Stop Widget
    mywidget *StopWidget;
    QVBoxLayout *StopWidgetLayout;

    QGroupBox *StopSetting;
    QGridLayout *StopSettingLayout;
    QLabel *StopCmdEditLabel;
    QLineEdit *StopCmd;

    //Inst Widget
    mywidget *InstWidget;
    QVBoxLayout *InstWidgetLayout;

    QGroupBox *InstSetting;
    QGridLayout *InstSettingLayout;
    QLabel *InstCmdEditLabel;
    QLineEdit *InstCmd;

    //Step Widget
    mywidget *StepWidget;
    QVBoxLayout *StepWidgetLayout;

    QGroupBox *StepSetting;
    QGridLayout *StepSettingLayout;
    QLabel *StepCmdEditLabel;
    QLineEdit *StepCmd;

    //Continue Widget
    mywidget *ContinueWidget;
    QVBoxLayout *ContinueWidgetLayout;

    QGroupBox *ContinueSetting;
    QGridLayout *ContinueSettingLayout;
    QLabel *ContinueCmdEditLabel;
    QLineEdit *ContinueCmd;

    //BreakPoint Widget
    mywidget *BreakWidget;
    QVBoxLayout *BreakWidgetLayout;

    QGroupBox *BreakMap;
    QGridLayout *BreakMapLayout;
    QTableWidget *BreakSettingTable;
    QStringList RowHeaders;
    QStringList ColumnHeaders;
    QHeaderView *Header;

    QGroupBox *BreakSetting;
    QGridLayout *BreakSettingLayout;
    QLabel *BreakCmdEditLabel;
    QLineEdit *BreakCmd;

    //Quit Widget
    mywidget *QuitWidget;
    QVBoxLayout *QuitWidgetLayout;

    QGroupBox *QuitSetting;
    QGridLayout *QuitSettingLayout;
    QLabel *QuitCmdEditLabel;
    QLineEdit *QuitCmd;

signals:

private slots:
    void exec();
    void on_Link_Left_triggered();
    void on_Link_Right_triggered();
    void on_Download_Left_triggered();
    void on_Download_Right_triggered();
    void on_Verify_Left_triggered();
    void on_Verify_Right_triggered();
    void on_Run_Left_triggered();
    void on_Run_Right_triggered();
    void on_Stop_Left_triggered();
    void on_Stop_Right_triggered();
    void on_Inst_Left_triggered();
    void on_Inst_Right_triggered();
    void on_Step_Left_triggered();
    void on_Step_Right_triggered();
    void on_Continue_Left_triggered();
    void on_Continue_Right_triggered();
    void on_AddBp_Left_triggered();
    void on_AddBp_Right_triggered();
    void on_Quit_Left_triggered();
    void on_Quit_Right_triggered();
    void on_Clear_triggered();
    void on_DebugPathOpen_triggered();
    void on_InitPathOpen_triggered();
    void on_ToolShow_refresh();
    void onLoadRadioButtonToggled(bool checked);
    void onVerifyRadioButtonToggled(bool checked);
    void on_BootFileSelect_triggered();
    void on_SourceFileSelect_triggered();
    void on_TargetFileSelect_triggered();
    void on_Generate_triggered();
    void on_SerialPort_triggered();
    void on_CmdDebug_triggered();
    void on_RegCalculator_triggered();

};



#endif // TOOLS_H
