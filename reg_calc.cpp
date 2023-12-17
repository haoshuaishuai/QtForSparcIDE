#include "reg_calc.h"

reg_calc::reg_calc(QWidget *parent)
    : QWidget{parent}{

    InitView();
    ConnectSignal();

}

void reg_calc::InitView(){

    RegCalcLayout = new QVBoxLayout();
    Caclulate = new QTableWidget(1,32);
    QStringList headers;
    for (int i = 31; i >= 0; --i){
        headers << QString::number(i);
    }
    Caclulate->setHorizontalHeaderLabels(headers);
    Caclulate->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    Caclulate->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    Caclulate->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    Caclulate->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    Caclulate->verticalHeader()->hide();
    Caclulate->horizontalHeader()->setStyleSheet(
        "QHeaderView::section{"
        "border-top:0px solid #E5E5E5;"
        "border-left:0px solid #E5E5E5;"
        "border-right:0.5px solid #E5E5E5;"
        "border-bottom: 0.5px solid #E5E5E5;"
        "background-color:white;"
        "padding:0px;"
        "}"
        );
    Caclulate->setStyleSheet("QTableView {selection-background-color: gray;}"
                               "QTableView {selection-color: white;}");
                               //"QTableView::item:focus {background: rgba(0, 0, 255, 0);}");
    Caclulate->setEditTriggers(QAbstractItemView::NoEditTriggers);
    Caclulate->setContextMenuPolicy(Qt::CustomContextMenu);
    QTableWidgetItems = new QList<QTableWidgetItem *>();
    for (int column = 0; column < 32; column++){
        QTableWidgetItem *item = new QTableWidgetItem("0");
        if ((4 <= column && column < 8) ||(12 <= column && column < 16) || (20 <= column && column < 24) || (28 <= column && column < 32)) {
            item->setBackgroundColor(QColor(245, 245, 245));
        } else {
            item->setBackgroundColor(Qt::white);
        }
        item->setTextAlignment(Qt::AlignCenter);
        Caclulate->setItem(0,column,item);
        QTableWidgetItems->append(item);
    }

    ValueWidget = new QWidget(this);
    ValueWidgetLayout = new QHBoxLayout();
    SetBit = new QPushButton("SetBit");
    SetReverse = new QPushButton("Reverse");
    HexLabel = new QLabel("Hex:");
    HexEdit = new QLineEdit();
    HexCopy = new QPushButton("Copy");
    OctLabel = new QLabel("Oct:");
    OctEdit = new QLineEdit();
    OctCopy = new QPushButton("Copy");
    BinLabel = new QLabel("Bin:");
    BinEdit = new QLineEdit();
    BinCopy = new QPushButton("Copy");
    ValueWidgetLayout->addWidget(SetBit);
    ValueWidgetLayout->addWidget(SetReverse);

    ValueWidgetLayout->addWidget(HexLabel);
    ValueWidgetLayout->addWidget(HexEdit);
    ValueWidgetLayout->addWidget(HexCopy);

    ValueWidgetLayout->addWidget(OctLabel);
    ValueWidgetLayout->addWidget(OctEdit);
    ValueWidgetLayout->addWidget(OctCopy);

    ValueWidgetLayout->addWidget(BinLabel);
    ValueWidgetLayout->addWidget(BinEdit);
    ValueWidgetLayout->addWidget(BinCopy);

    ValueWidgetLayout->setMargin(0);
    ValueWidget->setLayout(ValueWidgetLayout);

    RegCalcLayout->addWidget(Caclulate);
    RegCalcLayout->addWidget(ValueWidget);
    RegCalcLayout->setMargin(0);

    this->setLayout(RegCalcLayout);

    SetValue = new mywidget("SetValue",this);
    //SetValue->setWindowIcon(QIcon(":/new/prefix1/res/runFile"));
    SetValue->setWindowTitle("Set Value");
    SetValue->setWindowFlags(Qt::Dialog);
    SetValue->setWindowModality(Qt::ApplicationModal);

    SetValueLayout = new QVBoxLayout();
    SetValueSetting = new QGroupBox("Hex/Bin/Oct:");
    SetValueSettingLayout = new QGridLayout();
    SetValueCmd = new QLineEdit();
    Ok = new QPushButton("Active");
    validator = new QRegExpValidator(QRegExp("[x0-9A-Fa-f]+"), SetValueCmd);
    SetValueCmd->setValidator(validator);
    SetValueSettingLayout->addWidget(SetValueCmd,0,0);
    SetValueSettingLayout->addWidget(Ok,0,1);
    SetValueSetting->setLayout(SetValueSettingLayout);
    SetValueLayout->addWidget(SetValueSetting);
    SetValue->setLayout(SetValueLayout);
    RefreshResult(true);

}

