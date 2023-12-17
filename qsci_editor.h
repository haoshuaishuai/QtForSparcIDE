#ifndef QSCIEDITOR_H
#define QSCIEDITOR_H

#include <QWidget>
#include <QEvent>
#include <QKeyEvent>

#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"

class qscieditor : public QsciScintilla
{
    Q_OBJECT
public:
    explicit qscieditor(QWidget *parent = nullptr);

    void contextMenuEvent(QContextMenuEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

public:
    QAction *customAction;

signals:

};

#endif // QSCIEDITOR_H
