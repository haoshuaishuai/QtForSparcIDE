#ifndef COMPILE_PREFERENCE_H
#define COMPILE_PREFERENCE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCheckBox>
#include <QSplitter>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include "file.h"
#include "status_bar.h"

class tool_preference : public QWidget
{
    Q_OBJECT
public:

    explicit tool_preference(QWidget *parent = nullptr, file *FileMenu = nullptr,edit *Edit = nullptr,statusbar *StatusBar = nullptr,QWidget *MainWindow = nullptr);

    void InitWidget();

    void ConnectSignals();

    void RefreshIniFile(QString SdkPath);

    void ReadIni(QString SdkPath);

    void SetDefault();

    //Other Class
    QWidget *MainWindow;

    file *FileMenu;

    edit *Edit;

    statusbar *StatusBar;

private:

    //Home Layout
    QHBoxLayout *LayoutAll;

    /************************************/

    //Select Tree
    QTreeWidget *SelectWidget;

    //Info Items
    QTreeWidgetItem *SelectInfo;

    //Build Items
    QTreeWidgetItem *SelectBuild;
    QTreeWidgetItem *BuildTools;
    QTreeWidgetItem *BuildSetting;

    //Link Items
    QTreeWidgetItem *SelectLink;
    QTreeWidgetItem *LinkLibary;
    QTreeWidgetItem *LinkSetting;

    //Editor Items
    QTreeWidgetItem *SelectEditorPropertis;

    //Base Items
     QTreeWidgetItem *BasePropertis;

    //Fold&Margin Items
    QTreeWidgetItem *FoldMarginPropertis;

    //Text Items
    QTreeWidgetItem *TextPropertis;

    /************************************/

    //Right Widget
    QWidget* WidgetRight;
    QVBoxLayout *LayoutRight;

    /************************************/

    //Preference Widget
    QWidget *Preference;

    //Info Widget
    QVBoxLayout *Info;
    QLabel *SelectName;
    QLabel *ProjectName;
    QLabel *ProjectLocation;
    QLabel *LastModified;
    QLabel *FileNums;
    QLineEdit *Spliter;

    //Build Widget

    /*Build Tools*/
    QVBoxLayout *BuildToolsLayout;
    QTableWidget *BuildToolsWidget;
    QLabel *SelectBuildToolsName;
    QLineEdit *SelectBuildToolsSpliter;

    QLabel *GccName;
    QLabel *GccPlusName;
    QLabel *GdbName;
    QLabel *LinkName;
    QLabel *ObjCopyName;
    QLabel *ObjDumpName;
    QLabel *ArName;
    QLabel *ReadElfName;
    QLabel *SizeName;

    QPushButton *GccNameDir;
    QPushButton *GccPlusNameDir;
    QPushButton *GdbNameDir;
    QPushButton *LinkNameDir;
    QPushButton *ObjCopyNameDir;
    QPushButton *ObjDumpNameDir;
    QPushButton *ArNameDir;
    QPushButton *ReadElfNameDir;
    QPushButton *SizeNameDir;

     /*Build Setting*/
    QVBoxLayout *BuildSettingLayout;
    QLabel *SelectBuildSettinName;
    QLineEdit *SelectBuildSettinSpliter;

    //Warning
    QGroupBox * WarningMessage;
    QGridLayout * WarningMessageLayout;
    QRadioButton *SyntaxCheckOnly;
    QRadioButton *Pedantic;
    QRadioButton *InhibitWarning;
    QRadioButton *AllWarning;

    //Opt
    QGroupBox * OptMessage;
    QGridLayout * OptMessageLayout;
    QLabel *OptLevelName;
    QComboBox *OptLevel;
    QLabel *OtherOptName;
    QLineEdit *OtherOpt;

    //Other Parameter
    QGroupBox * OtherMessage;
    QGridLayout * OtherMessageLayout;
    QLabel *MakeFileTypeName;
    QComboBox *MakeFileType;
    QLabel *OtherParameterName;
    QLineEdit *OtherParameter;

    //Hardware
    QGroupBox * HardwareMessage;
    QGridLayout * HardwareMessageLayout;
    QCheckBox *V8Ext;
    QCheckBox *NoFPU;
    QCheckBox *SoftFloat;
    QCheckBox *Mflat;

    //Link Setting Widget
    QVBoxLayout *LinkSettingLayout;
    QLabel *SelectLinkSettingName;
    QLineEdit *SelectLinkSettingSpliter;

    QGroupBox * LinkAddress;
    QGridLayout * LinkAddressLayout;
    QLabel *TextAddress;
    QLineEdit *TextAddressEdit;
    QLabel *DataAddress;
    QLineEdit *DataAddressEdit;
    QLabel *BssAddress;
    QLineEdit *BssAddressEdit;
    QLabel *StackAddress;
    QLineEdit *StackAddressEdit;

