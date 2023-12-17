#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QAction>
#include <QFileSystemWatcher>
#include "qsci_editor.h"
#include "file_tree.h"
#include "config_file.h"
#include "status_bar.h"

class file : public QWidget{

    Q_OBJECT

public:

    //Other Class
    QWidget *MainWindow;

    treemenu *ProjectTree;

    configfile *SettingFile;

    statusbar *StatusBar;

    //File QActions

    QAction *NewProjectOrFile;

    QAction *OpenProjectOrFile;

    QAction *CloseProject;

    QAction *CloseAllFiles;

    QAction *Save;

    QAction *SaveAllFiles;

    QAction *ReloadFiles;
	
	QAction *Exit;
	
	QAction *Print;

    QAction *CleanRecentFiles;

    QAction *CleanRecentProjects;

    //File QMenu

    QMenu *RecentProjects;

    QMenu *RecentFiles;

    //Mainwindow QWidgets

    QTabWidget *EditWidget;

    QList<qscieditor *> *EditList;

    //Global Variable

    QString SdkFilePath;

public:

    explicit file(QWidget *parent = nullptr,treemenu *ProjectTree= nullptr,configfile *SettingFile= nullptr,statusbar *StatusBar = nullptr);

    void ConnectSignal();

    void RefreshRecent(const QString &fileName);

    bool SaveFile(const QString &fileName,qscieditor * edit);

    void UpdataRecent();

    bool UpdataSdkFile(const QString &fileName);

    void ReOpenProjectOrFile(const QString &fileName);

    ~file();

public slots:

    void on_NewProjectOrFile_triggered();

	void on_OpenProjectOrFile_triggered();

    void on_Save_triggered();

    void on_SaveAllFiles_triggered();

    void on_CloseProject_triggered();

    void on_CloseAllFiles_triggered();

    void on_Exit_triggered();

    void on_ReloadFiles_triggered();

    void on_Print_triggered();

    void on_CleanRecentFiles_triggered();

    void on_CleanRecentProjects_triggered();

};

#endif // FILE_H
