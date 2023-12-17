#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>
#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>

class statusbar : public QWidget
{
    Q_OBJECT

public:

    QStatusBar *StatusBar;

    QProgressBar *ProgressBar;

    QLabel *Label;

    QLabel *PermanentLabel;

public:

    explicit statusbar(QWidget *parent = nullptr);

    void ShowMessage(QString message);

    void DetermineMessage(QString message);

    void UpdataProgress(QString LabelText,unsigned int Value);

    void InitWidget();



signals:

};

#endif // STATUSBAR_H
