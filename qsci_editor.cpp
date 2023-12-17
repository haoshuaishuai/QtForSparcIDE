#include "qsci_editor.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerlua.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"

#include <QDebug>
#include <QMenu>

qscieditor::qscieditor(QWidget *parent) : QsciScintilla{parent}{

    customAction = new QAction(tr("Jump Include File"), this);

}

void qscieditor::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_Control) {

        this->SendScintilla(QsciScintilla::SCI_STYLESETHOTSPOT, 11, true);
        this->SendScintilla(QsciScintilla::SCI_STYLESETHOTSPOT, 11, true);
        this->SendScintilla(QsciScintilla::SCI_SETHOTSPOTACTIVEFORE, true, QColor(Qt::blue));
        this->SendScintilla(QsciScintilla::SCI_SETHOTSPOTACTIVEUNDERLINE, true);

    }
    QsciScintilla::keyPressEvent(event);

}

void qscieditor::keyReleaseEvent(QKeyEvent *event){

    if (event->key() == Qt::Key_Control){
        this->SendScintilla(QsciScintilla::SCI_STYLESETHOTSPOT, 11, false);
    }
    QsciScintilla::keyReleaseEvent(event);

}

void qscieditor::contextMenuEvent(QContextMenuEvent *event){

    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();

    int pos, index;
    this->getCursorPosition(&pos, &index);
    QString line = this->text(pos);
    if (line.trimmed().startsWith("#include")) {
        QRegExp rx("\"(.*)\"");
        if (rx.indexIn(line) != -1) {
            customAction->setEnabled(true);
        }else{
            customAction->setEnabled(false);
        }
    }else{
        customAction->setEnabled(false);
    }

    menu->addAction(customAction);
    menu->exec(event->globalPos());
    delete menu;

}