void reg_calc::ConnectSignal(){

    connect(Caclulate,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(rightClickedSlot(QPoint)));

    connect(Ok, &QPushButton::clicked,this,[=]{on_Ok_triggered();});

    connect(SetValueCmd, &QLineEdit::textChanged,this, [&](const QString &text){
        QString input = SetValueCmd->text();
        if (input.startsWith("0x") || input.startsWith("0X")) {
            validator->setRegExp(QRegExp("[xbXB0-9a-fA-F]+"));
            SetValueCmd->setValidator(validator);
        } else if (input.startsWith("0b") || input.startsWith("0B")) {
            validator->setRegExp(QRegExp("[xbXB01]+"));
            SetValueCmd->setValidator(validator);
        } else {
            validator->setRegExp(QRegExp("[xbXB0-9]+"));
            SetValueCmd->setValidator(validator);
        }
    });

    QObject::connect(SetValueCmd, &QLineEdit::returnPressed,this, [&](){
        on_Ok_triggered();
    });

    connect(SetBit, &QPushButton::clicked,this,[=]{on_SetBit_triggered();});

    connect(SetReverse, &QPushButton::clicked,this,[=]{on_SetReverse_triggered();});

    connect(HexCopy, &QPushButton::clicked,this,[=]{
        QString text = HexEdit->text();
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(text);
    });

    connect(OctCopy, &QPushButton::clicked,this,[=]{
        QString text = OctEdit->text();
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(text);
    });

    connect(BinCopy, &QPushButton::clicked,this,[=]{
        QString text = BinEdit->text();
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(text);
    });

    connect(Caclulate, &QTableWidget::itemClicked, [=](QTableWidgetItem* item){

        if(item->text()=="0"){
            item->setText("1");
        }else{
            item->setText("0");
        }
        Caclulate->clearSelection();
        RefreshResult(true);

    });

    connect(Caclulate->horizontalHeader(), &QHeaderView::sectionClicked,[=](int index){
        Caclulate->clearSelection();
        RefreshResult(true);
    });

    connect(Caclulate->selectionModel(), &QItemSelectionModel::selectionChanged,[=](const QItemSelection &selected, const QItemSelection &deselected){
        QList<QTableWidgetItem*> items = Caclulate->selectedItems();
        if(items.size()>1){
            RefreshResult(false);
        }
    });
}

void reg_calc::rightClickedSlot(QPoint pos){

    SetValueCmd->clear();
    SetValueCmd->setFocus();
    SetValue->show();

}

