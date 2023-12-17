#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QWidget>
#include <QAction>
#include <QTextEdit>
#include <QLineEdit>
#include <QProcess>
#include <QVBoxLayout>
#include <QMenu>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "mybutton.h"
#include "combobox.h"
#include "mywidget.h"
#include "mytextedit.h"
#include "status_bar.h"

class serial : public QObject
{
    Q_OBJECT
public:
    explicit serial(QObject *parent = nullptr,statusbar *StatusBar = nullptr);
    void ConnectSignal();
    void InitWidget();
    void RefreshSerial();
    void InitDebugPara();

public:

    mytextedit *SerialMessage;
    combobox * SendLine;
    statusbar *StatusBar;
    QPushButton *Send;
    MyButton *Open;
    MyButton *Clear;

    mywidget *OpenWidget;
    QVBoxLayout *OpenWidgetLayout;

    QGroupBox *SerialSetting;
    QGridLayout *LayoutSerialSetting;
    QLabel *SerialComtLabel;
    QLabel *SerialBaudtLabel;
    combobox * SerialCom;
    combobox * SerialBaud;
    QLabel *StopBitLabel;
    QComboBox *StopBit;
    QLabel *DataBitLabel;
    QComboBox *DataBit;
    QLabel *OddBitLabel;
    QComboBox *OddBit;
    QLabel *CheckLabel;
    QComboBox *CheckDtrOrRts;

    QGroupBox *TransSetting;
    QGridLayout *LayoutTransSetting;
    QLabel *TranLabel;
    QComboBox *TranHexOrAscii;

    QGroupBox *ReceiveSetting;
    QGridLayout *LayoutReceiveSetting;
    QLabel *RecLabel;
    QComboBox *RecHexOrAscii;

    QSerialPort *SerialPort;

signals:

public slots:

    void on_Open_Left_triggered();
    void on_Open_Right_triggered();
    void on_Send_Left_triggered();
    void on_Clear_Left_triggered();
    void on_SerialMessage_TextChanged(QString str);
    void ReceiveData();


};

#endif // SERIAL_H
