#ifndef MYBUTTON_H
#define MYBUTTON_H


#include <QObject>
#include <QPushButton>
#include <QMouseEvent>

/**
 * 由于原有的QPushButton类不能分辨鼠标左右键点击事件，这里需要继承它然后自定义两个信号。
 */
class MyButton : public QPushButton {
    //Q_OBJECT 是一个宏，添加它才能正常使用 Qt 的信号和槽机制
    Q_OBJECT
    //使用基类的构造函数
    using QPushButton::QPushButton;

signals:
    //定义两个信号函数
    void leftButtonClicked();

    void rightButtonClicked();

protected:
    void mousePressEvent(QMouseEvent *event) {

        if (event->button() == Qt::LeftButton)
                emit leftButtonClicked();        //判断鼠标左键点击

        if (event->button() == Qt::RightButton)
                emit rightButtonClicked();      //判断鼠标右键右键
    }
};


#endif //MYBUTTON_H
