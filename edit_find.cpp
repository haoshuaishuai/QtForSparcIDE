#include "qsci_editor.h"
#include "edit_find.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include <QFormLayout>
#include <QSizePolicy>
#include <QButtonGroup>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>

//class lockerbutton
LockerButton::LockerButton(QWidget* parent)
    : QPushButton(parent)
{
    m_imageLabel = new QLabel;
    m_imageLabel->setFixedWidth(20);
    m_imageLabel->setFixedHeight(20);
    m_imageLabel->setScaledContents(true);
    m_imageLabel->setStyleSheet("QLabel{background-color:transparent;}");

    m_textLabel = new QLabel;
    m_textLabel->setStyleSheet("QLabel{background-color:transparent;}");

    QPalette palette = this->palette();
    palette.setColor(QPalette::ButtonText, QColor(255, 255, 255, 0));
    this->setPalette(palette);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_imageLabel);
    mainLayout->addWidget(m_textLabel);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
}

void LockerButton::SetImageLabel(const QPixmap &pixmap){
    m_imageLabel->setPixmap(pixmap);
}

void LockerButton::SetTextLabel(QString text){
    m_textLabel->setText(text);
    this->setText("START:"+text);
}

QLabel* LockerButton::GetImageHandle(){
    return m_imageLabel;
}

QLabel* LockerButton::GetTextHandle(){

    return m_textLabel;
}

editfind::editfind(QWidget *parent,treemenu *ProjectTree) : QWidget(parent){

    this->ProjectTree = ProjectTree;

    this->FindFile = new QStringList;

    this->FindFileString = new QList<QStringList *>;

    this->FindEditList = new QList<qscieditor *>;

    this->setWindowTitle("Find/Replace");
    this->setWindowIcon(QIcon(":/new/prefix1/res/searchFiles"));

}

