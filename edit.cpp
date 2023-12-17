#include "qsci_editor.h"
#include "edit.h"
#include "file_tree.h"
#include "config_file.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"
#include "status_bar.h"

#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <edit_find.h>
#include <QDesktopWidget>
#include <QApplication>
#include <QSettings>
#include <QShortcut>

edit::edit(QWidget *Parent,treemenu *ProjectTree,configfile *SettingFile,statusbar *StatusBar){

    this->MainWindow  = Parent;    
    this->ProjectTree = ProjectTree;
    this->SettingFile = SettingFile;
    this->StatusBar = StatusBar;

}

void edit::InitWidget(){

    EditFind = new editfind(this,ProjectTree);
    EditFind->EditList = this->EditList;
    EditFind->EditWidget = this->EditWidget;
    EditFind->MessageWidget = this->MessageWidget;
    EditFind->ScrollArea = this->ScrollArea;
    EditFind->Output = this->Output;
    EditFind->InitWidget();
    EditFind->ConnectSignals();
    EditFind->setWindowFlags(EditFind->windowFlags() | Qt::WindowStaysOnTopHint);
    QDesktopWidget *desktop = QApplication::desktop();
    EditFind->move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);

}

void edit::SetEdit(qscieditor* edit){

    //set highleight
    QsciLexerCPP *textLexer = new QsciLexerCPP();
    edit->setLexer(textLexer);

    //show line number
    edit->setMarginType(0,QsciScintilla::NumberMargin);
    edit->setMarginWidth(0,40);
    edit->setMarginLineNumbers(0,true);

    //show break point
    edit->setMarginType(1,QsciScintilla::SymbolMargin); //设置1号页边显示符号
    edit->SendScintilla(QsciScintilla::SCI_MARKERSETFORE, 1,QColor(Qt::black)); //置标记前景和背景标记
    edit->SendScintilla(QsciScintilla::SCI_MARKERSETBACK, 1,QColor(Qt::red));
    edit->SendScintilla(QsciScintilla::SCI_SETMARGINMASKN,1,0x02); //1号页边显示2号标记
    edit->SendScintilla(edit->SCI_SETMARGINSENSITIVEN,1,true); //设置1号页边可以点击

    //set utf-8
    edit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);

    //set complete auto
    QsciAPIs *apis = new QsciAPIs(textLexer);
    QStringList keywords = {
        "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit",
        "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch",
        "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const",
        "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await",
        "co_return", "co_yield", "decltype", "default", "delete","do", "double",
        "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
        "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace",
        "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq",
        "private", "protected", "public", "register", "reinterpret_cast", "requires",
        "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast",
        "struct", "switch", "synchronized", "template", "this", "thread_local", "throw",
        "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
        "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
    };
    for (const QString &keyword : keywords) {
        apis->add(keyword);
    }
    apis->prepare();
    edit->setAutoCompletionSource(QsciScintilla::AcsAll);
    edit->setAutoCompletionCaseSensitivity(true);
    edit->setAutoCompletionThreshold(1);

    //set fold
    edit->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    edit->setFoldMarginColors(Qt::gray,Qt::lightGray);

    //other set
    edit->setAutoIndent(true);
    edit->setIndentationGuides(QsciScintilla::SC_IV_LOOKBOTH);
    edit->setCaretLineVisible(true);
    edit->setCaretLineBackgroundColor(Qt::lightGray);
    edit->setMarginsBackgroundColor(Qt::gray);
    edit->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    RefreshEdit(edit);

}

