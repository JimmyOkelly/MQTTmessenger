#ifndef PERSONALCHAT_H
#define PERSONALCHAT_H

#include <QDialog>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include<QDebug>
#include <QMqttClient>
#include "mqtt/async_client.h"
#include "ui_mydialog.h"
#include <string>
#include <algorithm>

using namespace std;

namespace Ui {
class personalchat;
}

class personalchat : public QDialog
{
    Q_OBJECT

public:
    explicit personalchat(QWidget *parent = nullptr);
    ~personalchat();

private slots:
        void brokerDisconnected();
        void updateLogStateChange();
        void on_send_clicked();
        void chatClicked(QString topic, QString host, int port, QString receiver, QString sender);
        bool isPresent(string fullmessage, string sub);
        QString erase(string fullmessage, string sub);
        void on_sub_clicked();
        void on_connect_clicked();


public slots:
    void setClientPort(int p);

private:
    Ui::personalchat *ui;
    QMqttClient *m_client;
    QString pTopic;
      QString pHost;
      int pPort;
      QString pReceiver;
      QString pSender;
};

#endif // PERSONALCHAT_H
