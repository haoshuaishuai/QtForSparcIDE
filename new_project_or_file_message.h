#ifndef NEWPROJECTORFILEMESSAGE_H
#define NEWPROJECTORFILEMESSAGE_H

#include <QWidget>

class newprojectorfile;
#include <new_project_or_file.h>

class newprojectorfilemessage : public QWidget
{
    Q_OBJECT
public:

    newprojectorfile * newitem;

public:

    explicit newprojectorfilemessage(QWidget *parent = nullptr,newprojectorfile * newitem = nullptr);

    void SetLayout();

};

#endif // NEWPROJECTORFILEMESSAGE_H