void edit::RefreshEdit(qscieditor* edit){

    QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
    QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

    GlobalSetting.beginGroup("EditBaseSetting");
    QString IndentationsUseTabsString =  GlobalSetting.value("IndentationsUseTabsString").toString();
    QString SetIndentationGuidesString = GlobalSetting.value("SetIndentationGuidesString").toString();
    QString AutoindentString = GlobalSetting.value("AutoindentString").toString();
    QString CaretLineVisibleString = GlobalSetting.value("CaretLineVisibleString").toString();
    QString BackspaceUnindentsString = GlobalSetting.value("BackspaceUnindentsString").toString();
    QString AutoCompletionSourceString = GlobalSetting.value("AutoCompletionSourceString").toString();
    GlobalSetting.endGroup();

    if(IndentationsUseTabsString == "true"){
        edit->setIndentationsUseTabs(true);
    }else{
        edit->setIndentationsUseTabs(false);
    }

    if(SetIndentationGuidesString == "true"){
        edit->setIndentationGuides(true);
    }else{
        edit->setIndentationGuides(false);
    }

    if(AutoindentString == "true"){
        edit->setAutoIndent(true);
    }else{
        edit->setAutoIndent(false);
    }

    if(CaretLineVisibleString == "true"){
        edit->setCaretLineVisible(true);
    }else{
        edit->setCaretLineVisible(false);
    }

    if(BackspaceUnindentsString == "true"){
        edit->setBackspaceUnindents(true);
    }else{
        edit->setBackspaceUnindents(false);
    }

    if(AutoCompletionSourceString == "true"){
        edit->setAutoCompletionSource(QsciScintilla::AcsAll);
        edit->setAutoCompletionThreshold(1);
    }else{
        edit->setAutoCompletionSource(QsciScintilla::AcsNone);
        edit->setAutoCompletionThreshold(-1);
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

    if(WrapModeString == "Wrap Word"){
        edit->setWrapMode(QsciScintilla::WrapWord);
    }else if(WrapModeString == "Wrap Character"){
        edit->setWrapMode(QsciScintilla::WrapCharacter);
    }else if(WrapModeString == "Wrap Whitespace"){
        edit->setWrapMode(QsciScintilla::WrapCharacter);
    }else{
        edit->setWrapMode(QsciScintilla::WrapNone);
    }

    edit->setTabWidth(QString(IndentationSizeString).toInt());
    edit->setCaretForegroundColor(QColor(CaretForegroundColorString));
    edit->setCaretLineBackgroundColor(QColor(CaretLineBackgroundColorString));
    edit->setCaretWidth(QString(CaretWidthString).toInt());
    edit->setSelectionBackgroundColor(QColor(SelectionBackgroundColorString));
    edit->setSelectionForegroundColor(QColor(SelectionForegroundColorString));

    GlobalSetting.beginGroup("EditFoldSetting");
    QString FoldStyleString = GlobalSetting.value("FoldStyleString").toString();
    QString FoldColorBackString = GlobalSetting.value("FoldColorBackString").toString();
    QString FoldColorFrontString = GlobalSetting.value("FoldColorFrontString").toString();
    GlobalSetting.endGroup();

    if(FoldStyleString == "Plain Fold Style"){
        edit->setFolding(QsciScintilla::PlainFoldStyle);
    }else if(FoldStyleString == "Circled Fold Style"){
        edit->setFolding(QsciScintilla::CircledFoldStyle);
    }else if(FoldStyleString == "Boxed Fold Style"){
        edit->setFolding(QsciScintilla::BoxedFoldStyle);
    }else if(FoldStyleString == "Circled Tree Fold Style"){
        edit->setFolding(QsciScintilla::CircledTreeFoldStyle);
    }else if(FoldStyleString == "Boxed Tree Fold Style"){
        edit->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    }else{
        edit->setFolding(QsciScintilla::NoFoldStyle);
    }
    edit->setFoldMarginColors(QColor(FoldColorBackString),QColor(FoldColorFrontString));

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

    edit->setMarginWidth(0,QString(MarginWidthString).toInt());
    edit->setMarginsBackgroundColor(QColor(MarginsBackgroundColorString));
    edit->setMarginsBackgroundColor(QColor(MarginsForegroundColorString));

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
    edit->setMarginsFont(MarginFont);
    if(BraceMatchingString == "Strict Brace Match"){
        edit->setBraceMatching(QsciScintilla::StrictBraceMatch);
    }else if(BraceMatchingString == "Sloppy Brace Match"){
        edit->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    }else{
        edit->setBraceMatching(QsciScintilla::NoBraceMatch);
    }

    GlobalSetting.beginGroup("Econde");
    QString SaveEconding = GlobalSetting.value("EditEconding").toString();;
    if(SaveEconding == "ANSI"){
        edit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CHARSET_ANSI);
    }else if(SaveEconding == "GB2312"){
        edit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CHARSET_GB2312);
    }else if(SaveEconding == "UTF8"){
        edit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
    }else{
        edit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CHARSET_DEFAULT);
    }
    GlobalSetting.endGroup();

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

    QsciLexerCPP* currentLexer = dynamic_cast<QsciLexerCPP*>(edit->lexer());
    currentLexer->setColor(QColor(Comment),QsciLexerCPP::Comment);
    currentLexer->setColor(QColor(CommentLine),QsciLexerCPP::CommentLine);
    currentLexer->setColor(QColor(CommentDoc),QsciLexerCPP::CommentDoc);
    currentLexer->setColor(QColor(Number),QsciLexerCPP::Number);
    currentLexer->setColor(QColor(DoubleQuotedString),QsciLexerCPP::DoubleQuotedString);
    currentLexer->setColor(QColor(SingleQuotedString),QsciLexerCPP::SingleQuotedString);
    currentLexer->setColor(QColor(UUID),QsciLexerCPP::UUID);
    currentLexer->setColor(QColor(PreProcessor),QsciLexerCPP::PreProcessor);
    currentLexer->setColor(QColor(Operator),QsciLexerCPP::Operator);
    currentLexer->setColor(QColor(Identifier),QsciLexerCPP::Identifier);
    currentLexer->setColor(QColor(UnclosedString),QsciLexerCPP::UnclosedString);
    currentLexer->setColor(QColor(VerbatimString),QsciLexerCPP::VerbatimString);
    currentLexer->setColor(QColor(Regex),QsciLexerCPP::Regex);
    currentLexer->setColor(QColor(CommentLineDoc),QsciLexerCPP::CommentLineDoc);
    currentLexer->setColor(QColor(KeywordSet2),QsciLexerCPP::KeywordSet2);
    currentLexer->setColor(QColor(CommentDocKeyword),QsciLexerCPP::CommentDocKeyword);
    currentLexer->setColor(QColor(CommentDocKeywordError),QsciLexerCPP::CommentDocKeywordError);
    currentLexer->setColor(QColor(GlobalClass),QsciLexerCPP::GlobalClass);
    currentLexer->setColor(QColor(RawString),QsciLexerCPP::RawString);
    currentLexer->setColor(QColor(TripleQuotedVerbatimString),QsciLexerCPP::TripleQuotedVerbatimString);
    currentLexer->setColor(QColor(HashQuotedString),QsciLexerCPP::HashQuotedString);
    currentLexer->setColor(QColor(PreProcessorComment),QsciLexerCPP::PreProcessorComment);
    currentLexer->setColor(QColor(PreProcessorCommentLineDoc),QsciLexerCPP::PreProcessorCommentLineDoc);
    currentLexer->setColor(QColor(UserLiteral),QsciLexerCPP::UserLiteral);
    currentLexer->setColor(QColor(TaskMarker),QsciLexerCPP::TaskMarker);
    currentLexer->setColor(QColor(EscapeSequence),QsciLexerCPP::EscapeSequence);

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
    currentLexer->setFont(WordFont);

    currentLexer->setPaper(QColor(PaperColorString));

}

