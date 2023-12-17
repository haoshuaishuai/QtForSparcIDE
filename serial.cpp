#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QSettings>
#include <QKeyEvent>
#include "mytextedit.h"
#include "serial.h"
#include "status_bar.h"

serial::serial(QObject *parent,statusbar *StatusBar) : QObject{parent}{

    this->SerialPort = new QSerialPort(this);
    this->InitWidget();
    this->InitDebugPara();
    this->StatusBar = StatusBar;

}

//connect signals
void serial::ConnectSignal(){

    connect(Open, SIGNAL(leftButtonClicked()),this,SLOT(on_Open_Left_triggered()));
    connect(Open, SIGNAL(rightButtonClicked()),this,SLOT(on_Open_Right_triggered()));
    connect(Send, &QPushButton::clicked,this,[=]{on_Send_Left_triggered();});
    connect(Clear,SIGNAL(leftButtonClicked()),this,SLOT(on_Clear_Left_triggered()));
    connect(SendLine->lineEdit(), &QLineEdit::returnPressed, [=](){on_Send_Left_triggered();});
    connect(SerialMessage,SIGNAL(EditKeyPressEvent(QString)),this,SLOT(on_SerialMessage_TextChanged(QString)));

}

void serial::on_Open_Left_triggered(){

    if(Open->text()=="Open"){

        SerialPort->setPortName(SerialCom->currentText());
        if(SerialPort->open(QIODevice::ReadWrite)){

            this->SerialMessage->append(">>SDK Open Uart "+SerialPort->portName()+" Success\r\n");
            QTextCursor cursor = SerialMessage->textCursor();
            cursor.movePosition(QTextCursor::End);
            SerialMessage->setTextCursor(cursor);
            Open->setText("Close");

        }
        else{

            this->SerialMessage->append(">>SDK Open Uart "+SerialPort->portName()+" Failed\r\n");
            QTextCursor cursor = SerialMessage->textCursor();
            cursor.movePosition(QTextCursor::End);
            SerialMessage->setTextCursor(cursor);

        }

        SerialPort->setBaudRate(SerialBaud->currentText().toInt());

        switch (StopBit->currentIndex()){

            case 0: SerialPort->setStopBits(QSerialPort::OneStop); break;
            case 1: SerialPort->setStopBits(QSerialPort::TwoStop); break;
            case 2: SerialPort->setStopBits(QSerialPort::OneAndHalfStop); break;
            default:break;

        }

        switch (DataBit->currentIndex()){

            case 0: SerialPort->setDataBits(QSerialPort::Data8); break;
            case 1: SerialPort->setDataBits(QSerialPort::Data7); break;
            case 2: SerialPort->setDataBits(QSerialPort::Data6); break;
            case 3: SerialPort->setDataBits(QSerialPort::Data5); break;
            default:break;

        }

        switch (OddBit->currentIndex()){

            case 0: SerialPort->setParity(QSerialPort::NoParity);   break;
            case 1: SerialPort->setParity(QSerialPort::OddParity);  break;
            case 2: SerialPort->setParity(QSerialPort::EvenParity); break;
            case 3: SerialPort->setParity(QSerialPort::SpaceParity);break;
            case 4: SerialPort->setParity(QSerialPort::MarkParity); break;
            default:break;

        }

        switch (CheckDtrOrRts->currentIndex()){

            case 1: SerialPort->setDataTerminalReady(true);   break;
            case 2: SerialPort->setRequestToSend(true); break;
            case 3: SerialPort->setDataTerminalReady(true);SerialPort->setRequestToSend(true); break;
            default:break;

        }

        connect(SerialPort,SIGNAL(readyRead()),this,SLOT(ReceiveData()));

        StatusBar->DetermineMessage("Open Uart Success");

    }else{

        SerialPort->close();
        this->SerialMessage->append(">>SDK Close Uart "+SerialPort->portName()+" Success\r\n");
        QTextCursor cursor = SerialMessage->textCursor();
        cursor.movePosition(QTextCursor::End);
        SerialMessage->setTextCursor(cursor);
        Open->setText("Open");
        StatusBar->DetermineMessage("Close Uart Success");

    }

}

void serial::on_SerialMessage_TextChanged(QString string){


    if(SerialPort->isOpen()){
        switch (TranHexOrAscii->currentIndex()){

        case 0:{

            QByteArray str;
            str = string.toLocal8Bit();
            SerialPort->write(str);

        }
        break;

        case 1:{

            QString str =string;
            str.remove(QRegExp("\\s"));
            if(str.length()%2==1)
                str=str.remove(str.length()-1,1);
            QByteArray text=QByteArray::fromHex(str.toLatin1());
            SerialPort->write(text.data());

        }

        break;
        default:break;

        }

    }else{

        this->SerialMessage->append(">>Uart Not Open\r\n");
        QTextCursor cursor = SerialMessage->textCursor();
        cursor.movePosition(QTextCursor::End);
        SerialMessage->setTextCursor(cursor);

    }

}

