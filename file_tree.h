#ifndef NEWBUILDFILE_H
#define NEWBUILDFILE_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QTreeWidget>
#include <QDir>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QFileSystemWatcher>
#include <QWidget>

#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciapis.h"
class edit;
#include "edit.h"
class editwidget;
#include "edit_widget.h"
#include "status_bar.h"
#include "jump_file.h"
#include "qsci_editor.h"

class NewBulidFile : public QDialog{

    Q_OBJECT

public:

    NewBulidFile(QWidget *parent=0);

    QLineEdit *fileNameEdit;

    QComboBox *fileNameTypeBox;

    QPushButton *okButton;

    QPushButton *cancalButton;

};

class NewBuildfolder : public QDialog{

    Q_OBJECT

public:

    NewBuildfolder(QWidget *parent=0);

    QLineEdit *fileNameEdit;

    QPushButton *okButton;

    QPushButton *cancalButton;

};

class NewRenameFile : public QDialog{

    Q_OBJECT

public:

    NewRenameFile(QWidget *parent=0);

    QLineEdit   *fileNameEdit;

    QPushButton *okButton;

    QPushButton *cancalButton;

};


class treemenu : public QTreeWidget{

    Q_OBJECT

public:

    explicit treemenu(QWidget *parent = 0,class edit *EditMenu = nullptr,configfile *SettingFile = nullptr,statusbar *StatusBar = nullptr,jump_file *CtrlClickJump = nullptr);

    void CreateTopItem(QString sdkpath,QString path,bool IsCreate);

    void RemoveTopItem();

    void FindFile(QDir *path,QTreeWidgetItem *parent,QString SdkFile,bool IsCreate);

    void buttonCollapseItem();

    void loadFile(const QString &fileName);

    void SetCurrentFile(const QString &fileName,qscieditor *edit);

    bool saveFile(const QString &fileName);

    void SetEdit(qscieditor* edit);

    void NewDir(QDir *path,QTreeWidgetItem *parent,QString NewPath,bool NeedCopy);

    void ConnectSignal();

    bool CreateNewFile(QString FilePath);

    //file Watcher

    QFileSystemWatcher *FileWatcher;

    QStringList WatchFiles;

    QStringList WatchDirs;

    QList<qscieditor *> *editlist;

    QStringList ChangedFiles;

    QTabWidget *editTab;

    statusbar *StatusBar;

    jump_file *CtrlClickJump;

    class edit *EditMenu;

    configfile *SettingFile;

    class editwidget *EditWidget;

    QList<QString> *EditString;

    int Position;

    QWidget *CentralWidget;

    //global vars

    QString SdkFile;

    QString IconPath;

public:

    QTreeWidgetItem *root;

    QMenu *dirMenu;

    QMenu *fileMenu;

    QTreeWidgetItem *nowItem;

    int nowCol;

    NewBulidFile *newFileWidget;

    NewBuildfolder *newDirWidget;

    NewRenameFile *newRenameFile;

signals:

    void showTextSignal(QString path,QString name,QTreeWidgetItem *item);

    void collapseItemSignal(const QTreeWidgetItem *item);

public slots:

    void DoubleClickpath(QTreeWidgetItem *item, int column);

    void itemPressedSlot(QTreeWidgetItem * pressedItem, int column);

    void tempActionInformation(QAction *act);

    void bulidNewFileSlot(bool flag);

    void closeBuildFileSlot(bool flag);

    void buildNewDirSlot(bool flag);

    void cancelDirSlot(bool flag);

    void cancelRenameSlot(bool flag);

    void renameFileSlot();

    void FileChange(QString str);

    void on_TabBar_triggered(int index);

};

#endif // NEWBUILDFILE_H
