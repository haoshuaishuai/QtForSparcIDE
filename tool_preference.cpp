#include "tool_preference.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qscilexercpp.h"
#include "edit.h"

#include <QTableView>
#include <QHeaderView>
#include <QDebug>
#include <QAction>
#include <QSettings>
#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QPalette>
#include <QDateTime>
#include "status_bar.h"

tool_preference::tool_preference(QWidget *parent,file *FileMenu,edit *Edit,statusbar *StatusBar,QWidget *MainWindow) : QWidget(parent){

    this->FileMenu = FileMenu;
    this->Edit = Edit;
    this->StatusBar = StatusBar;
    this->MainWindow = MainWindow;
    this->InitWidget();
    this->ConnectSignals();
    this->setWindowTitle("Preference");
    this->setWindowIcon(QIcon(":/new/prefix1/res/preferenceSet"));

}

void tool_preference::ConnectSignals(){

    connect(SelectWidget,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(itemPressedSlot(QTreeWidgetItem*,int)));
    connect(Default, &QPushButton::clicked,this,[=]{

        SetDefault();

    });
    connect(Ok, &QPushButton::clicked,this,[=]{

        RefreshIniFile(FileMenu->SdkFilePath);
        Edit->RefreshEditList();
        StatusBar->DetermineMessage("Config Active");

    });
    connect(Cancel, &QPushButton::clicked,this,[=]{

        this->close();
        StatusBar->DetermineMessage("Config Cancle");

    });
    connect(GccNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(0,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose Gcc tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(0,1)->setText(fileName);
        }

    });
    connect(GccPlusNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(1,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose G++ tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(1,1)->setText(fileName);
        }

    });
    connect(GdbNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(2,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose Gdb tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(2,1)->setText(fileName);
        }

    });
    connect(LinkNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(3,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose Ld tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(3,1)->setText(fileName);
        }

    });
    connect(ObjCopyNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(4,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose ObjCopy tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(4,1)->setText(fileName);
        }

    });
    connect(ObjDumpNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(5,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose ObjDump tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(5,1)->setText(fileName);
        }

    });
    connect(ArNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(6,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose ArName tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(6,1)->setText(fileName);
        }

    });
    connect(ReadElfNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(7,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose ReadElf tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(7,1)->setText(fileName);
        }

    });
    connect(SizeNameDir, &QPushButton::clicked,this,[=]{

        QFileInfo dir(BuildToolsWidget->item(8,1)->text());
        QString InitPath;
        if (dir.isFile()) {
            InitPath = dir.path();
        } else {
            InitPath = QCoreApplication::applicationDirPath();
        }

        QString fileName = QFileDialog::getOpenFileName(this, "Choose Size tool",InitPath,
                                                      "tools(*.exe);;ALL File(*.*)");
        if (!fileName.isEmpty()){
             BuildToolsWidget->item(8,1)->setText(fileName);
        }

    });
    connect(AddLibary, &QPushButton::clicked,this,[=]{

        QString fileName = QFileDialog::QFileDialog::getExistingDirectory(nullptr, "Select Include Dir", QCoreApplication::applicationDirPath(), QFileDialog::ShowDirsOnly);
        if (!fileName.isEmpty()){
            QTableWidgetItem * Item = new QTableWidgetItem(fileName);
            LinkLibaryWidget->setRowCount(LinkLibaryWidget->rowCount()+1);
            LinkLibaryWidget->setItem(LinkLibaryWidget->rowCount()-1,0,Item);
        }

    });
    connect(DeleateLibary, &QPushButton::clicked,this,[=]{

        QList<QTableWidgetItem*> selectedItems = LinkLibaryWidget->selectedItems();
        foreach (QTableWidgetItem* item, selectedItems){

            LinkLibaryWidget->removeRow(item->row());

        }

    });
    connect(CaretForegroundColorButton, &QPushButton::clicked,this,[=]{

        CaretForegroundColor = QColorDialog::getColor(CaretForegroundColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (CaretForegroundColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(CaretForegroundColor.name());
            CaretForegroundColorButton->setStyleSheet(styleSheet);
        }

    });
    connect(CaretLineBackgroundColorButton, &QPushButton::clicked,this,[=]{

        CaretLineBackgroundColor = QColorDialog::getColor(CaretLineBackgroundColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (CaretLineBackgroundColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(CaretLineBackgroundColor.name());
            CaretLineBackgroundColorButton->setStyleSheet(styleSheet);
        }

    });
    connect(SelectionBackgroundColorButton, &QPushButton::clicked,this,[=]{

        SelectionBackgroundColor = QColorDialog::getColor(SelectionBackgroundColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (SelectionBackgroundColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(SelectionBackgroundColor.name());
            SelectionBackgroundColorButton->setStyleSheet(styleSheet);
        }

    });
    connect(SelectionForegroundColorButton, &QPushButton::clicked,this,[=]{

        SelectionForegroundColor = QColorDialog::getColor(SelectionForegroundColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (SelectionForegroundColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(SelectionForegroundColor.name());
            SelectionForegroundColorButton->setStyleSheet(styleSheet);
        }

    });
    connect(FoldColorBackButton, &QPushButton::clicked,this,[=]{

        FoldColorBack = QColorDialog::getColor(FoldColorBackButton->palette().color(QPalette::Button), nullptr, "Select Color");
        FoldColorFront = FoldColorFrontButton->palette().color(QPalette::Button);
        if (FoldColorBack.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(FoldColorBack.name());
            FoldColorBackButton->setStyleSheet(styleSheet);
        }

    });
    connect(FoldColorFrontButton, &QPushButton::clicked,this,[=]{

        FoldColorBack = FoldColorBackButton->palette().color(QPalette::Button);
        FoldColorFront = QColorDialog::getColor(FoldColorFrontButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (FoldColorFront.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(FoldColorFront.name());
            FoldColorFrontButton->setStyleSheet(styleSheet);
        }

    });
    connect(MarginsBackgroundColorButton, &QPushButton::clicked,this,[=]{

        MarginsBackgroundColor = QColorDialog::getColor(MarginsBackgroundColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (MarginsBackgroundColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(MarginsBackgroundColor.name());
            MarginsBackgroundColorButton->setStyleSheet(styleSheet);
        }

    });
    connect(MarginsForegroundColorButton, &QPushButton::clicked,this,[=]{

        MarginsForegroundColor = QColorDialog::getColor(MarginsForegroundColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (MarginsForegroundColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(MarginsForegroundColor.name());
            MarginsForegroundColorButton->setStyleSheet(styleSheet);
        }

    });
    connect(MarginsFontButton, &QPushButton::clicked,this,[=]{

        bool ok;
        QFont font = QFontDialog::getFont(&ok, MarginsFontButton->font(), nullptr, "Select Font");
        if (ok) {
            MarginsFontButton->setFont(font);
            MarginsFontButton->setText(QString("Font: %1 / Size: %2").arg(font.family()).arg(font.pointSize()));
        }

    });
    connect(WordFontButton, &QPushButton::clicked,this,[=]{

        bool ok;
        QFont font = QFontDialog::getFont(&ok, WordFontButton->font(), nullptr, "Select Font");
        if (ok) {
            WordFontButton->setFont(font);
            WordFontButton->setText(QString("Font: %1 / Size: %2").arg(font.family()).arg(font.pointSize()));
        }

    });
    connect(PaperColorButton, &QPushButton::clicked,this,[=]{

        PaperColor = QColorDialog::getColor(PaperColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
        if (PaperColor.isValid()) {
            QString styleSheet = QString("background-color: %1; color: white").arg(PaperColor.name());
            PaperColorButton->setStyleSheet(styleSheet);
        }

    });

}

void tool_preference::RefreshIniFile(QString SdkPath){

      QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
      QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

      QString GccPath = BuildToolsWidget->item(0,1)->text();
      QString GPlusPlusPath = BuildToolsWidget->item(1,1)->text();
      QString GdbPath = BuildToolsWidget->item(2,1)->text();
      QString LinkPath = BuildToolsWidget->item(3,1)->text();
      QString ObjCopyPath = BuildToolsWidget->item(4,1)->text();
      QString ObjDumpPath = BuildToolsWidget->item(5,1)->text();
      QString ArPath = BuildToolsWidget->item(6,1)->text();
      QString ReadElfPath = BuildToolsWidget->item(7,1)->text();
      QString SizePath = BuildToolsWidget->item(8,1)->text();

      GlobalSetting.beginGroup("BuildTools");
      GlobalSetting.setValue("GccPath", GccPath);
      GlobalSetting.setValue("GPlusPlusPath", GPlusPlusPath);
      GlobalSetting.setValue("GdbPath", GdbPath);
      GlobalSetting.setValue("LinkPath", LinkPath);
      GlobalSetting.setValue("ObjCopyPath", ObjCopyPath);
      GlobalSetting.setValue("ObjDumpPath", ObjDumpPath);
      GlobalSetting.setValue("ArPath", ArPath);
      GlobalSetting.setValue("ReadElfPath", ReadElfPath);
      GlobalSetting.setValue("SizePath", SizePath);
      GlobalSetting.endGroup();

      if(SdkPath!=""){

          //BuildSetting Setting
          QFileInfo SdkFileInfo(SdkPath);
          QString ProjectIniPath = SdkFileInfo.path()+"/.Configure/Config.ini";
          QSettings ProjectSetting(ProjectIniPath,QSettings::IniFormat);
          QString WarningMessage="";
          QString V8ExtensionMessage="";
          QString NotEnableFpuMessage = "";
          QString SoftFloatMessage = "";
          QString FlatRegisterMessage = "";
          QString OptimizeSelectMessage = "";
          QString OtherOptmizationMessage = "";
          QString MakeFileTypeMessage = "";
          QString OtherBuildParmatersMessage = "";

          if(SyntaxCheckOnly->isChecked()){
                WarningMessage = "-fsyntax-only ";
          }else if(Pedantic->isChecked()){
                WarningMessage = "-pedantic ";
          }else if(InhibitWarning->isChecked()){
                WarningMessage = "-w ";
          }else {
                WarningMessage = "-Wall ";
          }

          if(V8Ext->isChecked()){
              V8ExtensionMessage = "-mcpu=v8 ";
          }else{
              V8ExtensionMessage = "";
          }

          if(NoFPU->isChecked()){
              NotEnableFpuMessage = "-mno-fpu ";
          }else{
              NotEnableFpuMessage = "";
          }

          if(SoftFloat->isChecked()){
              SoftFloatMessage = "-msoft-float ";
          }else{
              SoftFloatMessage = "";
          }

          if(Mflat->isChecked()){
              FlatRegisterMessage = "-mflat ";
          }else{
              FlatRegisterMessage = "";
          }

          switch(OptLevel->currentIndex()){

            case(0): OptimizeSelectMessage = "-O0 ";break;
            case(1): OptimizeSelectMessage = "-O1 ";break;
            case(2): OptimizeSelectMessage = "-O2 ";break;
            case(3): OptimizeSelectMessage = "-O3 ";break;
            default: OptimizeSelectMessage = "-O0 ";break;

          }

          OtherOptmizationMessage = OtherOpt->text();

          switch(MakeFileType->currentIndex()){

              case(0): MakeFileTypeMessage = "auto";break;
              case(1): MakeFileTypeMessage = "default";break;
              default: MakeFileTypeMessage = "auto";break;

          }

          OtherBuildParmatersMessage = OtherParameter->text();

          ProjectSetting.beginGroup("BuildSetting");
          ProjectSetting.setValue("WarningMessage", WarningMessage);
          ProjectSetting.setValue("V8Extension", V8ExtensionMessage);
          ProjectSetting.setValue("NotEnableFpu", NotEnableFpuMessage);
          ProjectSetting.setValue("SoftFloat", SoftFloatMessage);
          ProjectSetting.setValue("FlatRegister", FlatRegisterMessage);
          ProjectSetting.setValue("OptimizeSelect", OptimizeSelectMessage);
          ProjectSetting.setValue("OtherOptmization", OtherOptmizationMessage);
          ProjectSetting.setValue("MakeFileType", MakeFileTypeMessage);
          ProjectSetting.setValue("OtherBuildParmaters", OtherBuildParmatersMessage);
          ProjectSetting.endGroup();

          ProjectSetting.beginGroup("LinkLibary");
          QString LinkLibaryMessage = "";
          for(int i=0;i<LinkLibaryWidget->rowCount();i++){
            LinkLibaryMessage += LinkLibaryWidget->item(i,0)->text()+";";
          }
          ProjectSetting.setValue("LinkLibary", LinkLibaryMessage);
          ProjectSetting.endGroup();

          QString TextAddress = TextAddressEdit->text();
          QString DataAddress = DataAddressEdit->text();
          QString BssAddress = BssAddressEdit->text();
          QString StackAddress = StackAddressEdit->text();

          QString DontUseDefaultLibMessage = "";
          QString NoSharedLibMessage = "";
          QString DontPageAlignDataMessage = "";
          QString OmitAllSymboMessage = "";
          QString LinkFileTypeMessage = "";

          if(NoDefaultLibs->isChecked()){
             DontUseDefaultLibMessage = "-nodefaultlibs ";
          }else{
             DontUseDefaultLibMessage = "";
          }

          if(NoSharedLibraries->isChecked()){
             NoSharedLibMessage = "-static ";
          }else{
             NoSharedLibMessage = "";
          }

          if(DontPageAlign->isChecked()){
             DontPageAlignDataMessage = "-n ";
          }else{
             DontPageAlignDataMessage = "";
          }

          if(OmitAllSymbol->isChecked()){
             OmitAllSymboMessage = "-s ";
          }else{
             OmitAllSymboMessage = "";
          }


          QString UseCkExeMessage = "";
          QString ReadElfMessage = "";

          if(UseCkExe->isChecked()){
             UseCkExeMessage = "true";
          }else{
             UseCkExeMessage = "false";
          }

          if(ReadElf->isChecked()){
             ReadElfMessage = "true";
          }else{
             ReadElfMessage = "false";
          }

          if(LinkFileType->currentIndex()==0){
             LinkFileTypeMessage = "auto";
          }else{
             LinkFileTypeMessage = "default";
          }

          QString OtherLinkParmatersMessage = OtherLinkParameter->text();

          ProjectSetting.beginGroup("LinkSetting");
          ProjectSetting.setValue("TextAddress", TextAddress);
          ProjectSetting.setValue("DataAddress", DataAddress);
          ProjectSetting.setValue("BssAddress", BssAddress);
          ProjectSetting.setValue("StackAddress", StackAddress);

          ProjectSetting.setValue("DontUseDefaultLib", DontUseDefaultLibMessage);
          ProjectSetting.setValue("NoSharedLib", NoSharedLibMessage);
          ProjectSetting.setValue("DontPageAlignData", DontPageAlignDataMessage);
          ProjectSetting.setValue("OmitAllSymbo", OmitAllSymboMessage);

          ProjectSetting.setValue("UseCkExe", UseCkExeMessage);
          ProjectSetting.setValue("ReadElf", ReadElfMessage);

          ProjectSetting.setValue("LinkFileType", LinkFileTypeMessage);
          ProjectSetting.setValue("OtherLinkParmaters", OtherLinkParmatersMessage);
          ProjectSetting.endGroup();

      }

      QString IndentationsUseTabsString = "";
      QString SetIndentationGuidesString = "";
      QString AutoindentString = "";
      QString CaretLineVisibleString = "";
      QString BackspaceUnindentsString = "";
      QString AutoCompletionSourceString = "";

      if(IndentationsUseTabs->isChecked()){
        IndentationsUseTabsString = "true";
      }else{
        IndentationsUseTabsString = "false";
      }

      if(SetIndentationGuides->isChecked()){
        SetIndentationGuidesString = "true";
      }else{
        SetIndentationGuidesString = "false";
      }

      if(Autoindent->isChecked()){
        AutoindentString = "true";
      }else{
        AutoindentString = "false";
      }

      if(CaretLineVisible->isChecked()){
        CaretLineVisibleString = "true";
      }else{
        CaretLineVisibleString = "false";
      }

      if(BackspaceUnindents->isChecked()){
        BackspaceUnindentsString = "true";
      }else{
        BackspaceUnindentsString = "false";
      }

      if(AutoCompletionSource->isChecked()){
        AutoCompletionSourceString = "true";
      }else{
        AutoCompletionSourceString = "false";
      }

      GlobalSetting.beginGroup("EditBaseSetting");
      GlobalSetting.setValue("IndentationsUseTabsString", IndentationsUseTabsString);
      GlobalSetting.setValue("SetIndentationGuidesString", SetIndentationGuidesString);
      GlobalSetting.setValue("AutoindentString", AutoindentString);
      GlobalSetting.setValue("CaretLineVisibleString", CaretLineVisibleString);
      GlobalSetting.setValue("BackspaceUnindentsString", BackspaceUnindentsString);
      GlobalSetting.setValue("AutoCompletionSourceString", AutoCompletionSourceString);
      GlobalSetting.endGroup();

      QString WrapModeString = WrapMode->currentText();
      QString IndentationSizeString = IndentationSize->text();
      QString CaretForegroundColorString = CaretForegroundColorButton->palette().color(QPalette::Button).name();
      QString CaretLineBackgroundColorString = CaretLineBackgroundColorButton->palette().color(QPalette::Button).name();
      QString CaretWidthString = CaretWidth->text();
      QString SelectionBackgroundColorString = SelectionBackgroundColorButton->palette().color(QPalette::Button).name();
      QString SelectionForegroundColorString = SelectionForegroundColorButton->palette().color(QPalette::Button).name();

      GlobalSetting.beginGroup("EditOtherSetting");
      GlobalSetting.setValue("WrapModeString", WrapModeString);
      GlobalSetting.setValue("IndentationSizeString", IndentationSizeString);
      GlobalSetting.setValue("CaretForegroundColorString", CaretForegroundColorString);
      GlobalSetting.setValue("CaretLineBackgroundColorString", CaretLineBackgroundColorString);
      GlobalSetting.setValue("CaretWidthString", CaretWidthString);
      GlobalSetting.setValue("SelectionBackgroundColorString", SelectionBackgroundColorString);
      GlobalSetting.setValue("SelectionForegroundColorString", SelectionForegroundColorString);
      GlobalSetting.endGroup();

      QString FoldStyleString = FoldStyle->currentText();
      QString FoldColorBackString = FoldColorBackButton->palette().color(QPalette::Button).name();
      QString FoldColorFrontString = FoldColorFrontButton->palette().color(QPalette::Button).name();

      GlobalSetting.beginGroup("EditFoldSetting");
      GlobalSetting.setValue("FoldStyleString", FoldStyleString);
      GlobalSetting.setValue("FoldColorBackString", FoldColorBackString);
      GlobalSetting.setValue("FoldColorFrontString", FoldColorFrontString);
      GlobalSetting.endGroup();

      QString MarginWidthString = MarginWidth->text();
      QString MarginsBackgroundColorString = MarginsBackgroundColorButton->palette().color(QPalette::Button).name();
      QString MarginsForegroundColorString = MarginsForegroundColorButton->palette().color(QPalette::Button).name();
      QString MarginsFontName = MarginsFontButton->font().family();
      QString MarginsFontSize = QString::number(MarginsFontButton->font().pointSize());
      QString MarginsFontBold = MarginsFontButton->font().bold() ? "true" : "false";
      QString MarginsFontItalic  = MarginsFontButton->font().italic() ? "true" : "false";
      QString MarginsFontUnderline  = MarginsFontButton->font().underline() ? "true" : "false";
      QString BraceMatchingString = BraceMatching->currentText();

      GlobalSetting.beginGroup("EditMarginSetting");
      GlobalSetting.setValue("MarginWidthString", MarginWidthString);
      GlobalSetting.setValue("MarginsBackgroundColorString", MarginsBackgroundColorString);
      GlobalSetting.setValue("MarginsForegroundColorString", MarginsForegroundColorString);
      GlobalSetting.setValue("MarginsFontName", MarginsFontName);
      GlobalSetting.setValue("MarginsFontSize", MarginsFontSize);
      GlobalSetting.setValue("MarginsFontBold", MarginsFontBold);
      GlobalSetting.setValue("MarginsFontItalic", MarginsFontItalic);
      GlobalSetting.setValue("MarginsFontUnderline", MarginsFontUnderline);
      GlobalSetting.setValue("BraceMatchingString", BraceMatchingString);
      GlobalSetting.endGroup();

      QString Comment = StyleButtonList->at(0)->palette().color(QPalette::Button).name();
      QString CommentLine = StyleButtonList->at(1)->palette().color(QPalette::Button).name();
      QString CommentDoc = StyleButtonList->at(2)->palette().color(QPalette::Button).name();
      QString Number = StyleButtonList->at(3)->palette().color(QPalette::Button).name();
      QString Keyword = StyleButtonList->at(4)->palette().color(QPalette::Button).name();
      QString DoubleQuotedString = StyleButtonList->at(5)->palette().color(QPalette::Button).name();
      QString SingleQuotedString  = StyleButtonList->at(6)->palette().color(QPalette::Button).name();
      QString UUID  = StyleButtonList->at(7)->palette().color(QPalette::Button).name();
      QString PreProcessor = StyleButtonList->at(8)->palette().color(QPalette::Button).name();
      QString Operator = StyleButtonList->at(9)->palette().color(QPalette::Button).name();
      QString Identifier = StyleButtonList->at(10)->palette().color(QPalette::Button).name();
      QString UnclosedString = StyleButtonList->at(11)->palette().color(QPalette::Button).name();
      QString VerbatimString = StyleButtonList->at(12)->palette().color(QPalette::Button).name();
      QString Regex = StyleButtonList->at(13)->palette().color(QPalette::Button).name();
      QString CommentLineDoc = StyleButtonList->at(14)->palette().color(QPalette::Button).name();
      QString KeywordSet2 = StyleButtonList->at(15)->palette().color(QPalette::Button).name();
      QString CommentDocKeyword = StyleButtonList->at(16)->palette().color(QPalette::Button).name();
      QString CommentDocKeywordError = StyleButtonList->at(17)->palette().color(QPalette::Button).name();
      QString GlobalClass = StyleButtonList->at(18)->palette().color(QPalette::Button).name();
      QString RawString = StyleButtonList->at(19)->palette().color(QPalette::Button).name();
      QString TripleQuotedVerbatimString = StyleButtonList->at(20)->palette().color(QPalette::Button).name();
      QString HashQuotedString = StyleButtonList->at(21)->palette().color(QPalette::Button).name();
      QString PreProcessorComment = StyleButtonList->at(22)->palette().color(QPalette::Button).name();
      QString PreProcessorCommentLineDoc = StyleButtonList->at(23)->palette().color(QPalette::Button).name();
      QString UserLiteral = StyleButtonList->at(24)->palette().color(QPalette::Button).name();
      QString TaskMarker = StyleButtonList->at(25)->palette().color(QPalette::Button).name();
      QString EscapeSequence = StyleButtonList->at(26)->palette().color(QPalette::Button).name();

      QString WordFontName = WordFontButton->font().family();
      QString WordFontSize = QString::number(WordFontButton->font().pointSize());
      QString WordFontBold = WordFontButton->font().bold() ? "true" : "false";
      QString WordFontItalic  = WordFontButton->font().italic() ? "true" : "false";
      QString WordFontUnderline  = WordFontButton->font().underline() ? "true" : "false";

      QString PaperColorString = PaperColorButton->palette().color(QPalette::Button).name();

      GlobalSetting.beginGroup("EditStyleSetting");
      GlobalSetting.setValue("Comment", Comment);
      GlobalSetting.setValue("CommentLine", CommentLine);
      GlobalSetting.setValue("CommentDoc", CommentDoc);
      GlobalSetting.setValue("Number", Number);
      GlobalSetting.setValue("Keyword", Keyword);
      GlobalSetting.setValue("DoubleQuotedString", DoubleQuotedString);
      GlobalSetting.setValue("SingleQuotedString", SingleQuotedString);
      GlobalSetting.setValue("UUID", UUID);
      GlobalSetting.setValue("PreProcessor", PreProcessor);
      GlobalSetting.setValue("Operator", Operator);
      GlobalSetting.setValue("Identifier", Identifier);
      GlobalSetting.setValue("UnclosedString", UnclosedString);
      GlobalSetting.setValue("VerbatimString", VerbatimString);
      GlobalSetting.setValue("Regex", Regex);
      GlobalSetting.setValue("CommentLineDoc", CommentLineDoc);
      GlobalSetting.setValue("KeywordSet2", KeywordSet2);
      GlobalSetting.setValue("CommentDocKeyword", CommentDocKeyword);
      GlobalSetting.setValue("CommentDocKeywordError", CommentDocKeywordError);
      GlobalSetting.setValue("GlobalClass", GlobalClass);
      GlobalSetting.setValue("RawString", RawString);
      GlobalSetting.setValue("TripleQuotedVerbatimString", TripleQuotedVerbatimString);
      GlobalSetting.setValue("HashQuotedString", HashQuotedString);
      GlobalSetting.setValue("PreProcessorComment", PreProcessorComment);
      GlobalSetting.setValue("PreProcessorCommentLineDoc", PreProcessorCommentLineDoc);
      GlobalSetting.setValue("UserLiteral", UserLiteral);
      GlobalSetting.setValue("TaskMarker", TaskMarker);
      GlobalSetting.setValue("EscapeSequence", EscapeSequence);
      GlobalSetting.setValue("WordFontName", WordFontName);
      GlobalSetting.setValue("WordFontSize", WordFontSize);
      GlobalSetting.setValue("WordFontBold", WordFontBold);
      GlobalSetting.setValue("WordFontItalic", WordFontItalic);
      GlobalSetting.setValue("WordFontUnderline", WordFontUnderline);
      GlobalSetting.setValue("PaperColorString", PaperColorString);
      GlobalSetting.endGroup();

}

void tool_preference::ReadIni(QString SdkPath){

      QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
      QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

      GlobalSetting.beginGroup("BuildTools");
      QString GccPath =GlobalSetting.value("GccPath").toString();
      if(GccPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        GccPath = GccPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString GPlusPlusPath =GlobalSetting.value("GPlusPlusPath").toString();
      if(GPlusPlusPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        GPlusPlusPath = GPlusPlusPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString GdbPath =GlobalSetting.value("GdbPath").toString();
      if(GdbPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        GdbPath = GdbPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString LinkPath = GlobalSetting.value("LinkPath").toString();
      if(LinkPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        LinkPath = LinkPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString ObjCopyPath = GlobalSetting.value("ObjCopyPath").toString();
      if(ObjCopyPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ObjCopyPath = ObjCopyPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString ObjDumpPath = GlobalSetting.value("ObjDumpPath").toString();
      if(ObjDumpPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ObjDumpPath = ObjDumpPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString ArPath = GlobalSetting.value("ArPath").toString();
      if(ArPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ArPath = ArPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString ReadElfPath = GlobalSetting.value("ReadElfPath").toString();
      if(ReadElfPath.contains("CurrentDir:", Qt::CaseInsensitive)){
        ReadElfPath = ReadElfPath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      QString SizePath = GlobalSetting.value("SizePath").toString();
      if(SizePath.contains("CurrentDir:", Qt::CaseInsensitive)){
        SizePath = SizePath.replace("CurrentDir:",QCoreApplication::applicationDirPath());
      }
      GlobalSetting.endGroup();

      BuildToolsWidget->item(0,1)->setText(GccPath);
      BuildToolsWidget->item(1,1)->setText(GPlusPlusPath);
      BuildToolsWidget->item(2,1)->setText(GdbPath);
      BuildToolsWidget->item(3,1)->setText(LinkPath);
      BuildToolsWidget->item(4,1)->setText(ObjCopyPath);
      BuildToolsWidget->item(5,1)->setText(ObjDumpPath);
      BuildToolsWidget->item(6,1)->setText(ArPath);
      BuildToolsWidget->item(7,1)->setText(ReadElfPath);
      BuildToolsWidget->item(8,1)->setText(SizePath);

      if(SdkPath!=""){

          //BuildSetting Setting
          QFileInfo SdkFileInfo(SdkPath);
          QString ProjectIniPath = SdkFileInfo.path()+"/.Configure/Config.ini";
          QSettings ProjectSetting(ProjectIniPath,QSettings::IniFormat);

          QString ProjectNameString = SdkFileInfo.fileName();
          QString ProjectLoctionString = SdkFileInfo.path();
          QString LastModifiedString = "";
          QString FileNumsString = "";
          QDateTime minDateTime(QDate(0, 0, 0), QTime(0, 0, 0));
          QStringList AllFileList = FileMenu->SettingFile->readAllFile(SdkPath);
          for(int i=0;i<AllFileList.size();i++){
             QFileInfo ModifiedFile(AllFileList.at(i));
             if(ModifiedFile.lastModified()> minDateTime){
                 minDateTime = ModifiedFile.lastModified();
                 LastModifiedString = minDateTime.toString("yyyy.MM.dd hh:mm:ss");
             }
          }
          QStringList FileList = FileMenu->SettingFile->readSdkAllFile(SdkPath);
          FileNumsString = QString::number(FileList.size()) + " Files";

          ProjectName    ->setText("Project Name     : "+ProjectNameString);
          ProjectLocation->setText("Project Location : "+ProjectLoctionString);
          LastModified   ->setText("Last Modified    : "+LastModifiedString);
          FileNums       ->setText("Totle Files      : "+FileNumsString);

          QString WarningMessage="";
          QString V8ExtensionMessage="";
          QString NotEnableFpuMessage = "";
          QString SoftFloatMessage = "";
          QString FlatRegisterMessage = "";
          QString OptimizeSelectMessage = "";
          QString OtherOptmizationMessage = "";
          QString MakeFileTypeMessage = "";
          QString OtherBuildParmatersMessage = "";

          ProjectSetting.beginGroup("BuildSetting");
          WarningMessage = ProjectSetting.value("WarningMessage").toString();
          V8ExtensionMessage = ProjectSetting.value("V8Extension").toString();
          NotEnableFpuMessage = ProjectSetting.value("NotEnableFpu").toString();
          SoftFloatMessage = ProjectSetting.value("SoftFloat").toString();
          FlatRegisterMessage = ProjectSetting.value("FlatRegister").toString();
          OptimizeSelectMessage = ProjectSetting.value("OptimizeSelect").toString();
          OtherOptmizationMessage = ProjectSetting.value("OtherOptmization").toString();
          MakeFileTypeMessage = ProjectSetting.value("MakeFileType").toString();
          OtherBuildParmatersMessage = ProjectSetting.value("OtherBuildParmaters").toString();
          ProjectSetting.endGroup();

          if(WarningMessage == "-fsyntax-only "){
                SyntaxCheckOnly->setChecked(true);
          }else if(WarningMessage == "-pedantic "){
                Pedantic->setChecked(true);
          }else if(WarningMessage == "-w "){
                InhibitWarning->setChecked(true);
          }else {
                AllWarning->setChecked(true);
          }

          if( V8ExtensionMessage == "-mcpu=v8 "){
              V8Ext->setChecked(true);
          }else{
              V8Ext->setChecked(false);
          }

          if(NotEnableFpuMessage == "-mno-fpu "){
              NoFPU->setChecked(true);
          }else{
              NoFPU->setChecked(false);
          }

          if(SoftFloatMessage == "-msoft-float "){
              SoftFloat->setChecked(true);
          }else{
              SoftFloat->setChecked(false);
          }

          if(FlatRegisterMessage == "-mflat "){
              Mflat->setChecked(true);
          }else{
              Mflat->setChecked(false);
          }

          if(OptimizeSelectMessage == "-O1 "){
                OptLevel->setCurrentIndex(1);
          }else if(OptimizeSelectMessage == "-O2 "){
                OptLevel->setCurrentIndex(2);
          }else if(OptimizeSelectMessage == "-O3 "){
                OptLevel->setCurrentIndex(3);
          }else{
                OptLevel->setCurrentIndex(0);
          }

          OtherOpt->setText(OtherOptmizationMessage);

          if(MakeFileTypeMessage == "default"){
                MakeFileType->setCurrentIndex(1);
          }else{
                MakeFileType->setCurrentIndex(0);
          }

          OtherParameter->setText(OtherBuildParmatersMessage);

          ProjectSetting.beginGroup("LinkLibary");
          QString LinkLibaryMessage =  "";
          LinkLibaryMessage = ProjectSetting.value("LinkLibary").toString();
          ProjectSetting.endGroup();
          QStringList LinkLibs = LinkLibaryMessage.split(";");
          LinkLibaryWidget->setRowCount(LinkLibs.size()-1);
          for(int i = 0;i<LinkLibs.size();i++){
            LinkLibaryWidget->setItem(i,0,new QTableWidgetItem(LinkLibs.at(i)));

          }

          QString DontUseDefaultLibMessage = "";
          QString NoSharedLibMessage = "";
          QString DontPageAlignDataMessage = "";
          QString OmitAllSymboMessage = "";
          QString LinkFileTypeMessage = "";
          QString TextAddress = "";
          QString DataAddress = "";
          QString BssAddress = "";
          QString StackAddress = "";
          QString UseCkExeMessage = "";
          QString ReadElfMessage = "";
          QString OtherLinkParmatersMessage = "";

          ProjectSetting.beginGroup("LinkSetting");
          TextAddress = ProjectSetting.value("TextAddress").toString();
          DataAddress = ProjectSetting.value("DataAddress").toString();
          BssAddress = ProjectSetting.value("BssAddress").toString();
          StackAddress = ProjectSetting.value("StackAddress").toString();
          DontUseDefaultLibMessage = ProjectSetting.value("DontUseDefaultLib").toString();
          NoSharedLibMessage = ProjectSetting.value("NoSharedLib").toString();
          DontPageAlignDataMessage = ProjectSetting.value("DontPageAlignData").toString();
          OmitAllSymboMessage = ProjectSetting.value("OmitAllSymbo").toString();
          UseCkExeMessage = ProjectSetting.value("UseCkExe").toString();
          ReadElfMessage = ProjectSetting.value("ReadElf").toString();
          LinkFileTypeMessage = ProjectSetting.value("LinkFileType").toString();
          OtherLinkParmatersMessage = ProjectSetting.value("OtherLinkParmaters").toString();
          ProjectSetting.endGroup();
          TextAddressEdit->setText(TextAddress);
          DataAddressEdit->setText(DataAddress);
          BssAddressEdit->setText(BssAddress);
          StackAddressEdit->setText(StackAddress);

          if(DontUseDefaultLibMessage == "-nodefaultlibs "){
             NoDefaultLibs->setChecked(true);
          }else{
             NoDefaultLibs->setChecked(false);
          }

          if(NoSharedLibMessage == "-static "){
             NoSharedLibraries->setChecked(true);
          }else{
             NoSharedLibraries->setChecked(false);
          }

          if(DontPageAlignDataMessage == "-n "){
             DontPageAlign->setChecked(true);
          }else{
             DontPageAlign->setChecked(false);
          }

          if(OmitAllSymboMessage == "-s "){
             OmitAllSymbol->setChecked(true);
          }else{
             OmitAllSymbol->setChecked(false);
          }

          if(UseCkExeMessage == "true"){
             UseCkExe->setChecked(true);
          }else{
             UseCkExe->setChecked(false);
          }

          if(ReadElfMessage == "true"){
             ReadElf->setChecked(true);
          }else{
             ReadElf->setChecked(false);
          }

          if(LinkFileTypeMessage == "auto"){
             LinkFileType->setCurrentIndex(0);
          }else{
             LinkFileType->setCurrentIndex(1);
          }

          OtherLinkParameter->setText(OtherLinkParmatersMessage);

      }

      //Edit Setting

      GlobalSetting.beginGroup("EditBaseSetting");
      QString IndentationsUseTabsString =  GlobalSetting.value("IndentationsUseTabsString").toString();
      QString SetIndentationGuidesString = GlobalSetting.value("SetIndentationGuidesString").toString();
      QString AutoindentString = GlobalSetting.value("AutoindentString").toString();
      QString CaretLineVisibleString = GlobalSetting.value("CaretLineVisibleString").toString();
      QString BackspaceUnindentsString = GlobalSetting.value("BackspaceUnindentsString").toString();
      QString AutoCompletionSourceString = GlobalSetting.value("AutoCompletionSourceString").toString();
      GlobalSetting.endGroup();

      if(IndentationsUseTabsString == "true"){
         IndentationsUseTabs->setChecked(true);
      }else{
         IndentationsUseTabs->setChecked(false);
      }

      if(SetIndentationGuidesString == "true"){
         SetIndentationGuides->setChecked(true);
      }else{
         SetIndentationGuides->setChecked(false);
      }

      if(AutoindentString == "true"){
         Autoindent->setChecked(true);
      }else{
         Autoindent->setChecked(true);
      }

      if(CaretLineVisibleString == "true"){
         CaretLineVisible->setChecked(true);
      }else{
         CaretLineVisible->setChecked(false);
      }

      if(BackspaceUnindentsString == "true"){
         BackspaceUnindents->setChecked(true);
      }else{
         BackspaceUnindents->setChecked(false);
      }

      if(AutoCompletionSourceString == "true"){
         AutoCompletionSource->setChecked(true);
      }else{
         AutoCompletionSource->setChecked(false);
      }

      GlobalSetting.beginGroup("EditOtherSetting");
      QString WrapModeString = GlobalSetting.value("WrapModeString").toString();
      QString IndentationSizeString = GlobalSetting.value("IndentationSizeString").toString();
      QString CaretForegroundColorString = GlobalSetting.value("CaretForegroundColorString").toString();
      QString CaretLineBackgroundColorString = GlobalSetting.value("CaretLineBackgroundColorString").toString();
      QString CaretWidthString = GlobalSetting.value("CaretWidthString").toString();
      QString SelectionBackgroundColorString = GlobalSetting.value("SelectionBackgroundColorString").toString();
      QString SelectionForegroundColorString = GlobalSetting.value("SelectionForegroundColorString").toString();
      GlobalSetting.endGroup();

      WrapMode->setCurrentText(WrapModeString);
      IndentationSize->setText(IndentationSizeString);
      QString styleSheet = QString("background-color: %1; color: white").arg(CaretForegroundColorString);
      CaretForegroundColorButton->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(CaretLineBackgroundColorString);
      CaretLineBackgroundColorButton->setStyleSheet(styleSheet);
      CaretWidth->setText(CaretWidthString);
      styleSheet = QString("background-color: %1; color: white").arg(SelectionBackgroundColorString);
      SelectionBackgroundColorButton->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(SelectionForegroundColorString);
      SelectionForegroundColorButton->setStyleSheet(styleSheet);

      GlobalSetting.beginGroup("EditFoldSetting");
      QString FoldStyleString = GlobalSetting.value("FoldStyleString").toString();
      QString FoldColorBackString = GlobalSetting.value("FoldColorBackString").toString();
      QString FoldColorFrontString = GlobalSetting.value("FoldColorFrontString").toString();
      GlobalSetting.endGroup();

      FoldStyle->setCurrentText(FoldStyleString);
      styleSheet = QString("background-color: %1; color: white").arg(FoldColorBackString);
      FoldColorBackButton->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(FoldColorFrontString);
      FoldColorFrontButton->setStyleSheet(styleSheet);

      GlobalSetting.beginGroup("EditMarginSetting");
      QString MarginWidthString = GlobalSetting.value("MarginWidthString").toString();
      QString MarginsBackgroundColorString = GlobalSetting.value("MarginsBackgroundColorString").toString();
      QString MarginsForegroundColorString = GlobalSetting.value("MarginsForegroundColorString").toString();
      QString MarginsFontName = GlobalSetting.value("MarginsFontName").toString();
      QString MarginsFontSize = GlobalSetting.value("MarginsFontSize").toString();
      QString MarginsFontBold = GlobalSetting.value("MarginsFontBold").toString();
      QString MarginsFontItalic  = GlobalSetting.value("MarginsFontItalic").toString();
      QString MarginsFontUnderline  = GlobalSetting.value("MarginsFontUnderline").toString();
      QString BraceMatchingString = GlobalSetting.value("BraceMatchingString").toString();
      GlobalSetting.endGroup();

      MarginWidth->setText(MarginWidthString);
      styleSheet = QString("background-color: %1; color: white").arg(MarginsBackgroundColorString);
      MarginsBackgroundColorButton->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(MarginsForegroundColorString);
      MarginsForegroundColorButton->setStyleSheet(styleSheet);
      QFont MarginFont;
      MarginFont.setFamily(MarginsFontName);
      MarginFont.setPointSize(MarginsFontSize.toInt());
      if(MarginsFontBold=="true"){
         MarginFont.setBold(true);
      }else{
         MarginFont.setBold(false);
      }
      if(MarginsFontItalic=="true"){
         MarginFont.setItalic(true);
      }else{
         MarginFont.setItalic(false);
      }
      if(MarginsFontUnderline=="true"){
         MarginFont.setUnderline(true);
      }else{
         MarginFont.setUnderline(false);
      }
      MarginsFontButton->setFont(MarginFont);
      MarginsFontButton->setText(QString("Font: %1 / Size: %2").arg(MarginFont.family()).arg(MarginFont.pointSize()));
      BraceMatching->setCurrentText(BraceMatchingString);

      GlobalSetting.beginGroup("EditStyleSetting");
      QString Comment = GlobalSetting.value("Comment").toString();
      QString CommentLine = GlobalSetting.value("CommentLine").toString();
      QString CommentDoc = GlobalSetting.value("CommentDoc").toString();
      QString Number = GlobalSetting.value("Number").toString();
      QString Keyword = GlobalSetting.value("Keyword").toString();
      QString DoubleQuotedString = GlobalSetting.value("DoubleQuotedString").toString();
      QString SingleQuotedString  = GlobalSetting.value("SingleQuotedString").toString();
      QString UUID  = GlobalSetting.value("UUID").toString();
      QString PreProcessor = GlobalSetting.value("PreProcessor").toString();
      QString Operator = GlobalSetting.value("Operator").toString();
      QString Identifier = GlobalSetting.value("Identifier").toString();
      QString UnclosedString = GlobalSetting.value("UnclosedString").toString();
      QString VerbatimString = GlobalSetting.value("VerbatimString").toString();
      QString Regex = GlobalSetting.value("Regex").toString();
      QString CommentLineDoc = GlobalSetting.value("CommentLineDoc").toString();
      QString KeywordSet2 = GlobalSetting.value("KeywordSet2").toString();
      QString CommentDocKeyword = GlobalSetting.value("CommentDocKeyword").toString();
      QString CommentDocKeywordError = GlobalSetting.value("CommentDocKeywordError").toString();
      QString GlobalClass = GlobalSetting.value("GlobalClass").toString();
      QString RawString = GlobalSetting.value("RawString").toString();
      QString TripleQuotedVerbatimString = GlobalSetting.value("TripleQuotedVerbatimString").toString();
      QString HashQuotedString = GlobalSetting.value("HashQuotedString").toString();
      QString PreProcessorComment = GlobalSetting.value("PreProcessorComment").toString();
      QString PreProcessorCommentLineDoc = GlobalSetting.value("PreProcessorCommentLineDoc").toString();
      QString UserLiteral = GlobalSetting.value("UserLiteral").toString();
      QString TaskMarker = GlobalSetting.value("TaskMarker").toString();
      QString EscapeSequence = GlobalSetting.value("EscapeSequence").toString();
      QString WordFontName = GlobalSetting.value("WordFontName").toString();
      QString WordFontSize = GlobalSetting.value("WordFontSize").toString();
      QString WordFontBold = GlobalSetting.value("WordFontBold").toString();
      QString WordFontItalic  = GlobalSetting.value("WordFontItalic").toString();
      QString WordFontUnderline  = GlobalSetting.value("WordFontUnderline").toString();
      QString PaperColorString = GlobalSetting.value("PaperColorString").toString();
      GlobalSetting.endGroup();

      styleSheet = QString("background-color: %1; color: white").arg(Comment);
      StyleButtonList->at(0)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(CommentLine);
      StyleButtonList->at(1)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(CommentDoc);
      StyleButtonList->at(2)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(Number);
      StyleButtonList->at(3)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(Keyword);
      StyleButtonList->at(4)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(DoubleQuotedString);
      StyleButtonList->at(5)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(SingleQuotedString);
      StyleButtonList->at(6)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(UUID);
      StyleButtonList->at(7)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(PreProcessor);
      StyleButtonList->at(8)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(Operator);
      StyleButtonList->at(9)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(Identifier);
      StyleButtonList->at(10)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(UnclosedString);
      StyleButtonList->at(11)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(VerbatimString);
      StyleButtonList->at(12)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(Regex);
      StyleButtonList->at(13)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(CommentLineDoc);
      StyleButtonList->at(14)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(KeywordSet2);
      StyleButtonList->at(15)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(CommentDocKeyword);
      StyleButtonList->at(16)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(CommentDocKeywordError);
      StyleButtonList->at(17)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(GlobalClass);
      StyleButtonList->at(18)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(RawString);
      StyleButtonList->at(19)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(TripleQuotedVerbatimString);
      StyleButtonList->at(20)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(HashQuotedString);
      StyleButtonList->at(21)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(PreProcessorComment);
      StyleButtonList->at(22)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(PreProcessorCommentLineDoc);
      StyleButtonList->at(23)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(UserLiteral);
      StyleButtonList->at(24)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(TaskMarker);
      StyleButtonList->at(25)->setStyleSheet(styleSheet);
      styleSheet = QString("background-color: %1; color: white").arg(EscapeSequence);
      StyleButtonList->at(26)->setStyleSheet(styleSheet);

      QFont WordFont;
      WordFont.setFamily(WordFontName);
      WordFont.setPointSize(WordFontSize.toInt());
      if(WordFontBold=="true"){
         WordFont.setBold(true);
      }else{
         WordFont.setBold(false);
      }
      if(WordFontItalic=="true"){
         WordFont.setItalic(true);
      }else{
         WordFont.setItalic(false);
      }
      if(WordFontUnderline=="true"){
         WordFont.setUnderline(true);
      }else{
         WordFont.setUnderline(false);
      }
      WordFontButton->setFont(WordFont);
      WordFontButton->setText(QString("Font: %1 / Size: %2").arg(WordFont.family()).arg(WordFont.pointSize()));

      styleSheet = QString("background-color: %1; color: white").arg(PaperColorString);
      PaperColorButton->setStyleSheet(styleSheet);

}

void tool_preference::SetDefault(){

      QString styleSheet;
      QsciLexerCPP *textLexer = new QsciLexerCPP();
      QFont MarginsButtonFont;
      QFont WordFontButtonFont;

      QList<QTreeWidgetItem*> selectedItems = SelectWidget->selectedItems();
      for (int i = 0; i < selectedItems.size(); ++i) {
         QTreeWidgetItem* pressedItem = selectedItems.at(i);
         if(pressedItem->text(0)=="Information"){


         }
         else if(pressedItem->text(0)=="Build" || pressedItem->text(0)=="Build Tools"){

             //Build Tools
             BuildToolsWidget->item(0,1)->setText("");
             BuildToolsWidget->item(1,1)->setText("");
             BuildToolsWidget->item(2,1)->setText("");
             BuildToolsWidget->item(3,1)->setText("");
             BuildToolsWidget->item(4,1)->setText("");
             BuildToolsWidget->item(5,1)->setText("");
             BuildToolsWidget->item(6,1)->setText("");
             BuildToolsWidget->item(7,1)->setText("");
             BuildToolsWidget->item(8,1)->setText("");

         }
         else if(pressedItem->text(0)=="Build Setting"){

             //Build Setting
             SyntaxCheckOnly->setChecked(true);
             Pedantic->setChecked(true);
             InhibitWarning->setChecked(true);
             AllWarning->setChecked(true);
             V8Ext->setChecked(true);
             NoFPU->setChecked(true);
             SoftFloat->setChecked(true);
             Mflat->setChecked(true);
             OptLevel->setCurrentIndex(0);
             OtherOpt->setText("");
             MakeFileType->setCurrentIndex(0);
             OtherParameter->setText("");

         }
         else if(pressedItem->text(0)=="Link" || pressedItem->text(0)=="Link Include"){

            //Link Include
             LinkLibaryWidget->clear();
             LinkLibaryWidget->setRowCount(0);

         }
         else if(pressedItem->text(0)=="Link Setting"){

             //Link Setting
             TextAddressEdit->setText("0x00000000");
             DataAddressEdit->setText("");
             BssAddressEdit->setText("");
             StackAddressEdit->setText("");
             NoDefaultLibs->setChecked(false);
             NoSharedLibraries->setChecked(false);
             DontPageAlign->setChecked(false);
             OmitAllSymbol->setChecked(false);
             UseCkExe->setChecked(false);
             ReadElf->setChecked(false);
             LinkFileType->setCurrentIndex(0);
             OtherLinkParameter->setText("");

         }
         else if(pressedItem->text(0)=="Editor" || pressedItem->text(0)=="Base Setting"){

             //base setting
             IndentationsUseTabs->setChecked(true);
             SetIndentationGuides->setChecked(true);
             Autoindent->setChecked(true);
             CaretLineVisible->setChecked(true);
             BackspaceUnindents->setChecked(true);
             AutoCompletionSource->setChecked(true);

             WrapMode->setCurrentText("Wrap None");
             IndentationSize->setText("4");
             CaretWidth->setText("4");
             styleSheet = QString("background-color: %1; color: white").arg("#000000");
             CaretForegroundColorButton->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg("#c0c0c0");
             CaretLineBackgroundColorButton->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg("#c0c0c0");
             SelectionBackgroundColorButton->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg("#ffffff");
             SelectionForegroundColorButton->setStyleSheet(styleSheet);

         }
         else if(pressedItem->text(0)=="Support Setting"){

             //support setting

             FoldStyle->setCurrentText("Boxed Tree Fold Style");
             styleSheet = QString("background-color: %1; color: white").arg("#f0f0f0");
             FoldColorBackButton->setStyleSheet(styleSheet);
             FoldColorFrontButton->setStyleSheet(styleSheet);

             MarginWidth->setText("35");
             styleSheet = QString("background-color: %1; color: white").arg("#c0c0c0");
             MarginsBackgroundColorButton->setStyleSheet(styleSheet);
             MarginsForegroundColorButton->setStyleSheet(styleSheet);

             MarginsButtonFont.setFamily("Times New Roman");
             MarginsButtonFont.setPointSize(12);
             MarginsFontButton->setFont(MarginsButtonFont);
             MarginsFontButton->setText(QString("Font: %1 / Size: %2").arg(MarginsButtonFont.family()).arg(MarginsButtonFont.pointSize()));
             BraceMatching->setCurrentText("Sloppy Brace Match");


         }
         else if(pressedItem->text(0)=="Text Setting"){

             //text setting
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::Comment).name());
             StyleButtonList->at(0)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::CommentLine).name());
             StyleButtonList->at(1)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::CommentDoc).name());
             StyleButtonList->at(2)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::Number).name());
             StyleButtonList->at(3)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::Keyword).name());
             StyleButtonList->at(4)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::DoubleQuotedString).name());
             StyleButtonList->at(5)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::SingleQuotedString).name());
             StyleButtonList->at(6)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::UUID).name());
             StyleButtonList->at(7)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::PreProcessor).name());
             StyleButtonList->at(8)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::Operator).name());
             StyleButtonList->at(9)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::Identifier).name());
             StyleButtonList->at(10)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::UnclosedString).name());
             StyleButtonList->at(11)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::VerbatimString).name());
             StyleButtonList->at(12)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::Regex).name());
             StyleButtonList->at(13)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::CommentLineDoc).name());
             StyleButtonList->at(14)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::KeywordSet2).name());
             StyleButtonList->at(15)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::CommentDocKeyword).name());
             StyleButtonList->at(16)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::CommentDocKeywordError).name());
             StyleButtonList->at(17)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::GlobalClass).name());
             StyleButtonList->at(18)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::RawString).name());
             StyleButtonList->at(19)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::TripleQuotedVerbatimString).name());
             StyleButtonList->at(20)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::HashQuotedString).name());
             StyleButtonList->at(21)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::PreProcessorComment).name());
             StyleButtonList->at(22)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::PreProcessorCommentLineDoc).name());
             StyleButtonList->at(23)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::UserLiteral).name());
             StyleButtonList->at(24)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::TaskMarker).name());
             StyleButtonList->at(25)->setStyleSheet(styleSheet);
             styleSheet = QString("background-color: %1; color: white").arg(textLexer->defaultColor(QsciLexerCPP::EscapeSequence).name());
             StyleButtonList->at(26)->setStyleSheet(styleSheet);

             WordFontButtonFont.setFamily("Times New Roman");
             WordFontButtonFont.setPointSize(12);
             WordFontButton->setFont(WordFontButtonFont);
             WordFontButton->setText(QString("Font: %1 / Size: %2").arg(WordFontButtonFont.family()).arg(WordFontButtonFont.pointSize()));

             PaperColorButton->setStyleSheet("background-color: white;");

         }

         return;

      }

}

