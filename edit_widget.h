#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QWidget>
#include <QTabWidget>

#include "qsci_editor.h"
#include "file_tree.h"
#include "config_file.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciapis.h"
class file;
#include "file.h"
#include "status_bar.h"

class editwidget : public QWidget
{
    Q_OBJECT

public:

     //Other Class
     QWidget *MainWindow;

     file *FileMenu;

     statusbar *Statusbar;

     //Other Widget

     QTabWidget *EditWidget;

     QList<qscieditor *> *EditList;

     QMenu *fileMenu;

public:

    explicit editwidget(QWidget *Parent = nullptr,file *FileMenu = nullptr,statusbar *Statusbar = nullptr);

    void init();

    void ConnectSignal();

    void itemPressedSlot();

    void CloseTabBar(int index);

signals:

public slots:

     void CloseTab(int index);

     void tempActionInformation(QAction *act);

};

#endif // EDITWIDGET_H
