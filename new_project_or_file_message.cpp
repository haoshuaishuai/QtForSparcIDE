#include "new_project_or_file_message.h"
#include <new_project_or_file.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QTextDocument>
#include <QTextBlockFormat>

newprojectorfilemessage::newprojectorfilemessage(QWidget *parent,newprojectorfile * newitem) : QWidget(parent){

  this->newitem = newitem;

  this->setWindowFlags(Qt::Dialog);
  this->setWindowModality(Qt::WindowModal);

  QVBoxLayout *layoutall = new QVBoxLayout;
  QHBoxLayout *layoutbutton = new QHBoxLayout;
  QTextEdit *message =new QTextEdit;
  QWidget *buttonwidget = new QWidget(this);
  QPushButton *last = new QPushButton("< Back");
  QPushButton *next = new QPushButton("Next >");
  QPushButton *finish = new QPushButton("Finish");
  QPushButton *cancle = new QPushButton("Cancle");

  message->setEnabled(false);
  layoutbutton->addWidget(last);
  layoutbutton->addWidget(next);
  layoutbutton->addWidget(finish);
  layoutbutton->addWidget(cancle);
  buttonwidget->setLayout(layoutbutton);
  layoutall->addWidget(message);
  layoutall->addWidget(buttonwidget);

  this->setLayout(layoutall);
  next->setEnabled(false);

  connect(last, &QPushButton::clicked,this,[=]{

      newitem->setGeometry(this->frameGeometry().x()+9,this->frameGeometry().y()+30,this->width(),this->height());
      newitem->show();
      this->close();

  });

  connect(finish, &QPushButton::clicked,this,[=]{

      newitem->InitNewProject();
      this->close();
      newitem->close();

  });

  connect(cancle, &QPushButton::clicked,this,[=]{

      this->close();
      newitem->close();

  });

  message->setText(newitem->baseInformation);

  //set message word line margin
  QTextCursor cursor(message->textCursor());
  QTextBlockFormat format;
  format.setLineHeight(150, QTextBlockFormat::ProportionalHeight);
  cursor.select(QTextCursor::Document);
  cursor.mergeBlockFormat(format);

}