void tool_preference::itemPressedSlot(QTreeWidgetItem * pressedItem, int column){

    LayoutRight->removeWidget(Preference);
    Preference->setVisible(false);
    this->update();
    Preference = new QWidget;

    if(pressedItem->text(column)=="Information")
        Preference->setLayout(Info);
    else if(pressedItem->text(column)=="Build")
        Preference->setLayout(BuildToolsLayout);
    else if(pressedItem->text(column)=="Build Tools")
        Preference->setLayout(BuildToolsLayout);
    else if(pressedItem->text(column)=="Build Setting")
        Preference->setLayout(BuildSettingLayout);
    else if(pressedItem->text(column)=="Link")
        Preference->setLayout(LinkLibaryLayout);
    else if(pressedItem->text(column)=="Link Include")
        Preference->setLayout(LinkLibaryLayout);
    else if(pressedItem->text(column)=="Link Setting")
        Preference->setLayout(LinkSettingLayout);
    else if(pressedItem->text(column)=="Editor")
        Preference->setLayout(BasePropertisLayout);
    else if(pressedItem->text(column)=="Base Setting")
        Preference->setLayout(BasePropertisLayout);
    else if(pressedItem->text(column)=="Support Setting")
        Preference->setLayout(FoldMarginLayout);
    else if(pressedItem->text(column)=="Text Setting")
        Preference->setLayout(TextPropertisLayout);
    LayoutRight->insertWidget(0,Preference);
    LayoutRight->update();
    WidgetRight->update();
    this->update();

}

