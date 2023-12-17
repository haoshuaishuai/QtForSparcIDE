#include "edit_widget.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciapis.h"
#include "status_bar.h"

#include <QMessageBox>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QTabWidget>
#include <Qt>
#include <QApplication>

editwidget::editwidget(QWidget *Parent,file *FileMenu,statusbar *Statusbar){

    this->MainWindow = Parent;
    this->FileMenu = FileMenu;
    this->Statusbar = Statusbar;

}

//init edit widget
void editwidget::init(){

    EditWidget->clear();
    EditWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    fileMenu = new QMenu();
    fileMenu->addAction("Close This Files");
    fileMenu->addAction("Close Others Files");
    fileMenu->addAction("Close All Files");

}

//connect signals
void editwidget::ConnectSignal(){

    connect(EditWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(CloseTab(int)));
    connect(EditWidget, &QTabWidget::customContextMenuRequested, this, &editwidget::itemPressedSlot);
    connect(fileMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));

};

void editwidget::itemPressedSlot(){

    if(EditWidget->count()>0){
        fileMenu->exec(QCursor::pos());
    }

}

void editwidget::tempActionInformation(QAction *act){

    if(act->text() == "Close This Files"){

        CloseTab(EditWidget->currentIndex());

    }
    if(act->text() == "Close Others Files"){

        Statusbar->DetermineMessage("Close Others Files Success");
        while(EditWidget->count()>1){
            int j = EditWidget->currentIndex();
            if(j!=0){
                CloseTabBar(0);
            }else{
                CloseTabBar(1);
            }
        }

    }
    if(act->text() == "Close All Files"){

        FileMenu->on_CloseAllFiles_triggered();

    }

}

void editwidget::CloseTab(int index){

    Statusbar->DetermineMessage("Close File: "+EditWidget->tabText(index));
    CloseTabBar(index);

}


//close file tab
void editwidget::CloseTabBar(int index){

    if(EditWidget->tabText(index)[0]=="*"){
        QMessageBox::StandardButton box=QMessageBox::information(this,QString::fromLocal8Bit("Warning"),
                                                                 QString::fromLocal8Bit("Save Change Files?"),
                                                                 QMessageBox::Yes|QMessageBox::No);
        switch(box){
            case QMessageBox::Yes:
            for(int i=0;i<EditList->size();i++){
                if(EditList->at(i)->whatsThis()==EditWidget->widget(index)->whatsThis()){
                     FileMenu->SaveFile(EditList->at(i)->whatsThis(),EditList->at(i));
                }
            }
        }
    }

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis()==EditWidget->widget(index)->whatsThis()){
             EditList->removeAt(i);
        }
    }

    EditWidget->removeTab(index);

}
