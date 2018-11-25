#include "widget.h"
#include "ui_widget.h"

//%1 - left (right) radius
//%2 - width and height of left (right)
//%3 - center radius
//%4 - center width and height
QString myStyle (
"QFrame#left > QPushButton, QFrame#right > QPushButton {"
        "border-radius: %1px;"
        "min-width: %2px;"
        "min-height: %2px;"
        "max-width: %2px;"
        "max-height: %2px;"
        "border-width: 1px;"
"}"

//  greeen
"QFrame#center > QPushButton#greenCenter, "
"QFrame#left > QPushButton#greenLeft, "
"QFrame#right > QPushButton#greenRight {"
        "background-color: rgba(0, 255, 0, 15%);"
"}"
"QFrame#center > QPushButton#greenCenter:checked, "
"QFrame#left > QPushButton#greenLeft:checked, "
"QFrame#right > QPushButton#greenRight:checked {"
        "background-color: green;"
"}"

//  yellow
"QFrame#center > QPushButton#yellowCenter, "
"QFrame#left > QPushButton#yellowLeft, "
"QFrame#right > QPushButton#yellowRight {"
        "background-color: rgba(255, 255, 204, 35%);"
"}"
"QFrame#center > QPushButton#yellowCenter:checked, "
"QFrame#left > QPushButton#yellowLeft:checked, "
"QFrame#right > QPushButton#yellowRight:checked {"
        "background-color: yellow;"
"}"

//  red
"QFrame#center > QPushButton#redCenter, "
"QFrame#left > QPushButton#redLeft, "
"QFrame#right > QPushButton#redRight {"
        "background-color: rgba(255, 0, 0, 15%);"
"}"
"QFrame#center > QPushButton#redCenter:checked, "
"QFrame#left > QPushButton#redLeft:checked, "
"QFrame#right > QPushButton#redRight:checked {"
        "background-color: red;"
"}"

        // center sizes
"QFrame#center > QPushButton {"
        "border-radius: %3px;"
        "min-width: %4px;"
        "min-height: %4px;"
        "max-width: %4px;"
        "max-height: %4px;"
        "border-width: 1px;"
"}"
);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setStyleSheet(myStyle.arg(QString::number(SIDE_SIZE/2),
                                    QString::number(SIDE_SIZE),
                                    QString::number(CENTER_SIZE/2),
                                    QString::number(CENTER_SIZE)));

    QObject::connect(ui->redCenter, SIGNAL(toggled(bool)),
                     this, SLOT(buttonClicked(bool)));
    QObject::connect(ui->yellowCenter, SIGNAL(toggled(bool)),
                     this, SLOT(buttonClicked(bool)));
    QObject::connect(ui->greenCenter, SIGNAL(toggled(bool)),
                     this, SLOT(buttonClicked(bool)));

    nwam = new QNetworkAccessManager;
//    connect(nwam, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(replyFinish(QNetworkReply*)));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::replyFinish(QNetworkReply *reply)
{
//    qDebug() << ((reply->error() == QNetworkReply::NoError)? "Success!" : "Error!");
    QString answer = QString::fromUtf8(reply->readAll());
    qDebug() << "answer" << answer;
}

void Widget::buttonClicked(bool state)
{
    //    state == true // включить
    //    state == false // выключить
    QString lightName(sender()->objectName());
//    qDebug() << lightName << state;
    if ((lightName != "yellowCenter") && (lastPressedButton == lightName)){
        QPushButton *pb = qobject_cast<QPushButton *>(sender());
        if(pb){
            qDebug() << pb->objectName() << !state;
            pb->blockSignals(true);
            pb->setChecked(!state);
            pb->blockSignals(false);
        }
        return;
    }
    lastPressedButton = lightName;

    if (lightName == "redCenter"){
        sendCommand(lightName, state);
        if(state)
            redToggled();
    }
    else if (lightName == "yellowCenter") {
        sendCommand(lightName, state);
//        if(state)
        yellowToggled(state);
    }
    else if (lightName == "greenCenter") {
        sendCommand(lightName, state);
        if(state)
            greenToggled();
    }
}

void Widget::sendCommand(QString light, bool state)
{
    qDebug() << "sendCommand" << light << state;
//    QString   apiUrl(URL+light.remove("Center"));
//    QUrlQuery query;
//    query.addQueryItem("state", QString::number(state));
//    QUrl      url(apiUrl);
//    url.setQuery(query);

    QString   apiUrl(URL);
    QUrlQuery query; //  http://192.168.111.1/lights.lua?color=red&state=1&submit=Submit
    query.addQueryItem("color", light.remove("Center"));
    query.addQueryItem("state", QString::number(state));
    query.addQueryItem("submit", "Submit");
    QUrl      url(apiUrl);
    url.setQuery(query);

    QNetworkRequest request;
    request.setUrl(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
//    request.setRawHeader("Content-Transfer-Encoding","binary");

//    nwam->get(request);
    nwam->get(QNetworkRequest(url));
}

void Widget::redToggled()
{
    this->ui->yellowCenter->blockSignals(true);
    this->ui->greenCenter->blockSignals(true);

    this->ui->yellowCenter->setChecked(false);
    this->ui->greenCenter->setChecked(false);

    this->ui->yellowCenter->blockSignals(false);
    this->ui->greenCenter->blockSignals(false);

    this->ui->greenLeft->setChecked(true);
    this->ui->greenRight->setChecked(true);
    this->ui->yellowLeft->setChecked(false);
    this->ui->yellowRight->setChecked(false);
    this->ui->redLeft->setChecked(false);
    this->ui->redRight->setChecked(false);
}


void Widget::yellowToggled(bool state)
{
    this->ui->greenCenter->blockSignals(true);
    this->ui->redCenter->blockSignals(true);

    this->ui->greenCenter->setChecked(false);
    this->ui->redCenter->setChecked(false);

    this->ui->greenCenter->blockSignals(false);
    this->ui->redCenter->blockSignals(false);

    this->ui->greenLeft->setChecked(false);
    this->ui->greenRight->setChecked(false);
    this->ui->yellowLeft->setChecked(state);
    this->ui->yellowRight->setChecked(state);
    this->ui->redLeft->setChecked(false);
    this->ui->redRight->setChecked(false);
}

void Widget::greenToggled()
{
    this->ui->yellowCenter->blockSignals(true);
    this->ui->redCenter->blockSignals(true);

    this->ui->yellowCenter->setChecked(false);
    this->ui->redCenter->setChecked(false);

    this->ui->yellowCenter->blockSignals(false);
    this->ui->redCenter->blockSignals(false);

    this->ui->greenLeft->setChecked(false);
    this->ui->greenRight->setChecked(false);
    this->ui->yellowLeft->setChecked(false);
    this->ui->yellowRight->setChecked(false);
    this->ui->redLeft->setChecked(true);
    this->ui->redRight->setChecked(true);
}
