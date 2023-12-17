#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciapis.h"
#include "new_project_or_file.h"
#include "file.h"
#include "edit.h"
#include "file_tree.h"
#include "status_bar.h"
#include "edit_widget.h"
#include "tools.h"
#include "compile.h"
#include "serial.h"
#include "windowset.h"
#include "jump_file.h"
#include "reg_calc.h"

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }

QT_END_NAMESPACE

class MainWindow : public QMainWindow{

    Q_OBJECT

public:

    //Other Class

    configfile *SettingFile;

    file *FileMenu;

    edit *EditMenu;

    statusbar *StatusBar;

    editwidget *EditWidget;

    treemenu *ProjectTree;

    tools *ToolsMenu;

    compile *CompileMenu;

    serial *SerialTool;

    windowset *WindowMenu;

    QToolBar *FileToolBar;

    jump_file *CtrlClickJump;

    reg_calc *RegCalc;

    QToolBar *EditToolBar;

    QToolBar *CompileToolBar;

    QToolBar *ToolsToolBar;

    QToolBar *WindowBar;

    //global varible

    QTabWidget * tabWidget_2;

    QWidget *CentralWidget;

    QLayout *CentralWidgetLayout;

    QSplitter *Splitter;

    QList<qscieditor *> *EditList;

public:

    MainWindow(QWidget *parent = nullptr);

    void CreateClass();

    void InitView();

    void ConnectSignals();

    void DefaultLayout();

    void SetAbout();

    void addToolBarIcon();

    ~MainWindow();

protected:

    void dropEvent(QDropEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;

private slots:


private:

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