void reg_calc::on_Ok_triggered(){

    SetValue->hide();
    QList<QTableWidgetItem*> items = Caclulate->selectedItems();
    std::reverse(items.begin(), items.end());
    QString Value = SetValueCmd->text();
    QString Hex = "";
    QString Bin = "";
    QString Oct = "";
    if(Value.indexOf("0x")!=-1 || Value.indexOf("0X")!=-1){
        if(Value.indexOf("0x")!=-1){
            Hex = Value.split("0x")[1];
        }
        else{
            Hex = Value.split("0X")[1];
        }
        if(Hex!=""){
            bool detr;
            int decimal = Hex.toUInt(&detr, 16);
            if (detr) {
                QString binaryString = QString::number(decimal, 2);
                QByteArray byteArray = binaryString.toUtf8();
                std::string stdString = byteArray.toStdString();
                const char* charArray = stdString.c_str();
                std::string str(charArray);
                std::reverse(str.begin(), str.end());
                const char* reversedCharArray = str.c_str();
                for (int i = 0; i < items.size(); i++) {
                    if(i<binaryString.length()){
                        items.at(i)->setText(QString(reversedCharArray[i]));
                    }else{
                        items.at(i)->setText("0");
                    }
                }
            }else{
                return;
            }
        }
    }else if(Value.indexOf("0b")!=-1 || Value.indexOf("0B")!=-1){
        if(Value.indexOf("0b")!=-1){
            Bin = Value.split("0b")[1];
        }
        else{
            Bin = Value.split("0B")[1];
        }
        if(Bin != ""){

            QString binaryString = Bin;
            QByteArray byteArray = binaryString.toUtf8();
            std::string stdString = byteArray.toStdString();
            const char* charArray = stdString.c_str();
            std::string str(charArray);
            std::reverse(str.begin(), str.end());
            const char* reversedCharArray = str.c_str();
            for (int i = 0; i < items.size(); i++) {
                if(i<binaryString.length()){
                    items.at(i)->setText(QString(reversedCharArray[i]));
                }else{
                    items.at(i)->setText("0");
                }
            }

        }
    }else {
        Oct = Value;
        if(Oct!=""){
            bool detr;
            int decimal = Oct.toInt(&detr);
            if (detr) {
                QString binaryString = QString::number(decimal, 2);
                QByteArray byteArray = binaryString.toUtf8();
                std::string stdString = byteArray.toStdString();
                const char* charArray = stdString.c_str();
                std::string str(charArray);
                std::reverse(str.begin(), str.end());
                const char* reversedCharArray = str.c_str();
                for (int i = 0; i < items.size(); i++) {
                    if(i<binaryString.length()){
                        items.at(i)->setText(QString(reversedCharArray[i]));
                    }else{
                        items.at(i)->setText("0");
                    }
                }
            }else{
                return;
            }
        }
    }

    Caclulate->clearSelection();
    RefreshResult(true);

}

void reg_calc::on_SetBit_triggered(){

    if(SetBit->text()=="SetBit"){
        for(int i = 0;i<QTableWidgetItems->size();i++){
            QTableWidgetItems->at(i)->setText("1");
        }
        SetBit->setText("ResetBit");
    }else {
        for(int i = 0;i<QTableWidgetItems->size();i++){
            QTableWidgetItems->at(i)->setText("0");
        }
        SetBit->setText("SetBit");
    }

    Caclulate->clearSelection();
    RefreshResult(true);

}

void reg_calc::on_SetReverse_triggered(){

    for(int i = 0;i<QTableWidgetItems->size();i++){
        if(QTableWidgetItems->at(i)->text()=="0"){
            QTableWidgetItems->at(i)->setText("1");
        }else{
            QTableWidgetItems->at(i)->setText("0");
        }
    }

    Caclulate->clearSelection();
    RefreshResult(true);

}

void reg_calc::RefreshResult(bool SelectAll){

    QString Value = "";
    if(SelectAll){
        for(int i = 0;i<QTableWidgetItems->size();i++){
            Value = Value+QTableWidgetItems->at(i)->text();
        }
    }else{
        QList<QTableWidgetItem*> items = Caclulate->selectedItems();
        for(int i = 0;i<items.size();i++){
            Value = Value+items.at(i)->text();
        }
    }

    bool ok;
    uint decimal = Value.toUInt(&ok, 2);
    if (ok) {
        QString octdecimal = QString::number(decimal);
        OctEdit->setText(octdecimal);
        QString hexdecimal = "0x"+QString::number(decimal, 16);
        HexEdit->setText(hexdecimal);
        QString bindecimal = "0b"+QString::number(decimal, 2);
        BinEdit->setText(bindecimal);
    } else {
        return;
    }

}

void reg_calc::resizeEvent(QResizeEvent *event){

    QSize size = event->size();

    if(Caclulate->horizontalScrollBar()->maximum()>0){
        this->setMaximumHeight(101);
    } else {
        this->setMaximumHeight(84);
    }

    QWidget::resizeEvent(event);

}