    QGroupBox * LinkOption;
    QGridLayout * LinkOptionLayout;
    QCheckBox *NoDefaultLibs;
    QCheckBox *NoSharedLibraries;
    QCheckBox *DontPageAlign;
    QCheckBox *OmitAllSymbol;

    QGroupBox * LinkOutput;
    QGridLayout * LinkOutputLayout;
    QCheckBox *UseCkExe;
    QCheckBox *ReadElf;

    //Other Link Parameter
    QGroupBox * OtherLinkMessage;
    QGridLayout * OtherLinkMessageLayout;
    QLabel *LinkFileTypeName;
    QComboBox *LinkFileType;
    QLabel *OtherLinkParameterName;
    QLineEdit *OtherLinkParameter;

    //Link Libary Widget
    QVBoxLayout *LinkLibaryLayout;
    QLabel *SelectLinkLibaryName;
    QLineEdit *SelectLinkLibarySpliter;

    QHBoxLayout *LowerLayout;
    QWidget *LowerWidget;
    QTableWidget *LinkLibaryWidget;

    QWidget *ButtonWidget;
    QVBoxLayout *ButtonLayout;
    QPushButton *AddLibary;
    QPushButton *DeleateLibary;

    //Editor Propertis Widget

    QCheckBox *IndentationsUseTabs;
    QCheckBox *SetIndentationGuides;
    QCheckBox *Autoindent;
    QCheckBox *CaretLineVisible;
    QCheckBox *BackspaceUnindents;
    QCheckBox *AutoCompletionSource;

    QGroupBox *BaseSetting;
    QGridLayout *BaseSettingLayout;

    QLabel *WrapModeLabel;
    QComboBox *WrapMode;

    QLabel *IndentationSizeLabel;
    QLineEdit *IndentationSize;

    QLabel *CaretForegroundColorLabel;
    QPushButton *CaretForegroundColorButton;
    QColor CaretForegroundColor;

    QLabel *CaretLineBackgroundColorLabel;
    QPushButton *CaretLineBackgroundColorButton;
    QColor CaretLineBackgroundColor;

    QLabel *CaretWidthLabel;
    QLineEdit *CaretWidth;

    QLabel *SelectionBackgroundColorLabel;
    QPushButton *SelectionBackgroundColorButton;
    QColor SelectionBackgroundColor;

    QLabel *SelectionForegroundColorLabel;
    QPushButton *SelectionForegroundColorButton;
    QColor SelectionForegroundColor;

    QGroupBox *OtherSetting;
    QGridLayout *OtherSettingLayout;

    QVBoxLayout *TextPropertisLayout;
    QLabel *TextPropertisName;
    QLineEdit *TextPropertisSpliter;

    QVBoxLayout *BasePropertisLayout;
    QLabel *BasePropertisName;
    QLineEdit *BasePropertisSpliter;

    QComboBox *FoldStyle;
    QLabel *FoldStyleLabel;

    QLabel *FoldColorBackLabel;
    QPushButton *FoldColorBackButton;
    QColor FoldColorBack;

    QLabel *FoldColorFrontLabel;
    QPushButton *FoldColorFrontButton;
    QColor FoldColorFront;

    QGroupBox *FoldSetting;
    QGridLayout *FoldSettingLayout;

    QLabel *MarginWidthLabel;
    QLineEdit *MarginWidth;

    QLabel *MarginsBackgroundColorLabel;
    QPushButton *MarginsBackgroundColorButton;
    QColor MarginsBackgroundColor;

    QLabel *MarginsForegroundColorLabel;
    QPushButton *MarginsForegroundColorButton;
    QColor MarginsForegroundColor;

    QLabel *MarginsFontLabel;
    QPushButton *MarginsFontButton;
    QFont MarginsFont;

    QGroupBox *MarginSetting;
    QGridLayout *MarginSettingLayout;

    QLabel *BraceMatchingLabel;
    QComboBox *BraceMatching;

    QGroupBox *BraceMatchingSetting;
    QGridLayout *BraceMatchingLayout;

    QVBoxLayout *FoldMarginLayout;
    QLabel *FoldMarginName;
    QLineEdit *FoldMarginSpliter;

    QTableWidget *StyleColor;
    QGroupBox *StyleColorSetting;
    QVBoxLayout *StyleColorSettingLayout;
    QList<QPushButton*> *StyleButtonList;

    QLabel *WordFontLabel;
    QPushButton *WordFontButton;
    QFont WordFont;

    QLabel *PaperColorLabel;
    QPushButton *PaperColorButton;
    QColor PaperColor;

    QGroupBox *FonColorSetting;
    QGridLayout *FonColorSettingLayout;

    /************************************/

    //OkOrCancel Widget
    QWidget *WidgetOkOrCancel;
    QHBoxLayout *LayoutOkOrCancel;
    QPushButton *Default;
    QPushButton *Ok;
    QPushButton *Cancel;

     /************************************/

signals:

public slots:

void itemPressedSlot(QTreeWidgetItem * pressedItem, int column);

};

#endif // COMPILE_PREFERENCE_H