void serial::on_Open_Right_triggered(){

    OpenWidget->setWindowModality(Qt::ApplicationModal);
    OpenWidget->show();
    RefreshSerial();

}

void serial::on_Send_Left_triggered(){

    if(SerialPort->isOpen()){
        switch (TranHexOrAscii->currentIndex()){

            case 0:{

                QByteArray str;
                str = SendLine->currentText().toLocal8Bit();
                SerialPort->write(str);

            }
            break;

            case 1:{

                QString str =SendLine->currentText();
                str.remove(QRegExp("\\s"));
                if(str.length()%2==1)
                    str=str.remove(str.length()-1,1);
                QByteArray text=QByteArray::fromHex(str.toLatin1());
                SerialPort->write(text.data());

            }

            break;
            default:break;

        }

        QString text = SendLine->currentText();
        if(text!=""){
            int index = SendLine->findText(text);
            if (index != -1) {
                SendLine->setCurrentIndex(index);
            } else {
                SendLine->addItem(text);
                SendLine->setCurrentIndex(SendLine->count() - 1);
            }
        }

    }else{

        this->SerialMessage->append(">>Uart Not Open\r\n");
        QTextCursor cursor = SerialMessage->textCursor();
        cursor.movePosition(QTextCursor::End);
        SerialMessage->setTextCursor(cursor);

    }

}

void serial::on_Clear_Left_triggered(){

    SerialMessage->clear();

}

void serial::RefreshSerial(){

    QString SerialPort =  SerialCom->currentText();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){

        for(int i=0;i<SerialCom->count();i++){

            if(info.portName()==SerialCom->itemText(i)){
                SerialCom->removeItem(i);
            }

        }
        SerialCom->addItem(info.portName());

    }
    SerialCom->setCurrentText(SerialPort);

}

void serial::ReceiveData(){

    QByteArray buf = SerialPort->readAll();

    if(!buf.isEmpty()){

        switch (RecHexOrAscii->currentIndex()) {
            case 0:{

                    QString myStrTemp = QString::fromLocal8Bit(buf);
                    this->SerialMessage->insertPlainText(myStrTemp);
                    QTextCursor cursor = SerialMessage->textCursor();
                    cursor.movePosition(QTextCursor::End);
                    SerialMessage->setTextCursor(cursor);

            }
            break;

            case 1:{

                    QDataStream out(&buf,QIODevice::ReadWrite);
                    while (!out.atEnd()){

                        qint8 outChar =0;
                        out>>outChar;
                        QString str =QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
                        this->SerialMessage->insertPlainText(str);

                    }

            }
            break;
        }

    }
}