void editfind::ConnectSignals(){

    connect(Advanced, &QPushButton::clicked,this,[=]{

        if(layoutall->count()==6){

            layoutall->addWidget(AllWidget);
            this->setFixedHeight(480);
            this->setLayout(layoutall);

        }else{

            layoutall->removeWidget(AllWidget);
            AllWidget->setParent(NULL);
            this->setFixedHeight(352);
            this->setLayout(layoutall);

        }

    });

    connect(All, &QRadioButton::toggled,this,[=]{

        bool SelectStart  = this->All->isChecked();
        if(SelectStart){
            for(int i=0;i<EditList->size();i++){
                if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
                    EditList->at(i)->setCursorPosition(0,0);
                    break;
                }
            }
        }

    });

    connect(FindNext, &QPushButton::clicked,this,[=]{

        Find(true);
        TransformFind();

    });

    connect(FindPrevious, &QPushButton::clicked,this,[=]{

        Find(false);
        TransformFind();

    });

    connect(AdvancedFind, &QPushButton::clicked,this,[=]{

        bool SelectedCase = this->CaseSens->isChecked();
        bool SelectedWhole = this->WholeWord->isChecked();
        bool SelectedRegular = this->Regular->isChecked();
        QString FindString = FindEdit->currentText();
        QString Filter = PatternEdit->currentText().split("*").at(1);
        bool DoFilter = false;
        if(Filter == "."){
            Filter = "";
            DoFilter = true;
        }else{
            DoFilter = false;
        }

        if(this->ThisFile->isChecked()){

            //clear FindFile/FindFileString List
            FindFile->clear();
            FindFileString->clear();

            FindEditList->clear();
            for(int i=0;i<EditList->size();i++){
                FindEditList->append(EditList->at(i));
            }

            //search scope = this file
            for(int i=0;i<EditList->size();i++){

                QString FilePath = EditWidget->currentWidget()->whatsThis();
                if(EditList->at(i)->whatsThis( ) == FilePath && (FilePath.indexOf(Filter)==(FilePath.length()-2) || DoFilter)){

                    QStringList *StringInFile = new QStringList;
                    int FirstLine = 0;
                    int FindNum = 0;
                    int LastLine=-1,ThisLine=0,Pos=0;
                    EditList->at(i)->setCursorPosition(0,0);

                    bool Ok = EditList->at(i)->findFirst(FindString,SelectedRegular,SelectedCase,SelectedWhole,false,true);
                    while(Ok){

                        FindNum++;
                        EditList->at(i)->getCursorPosition(&ThisLine,&Pos);
                        if(FindNum==1){
                            FirstLine = ThisLine;
                        }
                        if(ThisLine!=LastLine){
                            QString AppendString = QString::number(ThisLine+1) +"."+EditList->at(i)->text(ThisLine);
                            StringInFile->append(AppendString);
                            LastLine = ThisLine;
                        }
                        Ok = EditList->at(i)->findNext();

                    }

                    FindFileString->append(StringInFile);
                    FindFile->append(EditList->at(i)->whatsThis( )+"("+QString::number(FindNum)+")");
                    EditList->at(i)->setCursorPosition(FirstLine,0);
                    break;

                }

            }
            this->SetUpUI();

        }else if(OpenFile->isChecked()){

            //clear FindFile/FindFileString List
            FindFile->clear();
            FindFileString->clear();

            FindEditList->clear();
            for(int i=0;i<EditList->size();i++){
                FindEditList->append(EditList->at(i));
            }

            //search scope = opened file
            for(int j=0;j<EditWidget->count();j++){

                for(int i=0;i<EditList->size();i++){

                    QString FilePath = EditWidget->widget(j)->whatsThis();
                    if(EditList->at(i)->whatsThis( )== FilePath && (FilePath.indexOf(Filter)==(FilePath.length()-2) || DoFilter)){

                        QStringList *StringInFile = new QStringList;
                        int FirstLine = 0;
                        int FindNum = 0;
                        int LastLine = -1,ThisLine = 0,Pos = 0;
                        EditList->at(i)->setCursorPosition(0,0);

                        bool Ok = EditList->at(i)->findFirst(FindString,SelectedRegular,SelectedCase,SelectedWhole,false,true);
                        while(Ok){

                            FindNum++;
                            EditList->at(i)->getCursorPosition(&ThisLine,&Pos);
                            if(FindNum==1){
                                FirstLine = ThisLine;
                            }
                            if(ThisLine!=LastLine){
                                QString AppendString = QString::number(ThisLine+1) +"."+EditList->at(i)->text(ThisLine);
                                StringInFile->append(AppendString);
                                LastLine = ThisLine;
                            }
                            Ok = EditList->at(i)->findNext();

                        }

                        FindFileString->append(StringInFile);
                        FindFile->append(EditList->at(i)->whatsThis( )+"("+QString::number(FindNum)+")");

                        EditList->at(i)->setCursorPosition(FirstLine,0);
                        break;

                    }
                }

            }
            this->SetUpUI();


        }else{

            if(!ProjectTree->root){
                return;
            }

            FindEditList->clear();

            QTreeWidgetItemIterator Iterator(ProjectTree->root);
            while(*Iterator){

                QFileInfo *FileInfo = new QFileInfo((*Iterator)->toolTip(0));
                QString FilePath = FileInfo->filePath();

                if(FileInfo->isFile() && (FilePath.indexOf(Filter)==(FilePath.length()-2) || DoFilter)){

                    QFile file(FilePath);

                    if (!file.open(QFile::ReadOnly)) {

                        QMessageBox::warning(this, tr("Application"),tr("Can not read file %1:\n%2.").arg(FilePath).arg(file.errorString()));
                        return;

                    }

                    //create file edit
                    QTextStream in(&file);

                    QApplication::setOverrideCursor(Qt::WaitCursor);

                    qscieditor *Edit = new qscieditor(this);

                    Edit->setWhatsThis(FilePath);

                    Edit->setText(in.readAll());

                    QApplication::restoreOverrideCursor();

                    FindEditList->append(Edit);

                }
                ++Iterator;

            }

            //clear FindFile/FindFileString List
            FindFile->clear();
            FindFileString->clear();

            //search scope = this project
            for(int i=0;i<FindEditList->size();i++){

                    QStringList *StringInFile = new QStringList;
                    int FirstLine = 0;
                    int FindNum = 0;
                    int LastLine = -1,ThisLine = 0,Pos = 0;
                    FindEditList->at(i)->setCursorPosition(0,0);

                    bool Ok = FindEditList->at(i)->findFirst(FindString,SelectedRegular,SelectedCase,SelectedWhole,false,true);
                    while(Ok){

                        FindNum++;
                        FindEditList->at(i)->getCursorPosition(&ThisLine,&Pos);

                        if(FindNum==1){

                            FirstLine = ThisLine;

                        }

                        if(ThisLine!=LastLine){

                            QString AppendString = QString::number(ThisLine+1) +"."+FindEditList->at(i)->text(ThisLine);
                            StringInFile->append(AppendString);
                            LastLine = ThisLine;

                        }

                        Ok = FindEditList->at(i)->findNext();

                    }

                    FindFileString->append(StringInFile);
                    FindFile->append(FindEditList->at(i)->whatsThis( )+"("+QString::number(FindNum)+")");

                    for(int j=0;j<EditList->size();j++){
                        if(EditList->at(j)->whatsThis() == FindEditList->at(i)->whatsThis())
                            EditList->at(j)->setCursorPosition(FirstLine,0);
                    }

             }

            this->SetUpUI();

        }

        TransformFind();

        //show find message widget
        MessageWidget->setCurrentIndex(1);
        Output->raise();

    });

    connect(ReplaceFind, &QPushButton::clicked,this,[=]{

        if(Find(true)){
            ReplaceWorld();
        }
        TransformFind();

    });

    connect(Replace, &QPushButton::clicked,this,[=]{

        ReplaceWorld();
        TransformReplace();

    });

    connect(ReplaceAll, &QPushButton::clicked,this,[=]{

        int position,index;

        for(int j=0;j<EditList->size();j++){
            if(EditList->at(j)->whatsThis() == EditWidget->currentWidget()->whatsThis()){
                EditList->at(j)->getCursorPosition(&position,&index);;
            }
        }

        while(Find(true)){
            ReplaceWorld();
        }

        for(int j=0;j<EditList->size();j++){
            if(EditList->at(j)->whatsThis() == EditWidget->currentWidget()->whatsThis())
                EditList->at(j)->setCursorPosition(position,index);
        }

        TransformReplace();

    });

    connect(FindEdit->lineEdit(), &QLineEdit::returnPressed, [=](){

        Find(true);
        EnterTransformFind();

    });

}

