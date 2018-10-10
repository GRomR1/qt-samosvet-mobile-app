#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QString>

#ifdef Q_OS_ANDROID
const int CENTER_SIZE = 550;
const int SIDE_SIZE = 200;
#else
const int CENTER_SIZE = 100;
const int SIDE_SIZE = 50;
#endif

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void buttonClicked(bool state);

private:
    Ui::Widget *ui;
    void sendCommand(QString light, bool state);

    void redToggled();
    void greenToggled();
    void yellowToggled();
};

#endif // WIDGET_H
