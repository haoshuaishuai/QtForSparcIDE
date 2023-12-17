#ifndef COMPILE_H
#define COMPILE_H

#include <QObject>
#include <QAction>
#include <QProcess>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDockWidget>
#include "status_bar.h"
#include "file.h"
#include "config_file.h"
#include "file_tree.h"
#include "qsci_editor.h"

class LockerButton2 : public QPushButton{

    Q_OBJECT
public:
    /// @brief 构造方法
    explicit LockerButton2(QWidget* parent = nullptr);

    /// @brief SetImageLabel
    /// 设置按钮图标
    void SetImageLabel(const QPixmap &pixmap);

    /// @brief SetTextLabel
    /// 设置按钮文字
    void SetTextLabel(QString text);

    /// @brief GetImageHandle
    /// 返回m_imageLabel
    QLabel* GetImageHandle();

    /// @brief GetImageHandle
    /// 返回m_textLabel
    QLabel* GetTextHandle();

private:
    // 按钮图标
    QLabel* m_imageLabel;

    // 按钮文字
    QLabel* m_textLabel;

};

class compile : public QObject
{
    Q_OBJECT
public:
    explicit compile(QObject *parent = nullptr,file *FileMenu = nullptr,configfile *SettingFile = nullptr,treemenu *ProjectTree = nullptr,statusbar *StatusBar = nullptr);

    void ConnectSignal();

    void on_CompileProject_triggered();

    void on_ReCompileAllFiles_triggered();

    void on_CleanProject_triggered();

    bool CreateMakefile();

    bool CreateLinkfile(QString TextAddress);

    bool CompileFile(QString FilePath,bool ReCompileAllFiles);

    void SetUpUI();

public:

    //Other Class
    file *FileMenu;

    configfile *SettingFile;

    treemenu *ProjectTree;

    statusbar *StatusBar;

public:

    QAction *CompileProject;

    QAction *ReCompileAllFiles;

    QAction *CleanProject;

    QProcess *ExecMakefile;

    QScrollArea *ScrollArea;

    QList<qscieditor *> *EditList;

    QStringList *CompileError;

    QStringList *CompileErrorAll;

    QStringList *CompileOutput;

    QTimer *Timer;

    //Compile Message

    QStringList FileList;

    QVBoxLayout* FindButtonLayout;

    QWidget *FindButtonWidget;

    LockerButton2 * FindStringButton;

    QStringList *CompileTools;

    QString LdFile;

    QTreeWidgetItem *nowItem;

    QTabWidget *MessageWidget;

    QDockWidget *Output;

signals:

private slots:

    void Exec();
    void ExecClean();
    void Finished();
    void TimeOut();

};

#endif // COMPILE_H
