#ifndef REG_CALC_H
#define REG_CALC_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QScrollBar>
#include <qDebug>
#include <QInputDialog>
#include <QGroupBox>
#include <QLabel>
#include <QtCore>
#include <QGuiApplication>
#include <QClipboard>
#include <QMouseEvent>
#include <QResizeEvent>
#include "mywidget.h"

class reg_calc : public QWidget
{
    Q_OBJECT
public:
    explicit reg_calc(QWidget *parent = nullptr);

    void InitView();

    void ConnectSignal();

    void on_Ok_triggered();

    void on_SetBit_triggered();

    void on_SetReverse_triggered();

    void RefreshResult(bool SelectAll);

protected:

    void resizeEvent(QResizeEvent *event) override;

public:
    QVBoxLayout *RegCalcLayout;

    QTableWidget *Caclulate;

    //Run Widget
    mywidget *SetValue;

    QVBoxLayout *SetValueLayout;

    QGroupBox *SetValueSetting;

    QGridLayout *SetValueSettingLayout;

    QLineEdit *SetValueCmd;

    QWidget *ValueWidget;

    QHBoxLayout *ValueWidgetLayout;

    QPushButton *Ok;

    QRegExpValidator* validator;

    QPushButton *SetBit;

    QPushButton *SetReverse;

    QLabel *HexLabel;

    QLineEdit *HexEdit;

    QPushButton *HexCopy;

    QLabel *OctLabel;

    QLineEdit *OctEdit;

    QPushButton *OctCopy;

    QLabel *BinLabel;

    QLineEdit *BinEdit;

    QPushButton *BinCopy;

    QList<QTableWidgetItem *> *QTableWidgetItems;

signals:

private slots:

    void rightClickedSlot(QPoint pos);

};

#endif // REG_CALC_H