void edit::RefreshEditList(){

    for(int i=0;i<EditList->size();i++){
        RefreshEdit(EditList->at(i));
    }

}

//connect signals
void edit::ConnectSignal(){

    connect(UnDo,&QAction::triggered,this,[=]{on_UnDo_triggered();});
    connect(ReDo,&QAction::triggered,this,[=]{on_ReDo_triggered();});
    connect(Cut,&QAction::triggered,this,[=]{on_Cut_triggered();});
    connect(Copy,&QAction::triggered,this,[=]{on_Copy_triggered();});
    connect(Paste,&QAction::triggered,this,[=]{on_Paste_triggered();});
    connect(Delate,&QAction::triggered,this,[=]{on_Delate_triggered();});
    connect(SelectAll,&QAction::triggered,this,[=]{on_SelectAll_triggered();});
    connect(FindOrReplace,&QAction::triggered,this,[=]{on_FindOrReplace_triggered();});
    connect(GoToLine,&QAction::triggered,this,[=]{on_GoToLine_triggered();});
    connect(SelectEconding,&QAction::triggered,this,[=]{on_SelectEconding_triggered();});

};

void edit::on_UnDo_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->undo();
            StatusBar->DetermineMessage("UnDo Success");
        }
    }

}

void edit::on_ReDo_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->redo();
            StatusBar->DetermineMessage("ReDo Success");
        }
    }

}

