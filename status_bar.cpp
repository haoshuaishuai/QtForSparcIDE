#include <QProgressBar>
#include <QLabel>
#include <QDateTime>
#include <QDebug>
#include "status_bar.h"

statusbar::statusbar(QWidget *parent) : QWidget(parent){

}

void statusbar::ShowMessage(QString message){

    StatusBar->showMessage(message,3000);

}

void statusbar::DetermineMessage(QString message){

    QString Time = QDateTime::currentDateTime().toString("hh:mm:ss");
    PermanentLabel->setText(Time+" - "+message);

}

void statusbar::UpdataProgress(QString LabelText,unsigned int Value){

    Label->setText(LabelText);
    ProgressBar->setValue(Value);

}

void statusbar::InitWidget(){

    ProgressBar = new QProgressBar();
    ProgressBar->setFixedWidth(200);
    ProgressBar->setRange(0,100);
    ProgressBar->setValue(0);
    ProgressBar->setTextVisible(false);

    Label= new QLabel();
    Label->setAlignment(Qt::AlignRight);
    Label->setFixedWidth(100);

    PermanentLabel = new QLabel();
    PermanentLabel->setAlignment(Qt::AlignLeft);
    //PermanentLabel->setFixedWidth(100);

    StatusBar->addPermanentWidget(PermanentLabel,1);
    StatusBar->addPermanentWidget(Label);
    StatusBar->addPermanentWidget(ProgressBar);

    ProgressBar->setVisible(false);
    Label->setVisible(false);

}

