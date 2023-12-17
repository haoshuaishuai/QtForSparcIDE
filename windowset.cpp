#include <QAction>
#include <QDebug>
#include <QTabBar>
#include <QWidget>
#include "windowset.h"
#include "status_bar.h"

windowset::windowset(QWidget *parent,statusbar *StatusBar):QWidget{parent}{

    this->StatusBar = StatusBar;

}

void windowset::ConnectSignals(){

    connect(ShowResource,&QAction::triggered,this,[=]{on_ShowResource_triggered();});
    connect(ShowOutput,&QAction::triggered,this,[=]{on_ShowOutput_triggered();});
    connect(ShowTerminal,&QAction::triggered,this,[=]{on_ShowTerminal_triggered();});
    connect(ShowDebug,&QAction::triggered,this,[=]{on_ShowDebug_triggered();});
    connect(ShowReg,&QAction::triggered,this,[=]{on_ShowReg_triggered();});

    connect(ShowToolBar,&QAction::triggered,this,[=]{on_ShowToolBar_triggered();});
    connect(ShowStatusBar,&QAction::triggered,this,[=]{on_ShowStatusBar_triggered();});

    connect(LastWindow,&QAction::triggered,this,[=]{on_LastWindow_triggered();});
    connect(NextWindow,&QAction::triggered,this,[=]{on_NextWindow_triggered();});

}

void windowset::on_LastWindow_triggered(){

    if(EditString->size()==0){
        StatusBar->DetermineMessage("Don't Have Last Window");
        return;
    }

    if((*Position)>0){
        (*Position)--;
    }

    //if this file is opened,set it as current file
    for (int i=0;i<EditWidget->count();i++) {

        if(EditWidget->widget(i)->whatsThis()==EditString->at((*Position))){
            EditWidget->setCurrentIndex(i);
            StatusBar->DetermineMessage("Open Last Window Success");
            return;
        }

    }

    //if this file not open,load it
    QFileInfo *FileInfo = new QFileInfo(EditString->at((*Position)));
    if(FileInfo->isFile()){
        ProjectTree->loadFile(EditString->at((*Position)));
    }

}

void windowset::on_NextWindow_triggered(){

    if(EditString->size()==0){
        StatusBar->DetermineMessage("Don't Have Next Window");
        return;
    }

    if((*Position)<EditString->size()-1){
        (*Position)++;
    }

    //if this file is opened,set it as current file
    for (int i=0;i<EditWidget->count();i++) {
        if(EditWidget->widget(i)->whatsThis()==EditString->at((*Position))){
            EditWidget->setCurrentIndex(i);
            StatusBar->DetermineMessage("Open Next Window Success");
            return;
        }
    }

    //if this file not open,load it
    QFileInfo *FileInfo = new QFileInfo(EditString->at((*Position)));
    if(FileInfo->isFile()){
        ProjectTree->loadFile(EditString->at((*Position)));
    }

}

void windowset::on_ShowReg_triggered(){

    EditWidget->show();
    CentralWidget->setStyleSheet("");
    if(ShowReg->isChecked()){
        RegCalcuWidget->setVisible(true);
        ShowReg->setChecked(true);
        RegCalcu->setChecked(true);
        StatusBar->DetermineMessage("Display Reg Calculator");
    }
    else{
        RegCalcuWidget->setVisible(false);
        ShowReg->setChecked(false);
        RegCalcu->setChecked(false);
        StatusBar->DetermineMessage("Don't Dispaly Reg Calculator");
    }

}

void windowset::on_ShowResource_triggered(){

    if(ShowResource->isChecked()){
        Resource->setVisible(true);
        ShowResource->setChecked(true);
        StatusBar->DetermineMessage("Display Resource Window");
    }
    else{
        Resource->setVisible(false);
        ShowResource->setChecked(false);
        StatusBar->DetermineMessage("Don't Display Resource Window");
    }

}

void windowset::on_ShowOutput_triggered(){

    if(ShowOutput->isChecked()){
        Output->setVisible(true);
        ShowOutput->setChecked(true);
        StatusBar->DetermineMessage("Display Output Window");
    }
    else{
        Output->setVisible(false);
        ShowOutput->setChecked(false);
        StatusBar->DetermineMessage("Don't Display Output Window");
    }

}

void windowset::on_ShowTerminal_triggered(){

    if(ShowTerminal->isChecked()){
        Terminal->setVisible(true);
        ShowTerminal->setChecked(true);
        SerialPort->setChecked(true);
        StatusBar->DetermineMessage("Display Terminal Window");
    }
    else{
        Terminal->setVisible(false);
        ShowTerminal->setChecked(false);
        SerialPort->setChecked(false);
        StatusBar->DetermineMessage("Don't Display Terminal Window");
    }

}

void windowset::on_ShowDebug_triggered(){

    if(ShowDebug->isChecked()){
        Debug->setVisible(true);
        ShowDebug->setChecked(true);
        CmdDebug->setChecked(true);
        StatusBar->DetermineMessage("Display Debug Window");
    }
    else{
        Debug->setVisible(false);
        ShowDebug->setChecked(false);
        CmdDebug->setChecked(false);
        StatusBar->DetermineMessage("Don't Display Debug Window");
    }

}

void windowset::on_ShowToolBar_triggered(){

    if(ShowToolBar->isChecked()){
        FileToolBar->setVisible(true);
        EditToolBar->setVisible(true);
        CompileToolBar->setVisible(true);
        ToolsToolBar->setVisible(true);
        WindowBar->setVisible(true);
        ShowToolBar->setChecked(true);
        StatusBar->DetermineMessage("Display ToolBar");
    }
    else{
        FileToolBar->setVisible(false);
        EditToolBar->setVisible(false);
        CompileToolBar->setVisible(false);
        ToolsToolBar->setVisible(false);
        WindowBar->setVisible(false);
        ShowToolBar->setChecked(false);
        StatusBar->DetermineMessage("Don't Display ToolBar");
    }

}

void windowset::on_ShowStatusBar_triggered(){

    if(ShowStatusBar->isChecked()){
        Status->setVisible(true);
        ShowStatusBar->setChecked(true);
        StatusBar->DetermineMessage("Display StatusBar");
    }
    else{
        Status->setVisible(false);
        ShowStatusBar->setChecked(false);
        StatusBar->DetermineMessage("Don't Display StatusBar");
    }

}
