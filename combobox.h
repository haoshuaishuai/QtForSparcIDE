#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QComboBox>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QProcess>
#include <QDebug>

class combobox : public QComboBox
{
public:
    explicit combobox(QWidget *parent = nullptr);
    void CtrlCEvent(QProcess *process);
    QProcess * Process;

protected:
    void keyPressEvent(QKeyEvent *event) override{

        if (event->key() == Qt::Key_Up) {
            int index = currentIndex() - 1;
            if (index < 0) {
                index = count() - 1;
            }
            setCurrentIndex(index);
            event->accept();
        } else if (event->key() == Qt::Key_Down) {
            int index = currentIndex() + 1;
            if (index >= count()) {
                index = 0;
            }
            setCurrentIndex(index);
            event->accept();
        } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C){
            CtrlCEvent(Process);
        }
        else {
            QComboBox::keyPressEvent(event);
        }

    }
};

#endif // COMBOBOX_H
