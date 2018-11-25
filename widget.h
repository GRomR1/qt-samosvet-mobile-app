#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

#ifdef Q_OS_ANDROID
const int CENTER_SIZE = 550;
const int SIDE_SIZE = 200;
#else
const int CENTER_SIZE = 100;
const int SIDE_SIZE = 50;
#endif

//const QString URL = "http://127.0.0.1:5000/lights/center/";
const QString URL = "http://192.168.111.1/lights.lua";
//http://192.168.111.1/lights.lua?color=red&state=1&submit=Submit

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
    void replyFinish(QNetworkReply *reply);

private:
    Ui::Widget *ui;
    QNetworkAccessManager *nwam;
    QString lastPressedButton;
    void sendCommand(QString light, bool state);

    void redToggled();
    void greenToggled();
    void yellowToggled(bool state);
};

#endif // WIDGET_H
