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
#include "ui_mydialog.h"



#include <string>
#include <algorithm>



using namespace std;



namespace Ui {
class Mydialog;
}

class Mydialog : public QDialog
{
    Q_OBJECT

public:
    explicit Mydialog(QWidget *parent = nullptr);
    ~Mydialog();

    /// @brief Handles user clicked add widget button.
    void onAddWidget();

    /// @brief Handles users clickin remove widget button.
    void onRemoveWidget();

private slots:
    //Buttons
    void on_globalChat_clicked();
    void on_connectBt_clicked();
    void on_publishBtn_clicked();
    void on_chat11_clicked();
    void on_logout_clicked();
    void on_groupChat_clicked();
    void on_addToGroup_clicked();
    void on_quitBtn_clicked();

    //Other Functions
    void brokerDisconnected();
    void updateLogStateChange();
    void loginClicked(QString nomVal);
    bool isPresent(string fullmessage, string sub);
    void showTime();
    QString erase(string fullmessage, string sub);
    void subscribe();



signals:
    void chatClicked(QString topic, QString host, int port, QString receiver, QString sender);


public slots:
    void setClientPort(int p);



private:
    Ui::Mydialog *ui;
    QMqttClient *m_client;
    QString nomValue;
    QHash<QPushButton*, QHBoxLayout*> mButtonToLayoutMap;

};

#endif // MYDIALOG_H
