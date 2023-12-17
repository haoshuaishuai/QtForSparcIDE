#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QObject>
#include <QWidget>

class tools;
class serial;
class reg_calc;

class mywidget : public QWidget{

public:
    explicit mywidget(QString id, QWidget *parent = nullptr,tools *Tool= nullptr);
    explicit mywidget(QString id,serial *Serial = nullptr);
    explicit mywidget(QString id,reg_calc *regcal = nullptr);
    tools *Tool;
    serial *Serial;
    reg_calc *RegCalc;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString m_id;

};

#endif // MYWIDGET_H
