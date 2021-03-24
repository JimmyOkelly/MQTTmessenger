#include "personalchat.h"
#include "ui_personalchat.h"
#include<QDebug>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QCheckBox>
#include <QtWidgets/QMessageBox>
#include <QTimer>
#include <QDateTime>
#include "mydialog.h"


personalchat::personalchat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::personalchat)
{
    ui->setupUi(this);

    //Creates new MQTTCLient object.
    m_client = new QMqttClient(this);
    m_client->setHostname(ui->host->text());
    m_client->setPort(ui->port->value());
    ui->topic->setText("chat");

    connect(m_client, &QMqttClient::stateChanged, this, &personalchat::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &personalchat::brokerDisconnected);
    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message) {

                      const QString content =
                      ui->receiver->text() + ":"
                      + message
                      + QLatin1Char('\n');

               //User must be attempting a 1-1 chat.
               //Therefore must have a sender and reciever in message:

              QString sub = "sender:" + ui->receiver->text() + "receiver:" + ui->sender->text();
              bool result;
              result = isPresent(message.toStdString(), sub.toStdString());
               if (result)
               {
                  QString final;
                  //If present then remove "receiver:**sender:**" from the message.
                   final = erase(content.toStdString(), sub.toStdString());
                   ui->messagelogger->appendPlainText(final + "\n");
               }
               else {
                   // ui->meslog->insertPlainText("Message received, cannot view since 1-1.,");
               }

          });

    connect(ui->host, &QLineEdit::textChanged, m_client, &QMqttClient::setHostname);
    connect(ui->port, QOverload<int>::of(&QSpinBox::valueChanged), this, &personalchat::setClientPort);
    updateLogStateChange();


    ui->sender->hide();
    ui->host->hide();
    ui->port->hide();
    ui->topic->hide();
    ui->receiver->hide();

    //on_connect_clicked();
    //on_sub_clicked();


}

personalchat::~personalchat()
{
    delete ui;
}

void personalchat::brokerDisconnected()
{
    ui->host->setEnabled(true);
    ui->port->setEnabled(true);

}

void personalchat::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                        + QLatin1String(": State Change")
                        + QString::number(m_client->state())
                        + QLatin1Char('\n');
    ui->messagelogger->insertPlainText(content);
}

void personalchat::on_send_clicked()
{
    QString message = "sender:" + ui->sender->text() + "receiver:" + ui->receiver->text() + ui->message->text();
    ui->messagelogger->appendPlainText(ui->sender->text() + ": " + ui->message->text() + "\n");

    //Publish function takes 2 arguements, topic and message.
    if (m_client->publish(ui->topic->text(), message.toUtf8()) == -1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));

}

void personalchat::chatClicked(QString topic, QString host, int port, QString receiver, QString sender)
{
        pTopic = topic;
        pHost = host;
        pPort = port;
        pReceiver = receiver;
        pSender = sender;

        ui->topic->setText(pTopic);
        ui->host->setText(pHost);
        ui->port->setValue(pPort);
        ui->receiver->setText(pReceiver);
        ui->sender->setText(pSender);

        ui->displayReceiver->setText(pSender + ",  you are chatting to :  " + pReceiver);
}

bool personalchat::isPresent(string fullmessage, string sub)
{
    bool result;
    size_t found;
    found = fullmessage.find(sub);
    result = false;
    if ((found!=string::npos)){
       result = true;
    }
    return result;
}

QString personalchat::erase(string fullmessage, string sub)
{
    QString result;
    std::string::size_type i = fullmessage.find(sub);
    if (i != std::string::npos)
       fullmessage.erase(i, sub.length());
    result = QString::fromStdString(fullmessage);
    return result;
}

void personalchat::setClientPort(int p)
{
    m_client->setPort(p);
}


void personalchat::on_sub_clicked()
{
    auto subscription = m_client->subscribe(ui->topic->text());
        if (!subscription) {
            QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
            return;
        }
    ui->sub->hide();
    ui->messagelogger->appendPlainText("You have subscrubed successfullly.");
}

void personalchat::on_connect_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->host->setEnabled(false);
        ui->port->setEnabled(false);
        ui->connect->setText(tr("Disconnect"));
        m_client->connectToHost();
        ui->connect->hide();
        ui->messagelogger->appendPlainText("You are connected! Subscribe to start talking");
    } else {
        ui->host->setEnabled(true);
        ui->port->setEnabled(true);
        ui->connect->setText(tr("Connect"));
        m_client->disconnectFromHost();
    }
}