void edit::on_Cut_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->cut();
            StatusBar->DetermineMessage("Cut Success");
        }
    }

}

void edit::on_Copy_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->copy();
            StatusBar->DetermineMessage("Copy Success");
        }
    }

}

void edit::on_Paste_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->paste();
            StatusBar->DetermineMessage("Paste Success");
        }
    }

}

void edit::on_Delate_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->delete_selection();
            StatusBar->DetermineMessage("Delate Selection Success");
        }
    }

}

void edit::on_SelectAll_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            EditList->at(i)->selectAll(true);
            StatusBar->DetermineMessage("Select All Success");
        }
    }

}

void edit::on_FindOrReplace_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            if(EditList->at(i)->hasSelectedText()){
                EditFind->FindEdit->setEditText(EditList->at(i)->selectedText());
                break;
            }else{
                EditFind->FindEdit->setEditText("");
            }
        }
    }
    EditFind->show();
    EditFind->activateWindow();
    EditFind->FindEdit->setFocus();

}

void edit::on_GoToLine_triggered(){

    for(int i=0;i<EditList->size();i++){
        if(EditList->at(i)->whatsThis( )== EditWidget->currentWidget()->whatsThis()){
            bool Ok;
            QInputDialog dialog;
            QWidget * DialogIcon= new QWidget(this);
            DialogIcon->setWindowIcon(QIcon(":/new/prefix1/res/goLine"));
            int Value = dialog.getInt(DialogIcon,tr("Go To"),tr("Go To The Line："),1,1,EditList->at(i)->lines(),1,&Ok,Qt::WindowFlags());
            if(Ok){
                EditList->at(i)->setCursorPosition(Value-1,0);
                StatusBar->DetermineMessage("At Line:"+ QString::number(Value));

            }

        }
    }

}

void edit::on_SelectEconding_triggered(){

    QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
    QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

    GlobalSetting.beginGroup("Econde");
    QString SaveEconding = GlobalSetting.value("EditEconding").toString();;
    int index = 0;
    if(SaveEconding == "ANSI"){
        index = 1;
    }else if(SaveEconding == "GB2312"){
        index = 2;
    }else if(SaveEconding == "UTF8"){
        index = 3;
    }else{
        index = 0;
    }
    GlobalSetting.endGroup();

    QString EditEconding="UTF8";
    QStringList options;
    options << "DEFAULT" << "ANSI" << "GB2312" << "UTF8";
    bool ok;
    QInputDialog dialog;
    QWidget * DialogIcon= new QWidget(this);
    DialogIcon->setWindowIcon(QIcon(":/new/prefix1/res/encodeFile"));
    QString SelectedOption = dialog.getItem(DialogIcon, "Select Option", "Choose an option:", options, index, false, &ok);
    if (ok && !SelectedOption.isEmpty()) {

        if(SelectedOption == "ANSI"){
            for(int i=0;i<EditList->size();i++){
                EditList->at(i)->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CHARSET_ANSI);
                EditList->at(i)->update();
            }
            EditEconding = "ANSI";
        }else if(SelectedOption == "GB2312"){
            for(int i=0;i<EditList->size();i++){
                EditList->at(i)->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CHARSET_GB2312);
                EditList->at(i)->update();
            }
            EditEconding = "GB2312";
        }else if(SelectedOption == "UTF8"){
            for(int i=0;i<EditList->size();i++){
                EditList->at(i)->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
                EditList->at(i)->update();
            }
            EditEconding = "UTF8";
        }else{
            for(int i=0;i<EditList->size();i++){
                EditList->at(i)->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CHARSET_DEFAULT);
                EditList->at(i)->update();

            }
            EditEconding = "Default";
        }
        GlobalSetting.beginGroup("Econde");
        GlobalSetting.setValue("EditEconding", EditEconding);
        GlobalSetting.endGroup();
        StatusBar->DetermineMessage("Select Econding: "+SelectedOption);

    }

}