void editfind::TransformFind(){

    QString FindString = FindEdit->currentText();

    if(FindEdit->count()==10){
        FindEdit->removeItem(9);
    }

    if(FindEdit->itemText(0)!=FindString){
        FindEdit->insertItem(0,FindString);
        FindEdit->setCurrentIndex(0);
    }

}

void editfind::EnterTransformFind(){

    QString FindString = FindEdit->currentText();

    if(FindEdit->count()==10){
        FindEdit->removeItem(9);
    }

    FindEdit->removeItem(FindEdit->count()-1);

    if(FindEdit->itemText(0)!=FindString){
        FindEdit->insertItem(0,FindString);
    }

    FindEdit->setCurrentIndex(0);

}

void editfind::TransformReplace(){

    QString ReplaceString = ReplaceEdit->currentText();

    if(ReplaceEdit->count()==10){
        ReplaceEdit->removeItem(9);
    }

    if(ReplaceEdit->itemText(0)!=ReplaceString){
        ReplaceEdit->insertItem(0,ReplaceString);
    }

}

bool editfind::Find(bool Next){

    bool SelectStart  = this->All->isChecked();
    bool SelectFoward = Next?this->Forward->isChecked():!this->Forward->isChecked();
    bool SelectedCase = this->CaseSens->isChecked();
    bool SelectedWrap = this->WrapSerach->isChecked();
    bool SelectedWhole = this->WholeWord->isChecked();
    bool SelectedRegular = this->Regular->isChecked();
    QString FindString = FindEdit->currentText();
    QString FindFile = EditWidget->currentWidget()->whatsThis();

    if(SelectStart==OldSelectStart && SelectedCase==OldSelectedCase && SelectedWrap==OldSelectedWrap
       && SelectedWhole==OldSelectedWhole && SelectedRegular==OldSelectedRegular
       && FindString==OldFindString && FindFile==OldFindFile){
        NewCursor = false;
    }else{
        NewCursor = true;
        OldSelectStart=SelectStart;
        OldSelectedCase=SelectedCase;
        OldSelectedWrap=SelectedWrap;
        OldSelectedWhole=SelectedWhole;
        OldSelectedRegular=SelectedRegular;
        OldFindString=FindString;
        OldFindFile = FindFile;
    }

    //set find position
    if(NewCursor && SelectStart && EditWidget->currentWidget()){

            for(int i=0;i<EditList->size();i++){
                if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
                    EditList->at(i)->setCursorPosition(0,0);
                    break;
                }
            }

    }

    //search scope = this file
    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            int ThisLine=0,Pos=0;
            EditList->at(i)->getCursorPosition(&ThisLine,&Pos);
            if(!SelectFoward){
                 Pos = Pos-EditList->at(i)->selectedText().size();
            }
            return EditList->at(i)->findFirst(FindString,SelectedRegular,SelectedCase,SelectedWhole,SelectedWrap,SelectFoward,ThisLine,Pos);
            break;
        }
    }

    return false;

}

