#ifndef NEWPROJECTORFILE_H
#define NEWPROJECTORFILE_H

#include <QWidget>
#include <QListWidgetItem>
class newprojectorfilemessage;
#include "new_project_or_file_message.h"
#include "mainwindow.h"
#include "file_tree.h"
#include "config_file.h"
#include "file.h"
#include "status_bar.h"

namespace Ui {
    class newprojectorfile;
}

class newprojectorfile : public QWidget
{
    Q_OBJECT

public:

    newprojectorfilemessage *message;

    treemenu *ProjectTree;

    configfile *SettingFile;

    file *FileMenu;

    statusbar *StatusBar;

    QWidget *MainWindow;

public:

    QString baseInformation;

    QString newProrfi;

    QString ProjectIndex;

    QString ProjectName;

    QString ProjectLocation;

    QString FiletName;

    QString FileLocation;

public:

    explicit newprojectorfile(QWidget *parent = nullptr,treemenu *ProjectTree = nullptr,
                              configfile *SettingFile = nullptr,file *FileMenu = nullptr,statusbar *StatusBar = nullptr);

    void InitNewProject();

    void SetParmater();

    ~newprojectorfile();

private slots:

    void on_newnext_clicked();

    void on_newfinish_clicked();

    void on_newcancle_clicked();

    void on_newbrowse_clicked();

    void onNewFileRadioButtonToggled(bool Show);

public:
    Ui::newprojectorfile *ui;
};

#endif // NEWPROJECTORFILE_H
