#ifndef EDITFIND_H
#define EDITFIND_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QScrollArea>
#include <QDockWidget>

#include "qsci_editor.h"
#include "file_tree.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qsciapis.h"


class LockerButton : public QPushButton{

    Q_OBJECT
public:
    /// @brief 构造方法
    explicit LockerButton(QWidget* parent = nullptr);

    /// @brief SetImageLabel
    /// 设置按钮图标
    void SetImageLabel(const QPixmap &pixmap);

    /// @brief SetTextLabel
    /// 设置按钮文字
    void SetTextLabel(QString text);

    /// @brief GetImageHandle
    /// 返回m_imageLabel
    QLabel* GetImageHandle();

    /// @brief GetImageHandle
    /// 返回m_textLabel
    QLabel* GetTextHandle();

private:
    // 按钮图标
    QLabel* m_imageLabel;

    // 按钮文字
    QLabel* m_textLabel;

};


class editfind : public QWidget
{
    Q_OBJECT

public:

    explicit editfind(QWidget *parent = nullptr,treemenu *ProjectTree = nullptr);

    void ConnectSignals();

    void InitWidget();

    void SetUpUI();

    void ConnectButtonSignals(LockerButton * FindFileButton,QWidget *FindButtonWidget);

    void add();

    bool Find(bool Next);

    void ReplaceWorld();

    void TransformFind();

    void EnterTransformFind();

    void TransformReplace();

public:

    QTabWidget *EditWidget;

    QList<qscieditor *> *EditList;

    QList<qscieditor *> *FindEditList;

    QTabWidget *MessageWidget;

    QScrollArea *ScrollArea;

    QDockWidget *Output;

    //other class
    treemenu *ProjectTree;

private:

    bool OldSelectStart;
    bool OldSelectedCase;
    bool OldSelectedWrap;
    bool OldSelectedWhole;
    bool OldSelectedRegular;
    QString OldFindString;
    QString OldFindFile;
    bool NewCursor;

public:

    QVBoxLayout *layoutall;

    //Find
    QWidget *FindWidget;
    QHBoxLayout *LayoutFind;
    QLabel *FindLabel;
    QComboBox *FindEdit;

    //Replace with
    QWidget *ReplaceWidget;
    QHBoxLayout *LayoutReplace;
    QLabel *ReplaceLabel;
    QComboBox *ReplaceEdit;

    //Direction
    QGroupBox *DirectionGroup;
    QHBoxLayout *LayoutDirectionGroup;
    QRadioButton *Forward;
    QRadioButton *Backward;

    //Scope
    QGroupBox *ScopeGroup;
    QHBoxLayout *LayoutScopeGroup;
    QRadioButton *All;
    QRadioButton *SelectedLines;

    //Direction & Scope
    QWidget *DirScoWidget;
    QHBoxLayout *DirSco;

    //option
    QGroupBox *OptionGroup;
    QHBoxLayout *LayoutOptionGroup;
    QCheckBox *CaseSens;
    QCheckBox *WrapSerach;
    QCheckBox *WholeWord;
    QCheckBox *Regular;
    QWidget *OptionWidget;
    QHBoxLayout *Option;

    //find button
    QWidget *buttonwidget1;
    QHBoxLayout *layoutbutton1;
    QPushButton *FindPrevious;
    QPushButton *FindNext;
    QPushButton *ReplaceFind;

    //replace button
    QWidget *buttonwidget2;
    QHBoxLayout *layoutbutton2;
    QPushButton *Replace;
    QPushButton *ReplaceAll;
    QPushButton *Advanced;

    //scope option
    QWidget *AdvancedScopeGroup;
    QHBoxLayout *LayoutScopGroup;
    QLabel *ScopeLabel;
    QRadioButton *ThisFile;
    QRadioButton *OpenFile;
    QRadioButton *OpenProject;

    //pattern option
    QWidget *PatternGroup;
    QHBoxLayout *LayoutPatternGroup;
    QLabel *PatternLabel;
    QComboBox *PatternEdit;
    QPushButton *AdvancedFind;

    //all advanced layout
    QGroupBox *AdvancedGroup;
    QVBoxLayout *AdvancedLayout;

    QWidget *AllWidget;
    QHBoxLayout *AllLayout;

    //find result widget
    QStringList *FindFile;
    QList<QStringList *> *FindFileString;

signals:

};

#endif // EDITFIND_H