void editfind::ReplaceWorld(){

    QString ReplaceString = ReplaceEdit->currentText();
    QString FindString = FindEdit->currentText();
    bool SelectedCase = this->CaseSens->isChecked();
    bool SelectedRegular = this->Regular->isChecked();

    if(SelectedRegular){

        for(int i=0;i<EditList->size();i++){
            if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
                    if(EditList->at(i)->hasSelectedText()){

                        EditList->at(i)->replaceSelectedText(ReplaceString);

                    }
            }
        }


    }else{

        //search scope = this file
        for(int i=0;i<EditList->size();i++){
            if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
                 if(!SelectedCase){
                    if(EditList->at(i)->hasSelectedText() && !FindString.compare(EditList->at(i)->selectedText(),Qt::CaseInsensitive)){

                        EditList->at(i)->replace(ReplaceString);

                    }
                 }else{
                    if(EditList->at(i)->hasSelectedText() && !FindString.compare(EditList->at(i)->selectedText(),Qt::CaseSensitive)){

                        EditList->at(i)->replaceSelectedText(ReplaceString);

                    }
                 }
            }
        }

    }

}

void editfind::InitWidget(){

    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    layoutall = new QVBoxLayout;

    //Find
    FindWidget = new QWidget(this);
    LayoutFind = new QHBoxLayout;
    FindLabel = new QLabel("Find:        ");
    FindEdit =new QComboBox;
    FindEdit->setDuplicatesEnabled(true);
    FindEdit->setEditable(true);
    FindEdit->setMinimumSize(320,25);
    LayoutFind->addWidget(FindLabel);
    LayoutFind->addWidget(FindEdit);
    FindWidget->setLayout(LayoutFind);

    //Replace with
    ReplaceWidget = new QWidget(this);
    LayoutReplace = new QHBoxLayout;
    ReplaceLabel = new QLabel("Replace with:");
    ReplaceEdit =new QComboBox;
    ReplaceEdit->setEditable(true);
    ReplaceEdit->setMinimumSize(320,25);
    LayoutReplace->addWidget(ReplaceLabel);
    LayoutReplace->addWidget(ReplaceEdit);
    ReplaceWidget->setLayout(LayoutReplace);


    //Direction
    DirectionGroup = new QGroupBox("Direction");
    LayoutDirectionGroup = new QHBoxLayout;
    Forward = new QRadioButton("Forward");
    Backward = new QRadioButton("Backward");
    Forward->setChecked(true);
    LayoutDirectionGroup->addWidget(Forward);
    LayoutDirectionGroup->addWidget(Backward);
    DirectionGroup->setLayout(LayoutDirectionGroup);

    //Scope
    ScopeGroup = new QGroupBox("Start Position");
    LayoutScopeGroup = new QHBoxLayout;
    All = new QRadioButton("Begin");
    SelectedLines = new QRadioButton("Cursor");
    SelectedLines->setChecked(true);
    LayoutScopeGroup->addWidget(All);
    LayoutScopeGroup->addWidget(SelectedLines);
    ScopeGroup->setLayout(LayoutScopeGroup);

    //Direction & Scope
    DirScoWidget = new QWidget(this);
    DirSco = new QHBoxLayout;
    DirSco->addWidget(DirectionGroup);
    DirSco->addWidget(ScopeGroup);
    DirScoWidget->setLayout(DirSco);

    //option
    OptionGroup = new QGroupBox("Option");
    LayoutOptionGroup = new QHBoxLayout;
    CaseSens = new QCheckBox("Case Senstive");
    WrapSerach = new QCheckBox("Wrape Serach");
    WholeWord = new QCheckBox("Whole Word");
    Regular = new QCheckBox("Use Regular");
    WrapSerach->setChecked(true);
    LayoutOptionGroup->addWidget(CaseSens);
    LayoutOptionGroup->addWidget(WrapSerach);
    LayoutOptionGroup->addWidget(WholeWord);
    LayoutOptionGroup->addWidget(Regular);
    OptionGroup->setLayout(LayoutOptionGroup);
    OptionWidget = new QWidget(this);
    Option = new QHBoxLayout;
    Option->addWidget(OptionGroup);
    OptionWidget->setLayout(Option);

    //find button
    buttonwidget1 = new QWidget(this);
    layoutbutton1 = new QHBoxLayout;
    FindPrevious = new QPushButton("<<Find Previous");
    FindNext = new QPushButton("Find Next>>");
    ReplaceFind = new QPushButton("Replace/Find");
    layoutbutton1->addWidget(FindPrevious);
    layoutbutton1->addWidget(FindNext);
    layoutbutton1->addWidget(ReplaceFind);
    buttonwidget1->setLayout(layoutbutton1);

    //replace button
    buttonwidget2 = new QWidget(this);
    layoutbutton2 = new QHBoxLayout;
    Replace = new QPushButton("Replace");
    ReplaceAll = new QPushButton("Replace All");
    Advanced = new QPushButton("Advanced...");
    layoutbutton2->addWidget(Replace);
    layoutbutton2->addWidget(ReplaceAll);
    layoutbutton2->addWidget(Advanced);
    buttonwidget2->setLayout(layoutbutton2);

    //scope option
    AdvancedScopeGroup = new QWidget();
    LayoutScopGroup = new QHBoxLayout;
    ScopeLabel = new QLabel("File Scope:");
    ThisFile = new QRadioButton("This File");
    OpenFile = new QRadioButton("Opened Files");
    OpenProject = new QRadioButton("Project Files");
    ThisFile->setChecked(true);
    LayoutScopGroup->addWidget(ScopeLabel);
    LayoutScopGroup->addWidget(ThisFile);
    LayoutScopGroup->addWidget(OpenFile);
    LayoutScopGroup->addWidget(OpenProject);
    AdvancedScopeGroup->setLayout(LayoutScopGroup);

    //pattern option
    PatternGroup = new QWidget();
    LayoutPatternGroup = new QHBoxLayout;
    PatternLabel = new QLabel("File Pattern:");
    PatternEdit = new QComboBox();
    PatternEdit->addItem("*.*");
    PatternEdit->addItem("*.c");
    PatternEdit->addItem("*.h");
    PatternEdit->addItem("*.s");
    PatternEdit->addItem("*.C");
    PatternEdit->addItem("*.H");
    PatternEdit->addItem("*.S");
    PatternEdit->setMinimumSize(200,20);
    AdvancedFind = new QPushButton("Find");
    AdvancedFind->setMinimumSize(20,22);
    LayoutPatternGroup->addWidget(PatternLabel);
    LayoutPatternGroup->addWidget(PatternEdit);
    LayoutPatternGroup->addWidget(AdvancedFind);
    PatternGroup->setLayout(LayoutPatternGroup);

    //all advanced layout
    AdvancedGroup = new QGroupBox("Advanced Option");
    AdvancedLayout = new QVBoxLayout;
    AdvancedLayout->addWidget(AdvancedScopeGroup);
    AdvancedLayout->addWidget(PatternGroup);
    AdvancedGroup->setLayout(AdvancedLayout);

    AllWidget = new QWidget();
    AllLayout = new QHBoxLayout;
    AllLayout->addWidget(AdvancedGroup);
    AllWidget->setLayout(AllLayout);

    //All layout
    layoutall->addWidget(FindWidget);
    layoutall->addWidget(ReplaceWidget);
    layoutall->addWidget(DirScoWidget);
    layoutall->addWidget(OptionWidget);
    layoutall->addWidget(buttonwidget1);
    layoutall->addWidget(buttonwidget2);

    this->setFixedWidth(446);
    this->setFixedHeight(352);
    this->setLayout(layoutall);

}

