#include "mydialog.h"
#include "ui_mydialog.h"
#include<QDebug>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>



#include <iostream>
using namespace std;

Mydialog::Mydialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mydialog)
{

    ui->setupUi(this);
    //Sets label to username:
    //QString s = welcomeUsername();

    //Sets placeholders for correct labels.
    ui->hostlbl->setPlaceholderText("Enter Host");
    ui->topiclbl->setPlaceholderText("Enter Topic");

    m_client = new QMqttClient(this);
    m_client->setHostname(ui->hostlbl->text());
    m_client->setPort(ui->portlbl->value());

    connect(m_client, &QMqttClient::stateChanged, this, &Mydialog::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &Mydialog::brokerDisconnected);

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
          const QString content = QDateTime::currentDateTime().toString()
                      + QLatin1String(" Received Topic: ")
                      + topic.name()
                      + QLatin1String(" Message: ")
                      + message
                      + QLatin1Char('\n');
          ui->meslog->insertPlainText(content);
          });

    connect(ui->hostlbl, &QLineEdit::textChanged, m_client, &QMqttClient::setHostname);
    connect(ui->portlbl, QOverload<int>::of(&QSpinBox::valueChanged), this, &Mydialog::setClientPort);
    updateLogStateChange();


}

Mydialog::~Mydialog()
{
    delete ui;
}

void Mydialog::on_subscribeBtn_clicked()
{
    auto subscription = m_client->subscribe(ui->topiclbl->text());
       if (!subscription) {
           QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
           return;
       }
}

void Mydialog::on_connectBt_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->hostlbl->setEnabled(false);
        ui->portlbl->setEnabled(false);
        ui->connectBt->setText(tr("Disconnect"));
        m_client->connectToHost();
    } else {
        ui->hostlbl->setEnabled(true);
        ui->portlbl->setEnabled(true);
        ui->connectBt->setText(tr("Connect"));
        m_client->disconnectFromHost();
    }
}

void Mydialog::on_quitBtn_clicked()
{
    QApplication::quit();
}

void Mydialog::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                        + QLatin1String(": State Change")
                        + QString::number(m_client->state())
                        + QLatin1Char('\n');
        ui->meslog->insertPlainText(content);
}

void Mydialog::on_publishBtn_clicked()
{
    if (m_client->publish(ui->topiclbl->text(), ui->messagelbl->text().toUtf8()) == -1)
           QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
     ui->messagelbl->setText("");
}

void Mydialog::brokerDisconnected()
{
    ui->hostlbl->setEnabled(true);
    ui->portlbl->setEnabled(true);
    ui->connectBt->setText(tr("Connect"));
}

void Mydialog::setClientPort(int p)
{
    m_client->setPort(p);
}