void tool_preference::InitWidget(){

    //Home Widget
    LayoutAll = new QHBoxLayout;

    //Select Tree
    SelectWidget =new QTreeWidget;
    SelectWidget->setFixedWidth(165);
    SelectWidget->setHeaderHidden(true);

    //Info Tree
    SelectInfo = new QTreeWidgetItem;
    SelectInfo->setIcon(0,QIcon(":/new/prefix1/res/information"));
    SelectInfo->setText(0,"Information");
    SelectWidget->addTopLevelItem(SelectInfo);

    Info = new QVBoxLayout;
    Info->setMargin(0);
    SelectName = new QLabel("Information");
    SelectName->setFixedHeight(25);
    QFont SelectNameFont;
    SelectNameFont.setPointSize(12);
    SelectName->setFont(SelectNameFont);
    Spliter = new QLineEdit;
    Spliter->setFixedHeight(2);
    Spliter->setEnabled(false);
    ProjectName = new QLabel("Project Name     : ");
    ProjectLocation = new QLabel("Project Location : ");
    LastModified = new QLabel("Last Modified    : ");
    FileNums = new QLabel("Totle Files      : ");
    ProjectName->setFixedHeight(18);
    ProjectName->setWordWrap(true);
    ProjectLocation->setFixedHeight(18);
    ProjectLocation->setWordWrap(true);
    LastModified->setFixedHeight(18);
    LastModified->setWordWrap(true);
    FileNums->setFixedHeight(18);
    FileNums->setWordWrap(true);
    Info->addWidget(SelectName);
    Info->addWidget(Spliter);
    Info->addWidget(ProjectName);
    Info->addWidget(ProjectLocation);
    Info->addWidget(LastModified);
    Info->addWidget(FileNums);
    Info->setAlignment(Qt::AlignTop);

    //Build Tree
    SelectBuild = new QTreeWidgetItem;
    SelectBuild->setIcon(0,QIcon(":/new/prefix1/res/build"));
    SelectBuild->setText(0,"Build");
    BuildTools = new QTreeWidgetItem;
    BuildTools->setIcon(0,QIcon(":/new/prefix1/res/buildTools"));
    BuildTools->setText(0,"Build Tools");
    BuildSetting = new QTreeWidgetItem;
    BuildSetting->setIcon(0,QIcon(":/new/prefix1/res/buildSetting"));
    BuildSetting->setText(0,"Build Setting");
    SelectWidget->addTopLevelItem(SelectBuild);
    SelectBuild->addChild(BuildTools);
    SelectBuild->addChild(BuildSetting);

    //Build Setting
    BuildToolsLayout = new QVBoxLayout;
    BuildToolsLayout->setMargin(0);
    BuildToolsLayout->setAlignment(Qt::AlignTop);

    SelectBuildToolsName = new QLabel("Build Tools");
    QFont SelectBuildNameFont;
    SelectBuildNameFont.setPointSize(12);
    SelectBuildToolsName->setFont(SelectBuildNameFont);
    SelectBuildToolsName->setFixedHeight(25);
    SelectBuildToolsSpliter = new QLineEdit;
    SelectBuildToolsSpliter->setFixedHeight(2);
    SelectBuildToolsSpliter->setEnabled(false);

    BuildToolsWidget =new QTableWidget;
    BuildToolsWidget->setFixedHeight(300);
    //BuildToolsWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    BuildToolsWidget->setRowCount(9);
    BuildToolsWidget->setColumnCount(3);
    BuildToolsWidget->verticalHeader()->setHidden(true);
    BuildToolsWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    BuildToolsWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    BuildToolsWidget->setColumnWidth(0,65);
    BuildToolsWidget->setColumnWidth(1,345);
    BuildToolsWidget->setColumnWidth(2,60);
    BuildToolsWidget->setHorizontalHeaderLabels(QStringList()<<tr("Name")<<tr("Tool Path")<<tr("Select"));

    GccName = new QLabel("Gcc");
    GccName->setAlignment(Qt::AlignCenter);
    GccPlusName = new QLabel("G++");
    GccPlusName->setAlignment(Qt::AlignCenter);
    GdbName = new QLabel("Gdb");
    GdbName->setAlignment(Qt::AlignCenter);
    LinkName = new QLabel("Link");
    LinkName->setAlignment(Qt::AlignCenter);
    ObjCopyName = new QLabel("ObjCopy");
    ObjCopyName->setAlignment(Qt::AlignCenter);
    ObjDumpName = new QLabel("ObjDump");
    ObjDumpName->setAlignment(Qt::AlignCenter);
    ArName = new QLabel("Ar");
    ArName->setAlignment(Qt::AlignCenter);
    ReadElfName = new QLabel("ReadElf");
    ReadElfName->setAlignment(Qt::AlignCenter);
    SizeName = new QLabel("Size");
    SizeName->setAlignment(Qt::AlignCenter);

    GccNameDir = new QPushButton("Change");
    GccPlusNameDir = new QPushButton("Change");
    GdbNameDir = new QPushButton("Change");
    LinkNameDir = new QPushButton("Change");
    ObjCopyNameDir = new QPushButton("Change");
    ObjDumpNameDir = new QPushButton("Change");
    ArNameDir = new QPushButton("Change");
    ReadElfNameDir = new QPushButton("Change");
    SizeNameDir = new QPushButton("Change");

    BuildToolsWidget->setCellWidget(0,0,GccName);
    BuildToolsWidget->setCellWidget(1,0,GccPlusName);
    BuildToolsWidget->setCellWidget(2,0,GdbName);
    BuildToolsWidget->setCellWidget(3,0,LinkName);
    BuildToolsWidget->setCellWidget(4,0,ObjCopyName);
    BuildToolsWidget->setCellWidget(5,0,ObjDumpName);
    BuildToolsWidget->setCellWidget(6,0,ArName);
    BuildToolsWidget->setCellWidget(7,0,ReadElfName);
    BuildToolsWidget->setCellWidget(8,0,SizeName);

    BuildToolsWidget->setItem(0,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(1,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(2,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(3,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(4,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(5,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(6,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(7,1,new QTableWidgetItem(""));
    BuildToolsWidget->setItem(8,1,new QTableWidgetItem(""));

    BuildToolsWidget->setCellWidget(0,2,GccNameDir);
    BuildToolsWidget->setCellWidget(1,2,GccPlusNameDir);
    BuildToolsWidget->setCellWidget(2,2,GdbNameDir);
    BuildToolsWidget->setCellWidget(3,2,LinkNameDir);
    BuildToolsWidget->setCellWidget(4,2,ObjCopyNameDir);
    BuildToolsWidget->setCellWidget(5,2,ObjDumpNameDir);
    BuildToolsWidget->setCellWidget(6,2,ArNameDir);
    BuildToolsWidget->setCellWidget(7,2,ReadElfNameDir);
    BuildToolsWidget->setCellWidget(8,2,SizeNameDir);

    BuildToolsLayout->addWidget(SelectBuildToolsName);
    BuildToolsLayout->addWidget(SelectBuildToolsSpliter);
    BuildToolsLayout->addWidget(BuildToolsWidget);

    //Build Tools
    BuildSettingLayout = new QVBoxLayout;
    BuildSettingLayout->setMargin(0);
    BuildSettingLayout->setAlignment(Qt::AlignTop);

    SelectBuildSettinName = new QLabel("Build Setting");
    QFont SelectBuildSettingNameFont;
    SelectBuildSettingNameFont.setPointSize(12);
    SelectBuildSettinName->setFont(SelectBuildSettingNameFont);
    SelectBuildSettinName->setFixedHeight(25);
    SelectBuildSettinSpliter = new QLineEdit;
    SelectBuildSettinSpliter->setFixedHeight(2);
    SelectBuildSettinSpliter->setEnabled(false);

    //Warning
    WarningMessage = new QGroupBox("Warning Message");
    WarningMessageLayout = new QGridLayout;
    SyntaxCheckOnly = new QRadioButton("Check Syntax Only");
    Pedantic = new QRadioButton("Pedantic");
    InhibitWarning = new QRadioButton("Inhibit All Warnings");
    AllWarning = new QRadioButton("Show All Warnings");
    WarningMessageLayout->addWidget(SyntaxCheckOnly,0,0);
    WarningMessageLayout->addWidget(Pedantic,0,1);
    WarningMessageLayout->addWidget(InhibitWarning,1,0);
    WarningMessageLayout->addWidget(AllWarning,1,1);
    WarningMessage->setLayout(WarningMessageLayout);

    //HardWare
    HardwareMessage = new QGroupBox("HardWare Setting");
    HardwareMessageLayout = new QGridLayout;
    V8Ext = new QCheckBox("V8 Extension");
    NoFPU = new QCheckBox("Not Enable FPU");
    SoftFloat = new QCheckBox("Soft Float");
    Mflat = new QCheckBox("Flat Register Window Model");
    HardwareMessageLayout->addWidget(V8Ext,0,0);
    HardwareMessageLayout->addWidget(NoFPU,0,1);
    HardwareMessageLayout->addWidget(SoftFloat,1,0);
    HardwareMessageLayout->addWidget(Mflat,1,1);
    HardwareMessage->setLayout(HardwareMessageLayout);

    //Opt
    OptMessage = new QGroupBox("Build Optimization");
    OptMessageLayout = new QGridLayout;
    OptLevelName = new QLabel("Optimization Select:");
    OptLevelName->setFixedWidth(132);
    OptLevel = new QComboBox;
    OptLevel->addItem("Optimize None(-O0)");
    OptLevel->addItem("Optimize(-O1)");
    OptLevel->addItem("Optimize More(-O2)");
    OptLevel->addItem("Optimize Most(-O3)");
    OtherOptName= new QLabel("Other Optimization:");
    OtherOpt = new QLineEdit;
    OptMessageLayout->addWidget(OptLevelName,1,1);
    OptMessageLayout->addWidget(OptLevel,1,2);
    OptMessageLayout->addWidget(OtherOptName,2,1);
    OptMessageLayout->addWidget(OtherOpt,2,2);
    OptMessage->setLayout(OptMessageLayout);

    //Other Option
    OtherMessage = new QGroupBox("Other Options");
    OtherMessageLayout = new QGridLayout;
    MakeFileTypeName = new QLabel("MakeFile Type:");
    MakeFileType = new QComboBox;
    MakeFileType->addItem("Auto Create Makefile");
    MakeFileType->addItem("Use Default Makefile");
    OtherParameterName = new QLabel("Other Build Parmaters:");
    OtherParameter = new QLineEdit;
    OtherMessageLayout->addWidget(MakeFileTypeName,0,0);
    OtherMessageLayout->addWidget(MakeFileType,0,1);
    OtherMessageLayout->addWidget(OtherParameterName,1,0);
    OtherMessageLayout->addWidget(OtherParameter,1,1);
    OtherMessage->setLayout(OtherMessageLayout);

    BuildSettingLayout->addWidget(SelectBuildSettinName);
    BuildSettingLayout->addWidget(SelectBuildSettinSpliter);
    BuildSettingLayout->addWidget(WarningMessage);
    BuildSettingLayout->addWidget(HardwareMessage);
    BuildSettingLayout->addWidget(OptMessage);
    BuildSettingLayout->addWidget(OtherMessage);

    //Link Tree
    SelectLink = new QTreeWidgetItem;
    SelectLink->setIcon(0,QIcon(":/new/prefix1/res/link"));
    SelectLink->setText(0,"Link");
    LinkSetting = new QTreeWidgetItem;
    LinkSetting->setIcon(0,QIcon(":/new/prefix1/res/linkSetting"));
    LinkSetting->setText(0,"Link Setting");
    LinkLibary = new QTreeWidgetItem;
    LinkLibary->setIcon(0,QIcon(":/new/prefix1/res/linkInclude"));
    LinkLibary->setText(0,"Link Include");
    SelectLink->addChild(LinkLibary);
    SelectLink->addChild(LinkSetting);
    SelectWidget->addTopLevelItem(SelectLink);

    //Link Libary
    LinkLibaryLayout = new QVBoxLayout;
    LinkLibaryLayout->setMargin(0);
    LinkLibaryLayout->setAlignment(Qt::AlignTop);
    SelectLinkLibaryName = new QLabel("Link Include");
    QFont SelectLinkLibaryNameFont;
    SelectLinkLibaryNameFont.setPointSize(12);
    SelectLinkLibaryName->setFont(SelectLinkLibaryNameFont);
    SelectLinkLibaryName->setFixedHeight(25);
    SelectLinkLibarySpliter = new QLineEdit;
    SelectLinkLibarySpliter->setFixedHeight(2);
    SelectLinkLibarySpliter->setEnabled(false);

    LowerLayout = new QHBoxLayout;
    LowerLayout->setMargin(0);
    LowerWidget = new QWidget;

    LinkLibaryWidget =new QTableWidget;
    LinkLibaryWidget->setRowCount(0);
    LinkLibaryWidget->setColumnCount(1);
    LinkLibaryWidget->verticalHeader()->setHidden(true);
    LinkLibaryWidget->horizontalHeader()->setHidden(true);
    LinkLibaryWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ButtonLayout = new QVBoxLayout;
    ButtonLayout->setMargin(0);
    ButtonLayout->setAlignment(Qt::AlignTop);
    ButtonWidget = new QWidget;

    AddLibary = new QPushButton("Add");
    DeleateLibary = new QPushButton("Deleate");
    ButtonLayout->addWidget(AddLibary);
    ButtonLayout->addWidget(DeleateLibary);
    ButtonWidget->setLayout(ButtonLayout);

    LowerLayout->addWidget(LinkLibaryWidget);
    LowerLayout->addWidget(ButtonWidget);
    LowerWidget->setLayout(LowerLayout);

    LinkLibaryLayout->addWidget(SelectLinkLibaryName);
    LinkLibaryLayout->addWidget(SelectLinkLibarySpliter);
    LinkLibaryLayout->addWidget(LowerWidget);

    //Link Setting
    LinkSettingLayout = new QVBoxLayout;
    LinkSettingLayout->setMargin(0);
    LinkSettingLayout->setAlignment(Qt::AlignTop);
    SelectLinkSettingName = new QLabel("Link Setting");
    QFont SelectLinkSettingNameFont;
    SelectLinkSettingNameFont.setPointSize(12);
    SelectLinkSettingName->setFont(SelectLinkSettingNameFont);
    SelectLinkSettingName->setFixedHeight(25);
    SelectLinkSettingSpliter = new QLineEdit;
    SelectLinkSettingSpliter->setFixedHeight(2);
    SelectLinkSettingSpliter->setEnabled(false);

    LinkAddress = new QGroupBox("Link Address");
    LinkAddressLayout = new QGridLayout;
    //LinkAddressLayout->setAlignment(Qt::AlignLeft);
    TextAddress = new QLabel("Text Address:");
    TextAddress->setFixedWidth(132);
    TextAddressEdit = new QLineEdit();
    DataAddress = new QLabel("Data Address:");
    DataAddressEdit = new QLineEdit() ;
    BssAddress = new QLabel("Bss Address:");
    BssAddressEdit = new QLineEdit() ;
    StackAddress = new QLabel("Stack Address:");
    StackAddressEdit = new QLineEdit() ;

    LinkAddressLayout->addWidget(TextAddress,0,0);
    LinkAddressLayout->addWidget(TextAddressEdit,0,1);
    LinkAddressLayout->addWidget(DataAddress,1,0);
    LinkAddressLayout->addWidget(DataAddressEdit,1,1);
    LinkAddressLayout->addWidget(BssAddress,2,0);
    LinkAddressLayout->addWidget(BssAddressEdit,2,1);
    //LinkAddressLayout->addWidget(StackAddress,3,0);
    //LinkAddressLayout->addWidget(StackAddressEdit,3,1);
    LinkAddress->setLayout(LinkAddressLayout);

    LinkOption = new QGroupBox("Link Options");
    LinkOptionLayout = new QGridLayout;
    NoDefaultLibs = new QCheckBox("Do Not Use Default Libraries");
    NoSharedLibraries = new QCheckBox("No Shared Libraries");
    DontPageAlign = new QCheckBox("Do Not Page Align Data");
    OmitAllSymbol = new QCheckBox("Omit All Symbol Information");
    LinkOptionLayout->addWidget(NoDefaultLibs,0,0);
    LinkOptionLayout->addWidget(NoSharedLibraries,0,1);
    LinkOptionLayout->addWidget(DontPageAlign,1,0);
    LinkOptionLayout->addWidget(OmitAllSymbol,1,1);
    LinkOption->setLayout(LinkOptionLayout);

    LinkOutput = new QGroupBox("Outputs Options");
    LinkOutputLayout = new QGridLayout;
    UseCkExe = new QCheckBox("Correction Instruction Sequence");
    ReadElf = new QCheckBox("Generate ReadElf File");
    LinkOutputLayout->addWidget(UseCkExe,0,0);
    LinkOutputLayout->addWidget(ReadElf,0,1);
    LinkOutput->setLayout(LinkOutputLayout);

    OtherLinkMessage = new QGroupBox("Other Options");
    OtherLinkMessageLayout = new QGridLayout;
    LinkFileTypeName = new QLabel("Link File Type:");
    LinkFileTypeName->setFixedWidth(132);
    LinkFileType = new QComboBox;
    LinkFileType->addItem("Auto Create Link File");
    LinkFileType->addItem("Use Default Link File");
    OtherLinkParameterName = new QLabel("Other Link Parmaters:");
    OtherLinkParameter = new QLineEdit;
    OtherLinkMessageLayout->addWidget(LinkFileTypeName,0,0);
    OtherLinkMessageLayout->addWidget(LinkFileType,0,1);
    OtherLinkMessageLayout->addWidget(OtherLinkParameterName,1,0);
    OtherLinkMessageLayout->addWidget(OtherLinkParameter,1,1);
    OtherLinkMessage->setLayout(OtherLinkMessageLayout);

    LinkSettingLayout->addWidget(SelectLinkSettingName);
    LinkSettingLayout->addWidget(SelectLinkSettingSpliter);
    LinkSettingLayout->addWidget(LinkAddress);
    LinkSettingLayout->addWidget(LinkOption);
    LinkSettingLayout->addWidget(LinkOutput);
    LinkSettingLayout->addWidget(OtherLinkMessage);

    //SelectEditorPropertis Tree
    SelectEditorPropertis = new QTreeWidgetItem;
    SelectEditorPropertis->setIcon(0,QIcon(":/new/prefix1/res/editor"));
    SelectEditorPropertis->setText(0,"Editor");
    SelectWidget->addTopLevelItem(SelectEditorPropertis);

    /*基础设置*/

    //是否使用tab作为缩进，否则为空格
    IndentationsUseTabs = new QCheckBox("Indentations Use Tabs");

    //是否显示缩进的标记
    SetIndentationGuides = new QCheckBox("Set Indentation Guides");

    //回车下一行时是否和上一行一样的缩进
    Autoindent = new QCheckBox("Auto Indent");

    //光标下的行的颜色是否可见
    CaretLineVisible = new QCheckBox("Caret Line Visible");

    //退格键取消缩进
    BackspaceUnindents = new QCheckBox("Backspace Unindents");

    //设置代码自动补全
    AutoCompletionSource = new QCheckBox("Auto Completion");

    BaseSetting = new QGroupBox("Base Setting");
    BaseSettingLayout = new QGridLayout();
    BaseSettingLayout->addWidget(IndentationsUseTabs,0,0);
    BaseSettingLayout->addWidget(SetIndentationGuides,1,0);
    BaseSettingLayout->addWidget(Autoindent,2,0);
    BaseSettingLayout->addWidget(CaretLineVisible,0,1);
    BaseSettingLayout->addWidget(BackspaceUnindents,1,1);
    BaseSettingLayout->addWidget(AutoCompletionSource,2,1);
    BaseSetting->setLayout(BaseSettingLayout);

    //换行方法
    WrapModeLabel = new QLabel("Wrap Mode:");
    WrapModeLabel->setFixedWidth(132);
    WrapMode = new QComboBox();
    WrapMode->addItem("Wrap None");
    WrapMode->addItem("Wrap Word");
    WrapMode->addItem("Wrap Character");
    WrapMode->addItem("Wrap Whitespace");

    //Tab缩进的个数
    IndentationSizeLabel = new QLabel("Tab Size:");
    IndentationSize = new QLineEdit("4");
    IndentationSize->setValidator(new QIntValidator());

    //选择光标颜色
    CaretForegroundColorLabel = new QLabel("Caret Upper Color:");
    CaretForegroundColorButton = new QPushButton();
    CaretForegroundColorButton->setStyleSheet("background-color: black;");

    //设置光标下的行的颜色
    CaretLineBackgroundColorLabel = new QLabel("Caret Back Color:");
    CaretLineBackgroundColorButton = new QPushButton();
    CaretLineBackgroundColorButton->setStyleSheet("background-color: gray;");

    //设置光标的宽度
    CaretWidthLabel = new QLabel("Caret Width:");
    CaretWidth = new QLineEdit("4");
    CaretWidth->setValidator(new QIntValidator());

    //设置选中文本的背景色
    SelectionBackgroundColorLabel = new QLabel("Selection Back Color:");
    SelectionBackgroundColorButton = new QPushButton();
    SelectionBackgroundColorButton->setStyleSheet("background-color: blue;");

    //设置选中文本的前景色
    SelectionForegroundColorLabel = new QLabel("Selection Upper Color:");
    SelectionForegroundColorButton = new QPushButton();
    SelectionForegroundColorButton->setStyleSheet("background-color: white;");

    OtherSetting = new QGroupBox("Other Setting");
    OtherSettingLayout = new QGridLayout();
    OtherSettingLayout->addWidget(WrapModeLabel,0,0);
    OtherSettingLayout->addWidget(WrapMode,0,1);
    OtherSettingLayout->addWidget(IndentationSizeLabel,1,0);
    OtherSettingLayout->addWidget(IndentationSize,1,1);
    OtherSettingLayout->addWidget(CaretWidthLabel,2,0);
    OtherSettingLayout->addWidget(CaretWidth,2,1);
    OtherSettingLayout->addWidget(CaretForegroundColorLabel,3,0);
    OtherSettingLayout->addWidget(CaretForegroundColorButton,3,1);
    OtherSettingLayout->addWidget(CaretLineBackgroundColorLabel,4,0);
    OtherSettingLayout->addWidget(CaretLineBackgroundColorButton,4,1);
    OtherSettingLayout->addWidget(SelectionBackgroundColorLabel,5,0);
    OtherSettingLayout->addWidget(SelectionBackgroundColorButton,5,1);
    OtherSettingLayout->addWidget(SelectionForegroundColorLabel,6,0);
    OtherSettingLayout->addWidget(SelectionForegroundColorButton,6,1);
    OtherSetting->setLayout(OtherSettingLayout);

    BasePropertis = new QTreeWidgetItem;
    BasePropertis->setIcon(0,QIcon(":/new/prefix1/res/baseSetting"));
    BasePropertis->setText(0,"Base Setting");
    SelectEditorPropertis->addChild(BasePropertis);

    BasePropertisLayout = new QVBoxLayout;
    BasePropertisLayout->setMargin(0);
    BasePropertisLayout->setAlignment(Qt::AlignTop);
    BasePropertisName = new QLabel("Base Setting");
    QFont BasePropertisFont;
    BasePropertisFont.setPointSize(12);
    BasePropertisName->setFont(BasePropertisFont);
    BasePropertisName->setFixedHeight(25);
    BasePropertisSpliter = new QLineEdit;
    BasePropertisSpliter->setFixedHeight(2);
    BasePropertisSpliter->setEnabled(false);

    BasePropertisLayout->addWidget(BasePropertisName);
    BasePropertisLayout->addWidget(BasePropertisSpliter);
    BasePropertisLayout->addWidget(BaseSetting);
    BasePropertisLayout->addWidget(OtherSetting);

    /*折叠和行号功能设置*/

    //代码函数折叠的样式
    FoldStyleLabel = new QLabel("Fold Style:");
    FoldStyleLabel->setFixedWidth(132);
    FoldStyle = new QComboBox();
    FoldStyle->addItem("No Fold Style");
    FoldStyle->addItem("Plain Fold Style");
    FoldStyle->addItem("Circled Fold Style");
    FoldStyle->addItem("Boxed Fold Style");
    FoldStyle->addItem("Circled Tree Fold Style");
    FoldStyle->addItem("Boxed Tree Fold Style");

    //设置函数折叠栏颜色
    FoldColorBackLabel = new QLabel("Fold Color Back:");
    FoldColorBackButton = new QPushButton();
    FoldColorBackButton->setStyleSheet("background-color: white;");

    FoldColorFrontLabel = new QLabel("Fold Color Front:");
    FoldColorFrontButton = new QPushButton();
    FoldColorFrontButton->setStyleSheet("background-color: white;");

    FoldSetting = new QGroupBox("Fold Setting");
    FoldSettingLayout = new QGridLayout();
    FoldSettingLayout->addWidget(FoldStyleLabel,0,0);
    FoldSettingLayout->addWidget(FoldStyle,0,1);
    FoldSettingLayout->addWidget(FoldColorBackLabel,1,0);
    FoldSettingLayout->addWidget(FoldColorBackButton,1,1);
    FoldSettingLayout->addWidget(FoldColorFrontLabel,2,0);
    FoldSettingLayout->addWidget(FoldColorFrontButton,2,1);
    FoldSetting->setLayout(FoldSettingLayout);

    //设置行号宽度
    MarginWidthLabel = new QLabel("Margin Width:");
    MarginWidth = new QLineEdit("15");
    MarginWidth->setValidator(new QIntValidator());
    MarginWidthLabel->setFixedWidth(132);

    //设置行号背景色
    MarginsBackgroundColorLabel = new QLabel("Margin Back Color:");
    MarginsBackgroundColorButton = new QPushButton();
    MarginsBackgroundColorButton->setStyleSheet("background-color: white;");

    //设置行号前景色
    MarginsForegroundColorLabel = new QLabel("Margin Upper Color:");
    MarginsForegroundColorButton = new QPushButton();
    MarginsForegroundColorButton->setStyleSheet("background-color: white;");

    //设置行号字体与大小
    MarginsFontLabel = new QLabel("Margin Font:");
    MarginsFontButton = new QPushButton();
    QFont MarginsButtonFont;
    MarginsButtonFont.setFamily("Times New Roman");
    MarginsButtonFont.setPointSize(12);
    MarginsFontButton->setFont(MarginsButtonFont);

    MarginSetting = new QGroupBox("Margin Setting");
    MarginSettingLayout = new QGridLayout();
    MarginSettingLayout->addWidget(MarginWidthLabel,0,0);
    MarginSettingLayout->addWidget(MarginWidth,0,1);
    MarginSettingLayout->addWidget(MarginsBackgroundColorLabel,1,0);
    MarginSettingLayout->addWidget(MarginsBackgroundColorButton,1,1);
    MarginSettingLayout->addWidget(MarginsForegroundColorLabel,2,0);
    MarginSettingLayout->addWidget(MarginsForegroundColorButton,2,1);
    MarginSettingLayout->addWidget(MarginsFontLabel,3,0);
    MarginSettingLayout->addWidget(MarginsFontButton,3,1);
    MarginSetting->setLayout(MarginSettingLayout);

    //设置括号匹配
    BraceMatchingLabel = new QLabel("Brace Matching:");
    BraceMatchingLabel->setFixedWidth(132);
    BraceMatching = new QComboBox();
    BraceMatching->addItem("No Brace Match");
    BraceMatching->addItem("Strict Brace Match");
    BraceMatching->addItem("Sloppy Brace Match");
    //Edit->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    BraceMatchingSetting = new QGroupBox("Brace Matching Setting");
    BraceMatchingLayout = new QGridLayout();
    BraceMatchingLayout->addWidget(BraceMatchingLabel,0,0);
    BraceMatchingLayout->addWidget(BraceMatching,0,1);
    BraceMatchingSetting->setLayout(BraceMatchingLayout);

    FoldMarginPropertis = new QTreeWidgetItem;
    FoldMarginPropertis->setIcon(0,QIcon(":/new/prefix1/res/supportSetting"));
    FoldMarginPropertis->setText(0,"Support Setting");
    SelectEditorPropertis->addChild(FoldMarginPropertis);

    FoldMarginLayout = new QVBoxLayout;
    FoldMarginLayout->setAlignment(Qt::AlignTop);
    FoldMarginLayout->setMargin(0);
    FoldMarginName = new QLabel("Support Setting");
    QFont FoldMarginFont;
    FoldMarginFont.setPointSize(12);
    FoldMarginName->setFont(FoldMarginFont);
    FoldMarginName->setFixedHeight(25);
    FoldMarginSpliter = new QLineEdit;
    FoldMarginSpliter->setFixedHeight(2);
    FoldMarginSpliter->setEnabled(false);

    FoldMarginLayout->addWidget(FoldMarginName);
    FoldMarginLayout->addWidget(FoldMarginSpliter);
    FoldMarginLayout->addWidget(FoldSetting);
    FoldMarginLayout->addWidget(MarginSetting);
    FoldMarginLayout->addWidget(BraceMatchingSetting);

    /*文本的颜色和大小设置*/

    //设置各特殊字符的颜色
    StyleColor = new QTableWidget();
    StyleColor = new QTableWidget(27,2);
    QStringList headerLabels;
    headerLabels << "Item" << "Color";
    StyleColor->setHorizontalHeaderLabels(headerLabels);
    StyleColor->setColumnWidth(0, 210);
    StyleColor->setColumnWidth(1, 200);
    StyleColor->setWordWrap(true);
    StyleColor->setShowGrid(false);
    StyleColor->setItem(0,0,new QTableWidgetItem("Comment"));
    StyleColor->setItem(1,0,new QTableWidgetItem("Comment Line"));
    StyleColor->setItem(2,0,new QTableWidgetItem("Comment Doc"));
    StyleColor->setItem(3,0,new QTableWidgetItem("Number"));
    StyleColor->setItem(4,0,new QTableWidgetItem("Keyword"));
    StyleColor->setItem(5,0,new QTableWidgetItem("Double Quoted String"));
    StyleColor->setItem(6,0,new QTableWidgetItem("Single Quoted String"));
    StyleColor->setItem(7,0,new QTableWidgetItem("UUID"));
    StyleColor->setItem(8,0,new QTableWidgetItem("Pre Processor"));
    StyleColor->setItem(9,0,new QTableWidgetItem("Operator"));
    StyleColor->setItem(10,0,new QTableWidgetItem("Identifier"));
    StyleColor->setItem(11,0,new QTableWidgetItem("Unclosed String"));
    StyleColor->setItem(12,0,new QTableWidgetItem("Verbatim String"));
    StyleColor->setItem(13,0,new QTableWidgetItem("Regex"));
    StyleColor->setItem(14,0,new QTableWidgetItem("Comment Line Doc"));
    StyleColor->setItem(15,0,new QTableWidgetItem("Keyword Set2"));
    StyleColor->setItem(16,0,new QTableWidgetItem("Comment Doc Keyword"));
    StyleColor->setItem(17,0,new QTableWidgetItem("Comment Doc Keyword Error"));
    StyleColor->setItem(18,0,new QTableWidgetItem("Global Class"));
    StyleColor->setItem(19,0,new QTableWidgetItem("Raw String"));
    StyleColor->setItem(20,0,new QTableWidgetItem("Triple Quoted Verbatim String"));
    StyleColor->setItem(21,0,new QTableWidgetItem("Hash Quoted String"));
    StyleColor->setItem(22,0,new QTableWidgetItem("Pre Processor Comment"));
    StyleColor->setItem(23,0,new QTableWidgetItem("Pre Processor Comment Line Doc"));
    StyleColor->setItem(24,0,new QTableWidgetItem("User Literal"));
    StyleColor->setItem(25,0,new QTableWidgetItem("Task Marker"));
    StyleColor->setItem(26,0,new QTableWidgetItem("Escape Sequence"));

    StyleButtonList = new QList<QPushButton*>();
    for(int i=0;i<27;i++){

        QPushButton *StyleColorButton = new QPushButton();
        StyleButtonList->append(StyleColorButton);
        StyleColorButton->setStyleSheet("background-color: white;");
        StyleColor->setCellWidget(i,1,StyleColorButton);

        connect(StyleColorButton, &QPushButton::clicked,this,[=]{

            QColor StyleColorS = QColorDialog::getColor(StyleColorButton->palette().color(QPalette::Button), nullptr, "Select Color");
            if (StyleColorS.isValid()) {
                QString styleSheet = QString("background-color: %1; color: white").arg(StyleColorS.name());
                StyleColorButton->setStyleSheet(styleSheet);
            }

        });

    }

    StyleColorSetting = new QGroupBox("Color Setting");
    StyleColorSettingLayout = new QVBoxLayout();
    StyleColorSettingLayout->addWidget(StyleColor);
    StyleColorSetting->setLayout(StyleColorSettingLayout);

    //设置字体
    WordFontLabel = new QLabel("Word Font:");
    WordFontLabel->setFixedWidth(132);
    WordFontButton = new QPushButton();
    QFont WordFontButtonFont;
    WordFontButtonFont.setFamily("仿宋");
    WordFontButtonFont.setPointSize(12);
    WordFontButton->setFont(WordFontButtonFont);

    //设置文本区域的背景色
    PaperColorLabel = new QLabel("Paper Color:");
    PaperColorButton = new QPushButton();
    PaperColorButton->setStyleSheet("background-color: white;");

    FonColorSetting = new QGroupBox("Style Setting");
    FonColorSettingLayout = new QGridLayout();
    FonColorSettingLayout->addWidget(WordFontLabel,0,0);
    FonColorSettingLayout->addWidget(WordFontButton,0,1);
    FonColorSettingLayout->addWidget(PaperColorLabel,1,0);
    FonColorSettingLayout->addWidget(PaperColorButton,1,1);
    FonColorSetting->setLayout(FonColorSettingLayout);

    TextPropertis = new QTreeWidgetItem;
    TextPropertis->setIcon(0,QIcon(":/new/prefix1/res/textSetting"));
    TextPropertis->setText(0,"Text Setting");
    SelectEditorPropertis->addChild(TextPropertis);

    TextPropertisLayout = new QVBoxLayout;
    TextPropertisLayout->setMargin(0);
    TextPropertisName = new QLabel("Text Setting");
    QFont TextPropertisFont;
    TextPropertisFont.setPointSize(12);
    TextPropertisName->setFont(FoldMarginFont);
    TextPropertisName->setFixedHeight(25);
    TextPropertisSpliter = new QLineEdit;
    TextPropertisSpliter->setFixedHeight(2);
    TextPropertisSpliter->setEnabled(false);

    TextPropertisLayout->addWidget(TextPropertisName);
    TextPropertisLayout->addWidget(TextPropertisSpliter);
    TextPropertisLayout->addWidget(StyleColorSetting);
    TextPropertisLayout->addWidget(FonColorSetting);
    TextPropertisLayout->setAlignment(Qt::AlignTop);

    //Preference Widget
    Preference = new QWidget;
    Preference->setLayout(Info);

    //Ok&Cancel Widget
    Default = new QPushButton("Default");
    Default->setFixedHeight(26);
    Default->setFixedWidth(80);
    Ok = new QPushButton("Active");
    Ok->setFixedHeight(26);
    Ok->setFixedWidth(80);
    Cancel = new QPushButton("Cancel");
    Cancel->setFixedHeight(26);
    Cancel->setFixedWidth(80);
    WidgetOkOrCancel = new QWidget;
    LayoutOkOrCancel = new QHBoxLayout;
    LayoutOkOrCancel->setMargin(0);
    LayoutOkOrCancel->addWidget(Default);
    LayoutOkOrCancel->addWidget(Ok);
    LayoutOkOrCancel->addWidget(Cancel);
    LayoutOkOrCancel->setAlignment(Qt::AlignRight);
    WidgetOkOrCancel->setLayout(LayoutOkOrCancel);
    WidgetOkOrCancel->setFixedHeight(28);

    //Right Widget
    WidgetRight = new QWidget;
    LayoutRight = new QVBoxLayout;
    LayoutRight->setMargin(0);
    LayoutRight->addWidget(Preference);
    LayoutRight->addWidget(WidgetOkOrCancel);
    WidgetRight->setLayout(LayoutRight);

    LayoutAll->addWidget(SelectWidget);
    LayoutAll->addWidget(WidgetRight);

    this->setFixedWidth(665);
    this->setFixedHeight(410);
    this->setLayout(LayoutAll);

    SelectWidget->expandAll();

}
