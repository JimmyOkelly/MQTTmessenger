#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include<QDebug>
#include <QMqttClient>
#include "mqtt/async_client.h"
#include "mainwindow.h"

namespace Ui {
class Mydialog;
}

class Mydialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mydialog(QWidget *parent = nullptr);
    ~Mydialog();
private slots:
    //Buttons
    void on_subscribeBtn_clicked();
    void on_connectBt_clicked();
    void on_quitBtn_clicked();
    void on_publishBtn_clicked();
    //Other Functions
    void brokerDisconnected();
    void updateLogStateChange();
public slots:
    void setClientPort(int p);

private:
    Ui::Mydialog *ui;
    QMqttClient *m_client;
    QSqlDatabase database;
};

#endif // MYDIALOG_H
