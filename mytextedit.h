#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QDebug>
#include <QKeyEvent>

class mytextedit : public QTextEdit{

    Q_OBJECT
    using QTextEdit::QTextEdit;

signals:
    void EditKeyPressEvent(QString str);

public:
    explicit mytextedit(QTextEdit *parent = nullptr);

public:
    void keyPressEvent(QKeyEvent *event){
        emit EditKeyPressEvent(event->text());
    };

};

#endif // MYTEXTEDIT_H