class Mybutt : public QPushButton
{
public:
    Mybutt(const QString &text, QWidget *parent = nullptr)
        : QPushButton(parent)
    {
        QLabel *m_imageLabel = new QLabel;
        m_imageLabel->setFixedWidth(20);
        m_imageLabel->setScaledContents(true);
        m_imageLabel->setStyleSheet("QLabel{background-color:transparent;}");

        QLabel * m_textLabel = new QLabel;
        m_textLabel->setStyleSheet("QLabel{background-color:transparent;}");
        m_textLabel->setText(text);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->addWidget(m_imageLabel);
        mainLayout->addWidget(m_textLabel);
        mainLayout->setMargin(0);
        mainLayout->setSpacing(0);
        this->setLayout(mainLayout);
    }
};


void editfind::SetUpUI(){

    QVBoxLayout* vlayout = new QVBoxLayout;

    for(int i = 0;i<FindFile->size();i++){

        if(FindFileString->at(i)->size()==0){
            continue;
        }

        //set button base form
        LockerButton * FindFileButton = new LockerButton;
        FindFileButton->setObjectName("LockerButton");
        FindFileButton->SetTextLabel(FindFile->at(i));
        FindFileButton->SetImageLabel(QPixmap(":/new/prefix1/res/right.png"));
        FindFileButton->setStyleSheet("#LockerButton{background-color:transparent}"
        "#LockerButton:hover{background-color:rgba(195,195,195,0.4)}"
        "#LockerButton:pressed{background-color:rgba(127,127,127,0.4)}");

        //set button word fold
        QLabel* sizeLabel = FindFileButton->GetTextHandle();
        sizeLabel->setStyleSheet("QLabel{color:rgba(0,0,0,1)}");
        sizeLabel->setFont(QFont("Microsoft YaHei", 9, QFont::Black));
        FindFileButton->setFont(QFont("Microsoft YaHei", 9, QFont::Black));
        FindFileButton->SetImageLabel(QPixmap(":/new/prefix1/res/right.png"));
        vlayout->addWidget(FindFileButton);

        QWidget *FindButtonWidget = new QWidget();
        QVBoxLayout* FindButtonLayout = new QVBoxLayout;
        FindButtonLayout->setMargin(0);
        FindButtonLayout->setSpacing(0);

        //set use layout
        for(int j=0;j<FindFileString->at(i)->size();j++){

            //set use button
            LockerButton * FindStringButton = new LockerButton;
            FindStringButton->setObjectName("LockerButton");
            QString Text = FindFileString->at(i)->at(j);
            FindStringButton->SetTextLabel(Text.split("\n").at(0));
            FindStringButton->setStyleSheet("#LockerButton{background-color:transparent}"
            "#LockerButton:hover{background-color:rgba(195,195,195,0.4)}"
            "#LockerButton:pressed{background-color:rgba(127,127,127,0.4)}");

            QLabel* FindLabel = FindStringButton->GetTextHandle();
            //FindLabel->setStyleSheet("QLabel{color:rgba(183,71,42,1)}");
            FindLabel->setFont(QFont("Microsoft YaHei", 9, QFont::Normal));
            FindStringButton->setFont(QFont("Microsoft YaHei", 9, QFont::Normal));
            FindButtonLayout->addWidget(FindStringButton);

            connect(FindStringButton, &LockerButton::clicked, [=](bool) {

                QFileInfo *FileInfo = new QFileInfo(FindFile->at(i).split("(").at(0));
                if(FileInfo->isFile()){
                    ProjectTree->loadFile(FileInfo->filePath());
                }
                for(int t=0;t<EditList->size();t++){
                    if(EditList->at(t)->whatsThis() == FileInfo->filePath()){
                          int Line = FindFileString->at(i)->at(j).split(".").at(0).toInt()-1;
                          EditList->at(t)->setCursorPosition(Line,0);
                          EditList->at(t)->setSelection(Line,0,Line,EditList->at(t)->text(Line).size()-1);
                    }
                }

            });

        }

        //set all
        FindButtonWidget->setLayout(FindButtonLayout);
        vlayout->addWidget(FindButtonWidget);
        FindButtonWidget->setVisible(false);

        connect(FindFileButton, &LockerButton::clicked, [=](bool) {
            if (FindButtonWidget->isVisible()){

                FindFileButton->SetImageLabel(QPixmap(":/new/prefix1/res/right.png"));
                FindButtonWidget->setVisible(false);

            }
            else{

                FindFileButton->SetImageLabel(QPixmap(":/new/prefix1/res/down.png"));
                FindButtonWidget->setVisible(true);

            }
        });

    }

    vlayout->addStretch();
    vlayout->setMargin(0);
    vlayout->setSpacing(0);

    QWidget *AllWidget = new QWidget;
    AllWidget->setLayout(vlayout);
    this->ScrollArea->setFrameStyle(QFrame::NoFrame);
    this->ScrollArea->setWidget(AllWidget);

}
