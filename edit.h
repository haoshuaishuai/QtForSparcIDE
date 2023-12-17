#ifndef EDIT_H
#define EDIT_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QDockWidget>

#include "qsci_editor.h"
#include "status_bar.h"
#include "config_file.h"
class treemenu;
#include "file_tree.h"

class editfind;
#include "edit_find.h"

class edit : public QWidget
{
    Q_OBJECT

public:

    //Edit QAction
    QAction *UnDo;

    QAction *ReDo;

    QAction *Cut;

    QAction *Copy;

    QAction *Paste;

    QAction *Delate;

    QAction *SelectAll;

    QAction *FindOrReplace;

    QAction *GoToLine;

    QAction *SelectEconding;

    //Other Class
    QWidget *MainWindow;

    treemenu *ProjectTree;

    configfile *SettingFile;

    statusbar *StatusBar;

    editfind *EditFind;

    //Other Widget

    QTabWidget *EditWidget;

    QList<qscieditor *> *EditList;

    QTabWidget *MessageWidget;

    QScrollArea *ScrollArea;

    QDockWidget *Output;

public:

    explicit edit(QWidget *parent = nullptr,treemenu *ProjectTree = nullptr,configfile *SettingFile = nullptr,statusbar *StatusBar = nullptr);

    void InitWidget();

    void SetEdit(qscieditor* edit);

    void RefreshEdit(qscieditor* edit);

    void RefreshEditList();

    void ConnectSignal();

    void on_UnDo_triggered();

    void on_ReDo_triggered();

    void on_Cut_triggered();

    void on_Copy_triggered();

    void on_Paste_triggered();

    void on_Delate_triggered();

    void on_SelectAll_triggered();

    void on_FindOrReplace_triggered();

    void on_GoToLine_triggered();

    void on_SelectEconding_triggered();

private slots:

    //void on_OpenProject_triggered();

};

#endif // EDIT_H
