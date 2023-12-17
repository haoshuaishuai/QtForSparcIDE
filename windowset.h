#ifndef WINDOWSET_H
#define WINDOWSET_H

#include <QWidget>
#include <QDockWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QTabWidget>

#include "file_tree.h"
#include "status_bar.h"
#include "qsci_editor.h"
#include "reg_calc.h"

class windowset : public QWidget{

    Q_OBJECT

public:


    QWidget *CentralWidget;

    treemenu *ProjectTree;
    statusbar *StatusBar;

    QAction *ShowResource;
    QAction *ShowOutput;
    QAction *ShowTerminal;
    QAction *ShowDebug;
    QAction *ShowReg;

    QAction *ShowToolBar;
    QAction *ShowStatusBar;

    QAction *LastWindow;
    QAction *NextWindow;

    QAction *DefaultWindowLayout;

    QAction *SerialPort;
    QAction *CmdDebug;
    QAction *RegCalcu;

    QDockWidget *Resource;
    QDockWidget *Output;
    QDockWidget *Terminal;
    QDockWidget *Debug;

    QToolBar *FileToolBar;
    QToolBar *EditToolBar;
    QToolBar *CompileToolBar;
    QToolBar *ToolsToolBar;
    QToolBar *WindowBar;

    QStatusBar *Status;

    reg_calc *RegCalcuWidget;

    QTabWidget *EditWidget;
    QList<qscieditor *> *EditList;
    QList<QString> *EditString;
    int * Position;

    explicit windowset(QWidget *parent = nullptr,statusbar *StatusBar = nullptr);

public:

    void ConnectSignals();
    void on_ShowResource_triggered();
    void on_ShowOutput_triggered();
    void on_ShowTerminal_triggered();
    void on_ShowDebug_triggered();
    void on_ShowReg_triggered();
    void on_ShowToolBar_triggered();
    void on_ShowStatusBar_triggered();
    void on_LastWindow_triggered();
    void on_NextWindow_triggered();
    void on_DefaultWindowLayout_triggered();

signals:

};

#endif // WINDOWSET_H