void serial::InitWidget(){

    //Link BUtton Widget
    OpenWidget = new mywidget("OpenWidget",this);
    OpenWidget->setWindowIcon(QIcon(":/new/prefix1/res/openDebug"));
    OpenWidget->setWindowTitle("Serial Setting");
    OpenWidget ->setWindowFlags(Qt::Dialog);
    OpenWidget->setWindowModality(Qt::WindowModal);
    OpenWidgetLayout = new QVBoxLayout();

    SerialSetting = new QGroupBox("Base Setting");
    LayoutSerialSetting = new QGridLayout();
    SerialBaudtLabel = new QLabel("BaudRate:    ");
    SerialBaud = new combobox();
    SerialBaud->addItem("9600");
    SerialBaud->addItem("19200");
    SerialBaud->addItem("38400");
    SerialBaud->addItem("115200");
    SerialBaud->addItem("460800");
    SerialBaud->setCurrentText("115200");
    SerialBaud->setMinimumSize(180,20);
    SerialBaud->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutSerialSetting->addWidget(SerialBaudtLabel,0,0);
    LayoutSerialSetting->addWidget(SerialBaud,0,1);
    SerialComtLabel= new QLabel("SerialCom:      ");
    SerialCom = new combobox();
    SerialCom->setMinimumSize(180,20);
    SerialCom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutSerialSetting->addWidget(SerialComtLabel,1,0);
    LayoutSerialSetting->addWidget(SerialCom,1,1);
    StopBitLabel = new QLabel("StopBit: ");
    StopBit = new QComboBox();
    StopBit->addItem("1");
    StopBit->addItem("2");
    StopBit->addItem("1.5");
    StopBit->setMinimumSize(180,20);
    StopBit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutSerialSetting->addWidget(StopBitLabel,2,0);
    LayoutSerialSetting->addWidget(StopBit,2,1);
    DataBitLabel= new QLabel("DataBit:  ");
    DataBit = new QComboBox();
    DataBit->addItem("8");
    DataBit->addItem("7");
    DataBit->addItem("6");
    DataBit->addItem("5");
    DataBit->setMinimumSize(180,20);
    DataBit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutSerialSetting->addWidget(DataBitLabel,3,0);
    LayoutSerialSetting->addWidget(DataBit,3,1);
    OddBitLabel= new QLabel("OddBit:  ");
    OddBit = new QComboBox();
    OddBit->addItem("None");
    OddBit->addItem("Odd");
    OddBit->addItem("Even");
    OddBit->addItem("Space");
    OddBit->addItem("Mark");
    OddBit->setMinimumSize(180,20);
    OddBit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    LayoutSerialSetting->addWidget(OddBitLabel,4,0);
    LayoutSerialSetting->addWidget(OddBit,4,1);
    CheckLabel= new QLabel("Check:  ");
    CheckDtrOrRts = new QComboBox();
    CheckDtrOrRts->addItem("None");
    CheckDtrOrRts->addItem("DtrCheck");
    CheckDtrOrRts->addItem("RtsCheck");
    CheckDtrOrRts->addItem("DtrCheck & RtsCheck");
    LayoutSerialSetting->addWidget(CheckLabel,5,0);
    LayoutSerialSetting->addWidget(CheckDtrOrRts,5,1);
    SerialSetting->setLayout(LayoutSerialSetting);
    RefreshSerial();

    TransSetting = new QGroupBox("Translate Setting");
    LayoutTransSetting = new QGridLayout();
    TranLabel= new QLabel("HexOrAscii:     ");
    TranHexOrAscii = new QComboBox();
    TranHexOrAscii->setMinimumSize(180,20);
    TranHexOrAscii->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    TranHexOrAscii->addItem("Ascii");
    TranHexOrAscii->addItem("Hex");
    LayoutTransSetting->addWidget(TranLabel,0,0);
    LayoutTransSetting->addWidget(TranHexOrAscii,0,1);
    TransSetting->setLayout(LayoutTransSetting);

    ReceiveSetting = new QGroupBox("Receive Setting");
    LayoutReceiveSetting = new QGridLayout();
    RecLabel= new QLabel("HexOrAscii:     ");
    RecHexOrAscii = new QComboBox();
    RecHexOrAscii->setMinimumSize(180,20);
    RecHexOrAscii->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    RecHexOrAscii->addItem("Ascii");
    RecHexOrAscii->addItem("Hex");
    LayoutReceiveSetting->addWidget(RecLabel,0,0);
    LayoutReceiveSetting->addWidget(RecHexOrAscii,0,1);
    ReceiveSetting->setLayout(LayoutReceiveSetting);
    OpenWidgetLayout->addWidget(SerialSetting);
    OpenWidgetLayout->addWidget(TransSetting);
    OpenWidgetLayout->addWidget(ReceiveSetting);
    OpenWidget->setLayout(OpenWidgetLayout);

}

void serial::InitDebugPara() {

    QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
    QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

    GlobalSetting.beginGroup("UartSetting");
    QString SerialBaudSelect = GlobalSetting.value("SerialBaudSelect").toString();
    if(SerialBaudSelect!="")
        SerialBaud->setCurrentText(SerialBaudSelect);

    QString SerialComSelect   = GlobalSetting.value("SerialComSelect").toString();
    if(SerialComSelect!="")
        SerialCom->setCurrentText(SerialComSelect);

    QString StopBitSelect  = GlobalSetting.value("StopBitSelect").toString();
    if(StopBitSelect!="")
        StopBit->setCurrentText(StopBitSelect);

    QString DataBitSelect = GlobalSetting.value("DataBitSelect").toString();
    if(DataBitSelect!="")
        DataBit->setCurrentText(DataBitSelect);

    QString OddBitSelect = GlobalSetting.value("OddBitSelect").toString();
    if(OddBitSelect!="")
        OddBit->setCurrentText(OddBitSelect);

    QString CheckDtrOrRtsSelect = GlobalSetting.value("CheckDtrOrRtsSelect").toString();
    if(CheckDtrOrRtsSelect!="")
        CheckDtrOrRts->setCurrentText(CheckDtrOrRtsSelect);

    QString TranHexOrAsciiSelect = GlobalSetting.value("TranHexOrAsciiSelect").toString();
    if(TranHexOrAsciiSelect!="")
        TranHexOrAscii->setCurrentText(TranHexOrAsciiSelect);

    QString RecHexOrAsciiSelect = GlobalSetting.value("RecHexOrAsciiSelect").toString();
    if(RecHexOrAsciiSelect!="")
        RecHexOrAscii->setCurrentText(RecHexOrAsciiSelect);

    GlobalSetting.endGroup();

}
