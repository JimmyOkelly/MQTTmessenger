#include "mydialog.h"
#include "mainwindow.h"
#include "personalchat.h"
#include<QDebug>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QCheckBox>
#include <QtWidgets/QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QStringList>

#include <QObject>
//#include <QHash>
//#include <QComboBox>
#include <QListWidget>
//#include <QMouseEvent>
//#include <QScrollBar>
#include <QListWidgetItem>
#include <QTest>





#include <iostream>
using namespace std;



//My dialog implementation.
Mydialog::Mydialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mydialog)

{
    ui->setupUi(this);



    //Used for both 1-1 and groiup chats.
    ui->hostlbl->setText("127.0.0.1"); //Local host.
    ui->portlbl->setValue(1883);
    ui->clientn->setText("Welcome!");
    //Subscribe to global on load:
    ui->topiclbl->setText("chat");


    //Show live time:
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start();

    //Show date:
    QDate date=QDate::currentDate();
    QString d = date.toString("dddd, d   MMMM, yyyy");
    ui->date->setText(d);


    //Using combo box to display friends:
    //Pull this data from table in database.
    //Then look to populate w for loop.
    //This loop is going to populate the pushbuttons on load.
    ui->comboBox->addItem("spen10");
    ui->comboBox->addItem("exampleuser");
    ui->comboBox->addItem("jamesok");
    ui->comboBox->addItem("fred");


    //Pull friends username from database.
    //stick into array.
    //Cycle through array and display in checkboxes.
//    QStringList friends = {"spen10", "exampleuser", "fred", "jamesok"};

//    QVBoxLayout *lay = new QVBoxLayout(this);
//    for(int i=0;(i < friends.length());i++) //Five represent number of friends.
//    {
//        QCheckBox *dynamic = new QCheckBox(friends[i]);
//        dynamic->setChecked(false);
//        lay->addWidget(dynamic);
//    }
    //ui->scrollArea->setLayout(lay);

    //Connecting the signal and slot for adding widgets.
    //QObject::connect(ui->addwidget_button, &QPushButton::clicked, this, &Mydialog::onAddWidget);


    //Sets placeholders for correct labels.
    ui->hostlbl->setPlaceholderText("Enter Host");
    ui->topiclbl->setPlaceholderText("Enter Topic");

    //Creates new MQTTCLient object.
    m_client = new QMqttClient(this);
    m_client->setHostname(ui->hostlbl->text());
    m_client->setPort(ui->portlbl->value());

    connect(m_client, &QMqttClient::stateChanged, this, &Mydialog::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &Mydialog::brokerDisconnected);

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message) {

          //User must be attempting a 1-1 chat.
          if (ui->topiclbl->text() == "chat")
            {
              const QString content =
              ui->reciever->text() + ":"
              + message
              + QLatin1Char('\n');

               //Therefore must have a sender and reciever in message:
               QString sub = "receiver:" + ui->welcomelbl->text() + "sender:" + ui->reciever->text();
               bool result;
               result = isPresent(message.toStdString(), sub.toStdString());
               if (result)
               {
                   QString final;
                   //If present then remove "receiver:**sender:**" from the message.
                   final = erase(content.toStdString(), sub.toStdString());
                   ui->meslog->insertPlainText(final + "\n");
               }
               else {
                   // ui->meslog->insertPlainText("Message received, cannot view since 1-1.,");
               }

            }else if (ui->topiclbl->text() == "global"){
                const QString content = message + QLatin1Char('\n');
                //Therefore only has sender in message. Receiver needs to see who sender is,
                //therefore leave message unchanged.
                ui->meslog->appendPlainText(content + "\n");
          }else if (ui->topiclbl->text() == "group"){
              const QString content =
              ui->reciever->text() + ":"
              + message
              + QLatin1Char('\n');

          }
          });

    connect(ui->hostlbl, &QLineEdit::textChanged, m_client, &QMqttClient::setHostname);
    connect(ui->portlbl, QOverload<int>::of(&QSpinBox::valueChanged), this, &Mydialog::setClientPort);
    updateLogStateChange();

    //Automatically Connect on login:
    on_connectBt_clicked();
    ui->meslog->appendPlainText("You are connected, click a chat channel to subscribe... \n\n");
    //Automatically subscribe:
    //subscribe();
   // ui->meslog->appendPlainText("You are in the 1-1 chatroom.\n\n");
    //Hide label:
    ui->topiclbl->hide();



}

Mydialog::~Mydialog()
{
    delete ui;
}

void Mydialog::onAddWidget()
{

//Add this code in when an add friend button is required.


}

void Mydialog::onRemoveWidget()
{
//      QPushButton* button = qobject_cast<QPushButton*>(sender());
//      QHBoxLayout* layout = mButtonToLayoutMap.take(button);
//      while (layout->count() != 0){
//          QLayoutItem* item = layout->takeAt(0);
//          delete item->widget();
//          delete item;
//      }
//      delete layout;
}

void Mydialog::on_globalChat_clicked()
{

    //Used to be topiclbl in this function when topic was entered.
    ui->topiclbl->setText("global");
    ui->clientn->setText("Global Chatroom");
    auto subscription = m_client->subscribe(ui->topiclbl->text());
       if (!subscription) {
           QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
           return;
       }
    ui->meslog->appendPlainText(("You have subscribed to the global chatroom:\n "));
    ui->clientn->setText(("Global Chatroom"));
    ui->topiclbl->hide();
}
void Mydialog::on_groupChat_clicked()
{
    ui->topiclbl->setText("group");
  // ui->clientn->setText("Groupchat with" + (ui.);

}

void Mydialog::on_addToGroup_clicked()
{
        //   ui->comboBox->currentText()
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
    //1) connect to database.
    // Connecting to my MYSQL database

//    //Get the database.
//    MainWindow w;
//    w.init_database();
//    QSqlDatabase db = w.get_database();
//    bool db_status = w.get_db_status();
//    QCOMPARE(db_status, db.open());


//    //2) get user_list(id) number from your own username.
//    if(db_status){
//            ui->meslog->appendPlainText("the database is open");
//            QString un = ui->welcomelbl->text();
//            QSqlQuery qry;//(QSqlDatabase::database("MyConnect"));
//            qry.prepare(QString("SELECT id FROM user_list WHERE username = :username;"));
//            qry.bindValue(":username",un);
//            qry.exec();

//            //qry.isActive();
//            //qry.isSelect();
//            if (qry.next())
//                {int user_id = qry.value(0).toInt();
//                ui->meslog->appendPlainText("first query executed");


//        //3)use that value ^ to return the JSON type of friend id's.
//                QSqlQuery qry2;//(QSqlDatabase::database("MyConnect"));
//                qry2.prepare(QString("SELECT friends FROM friend_list WHERE user_id = :user_id;"));
//                qry2.bindValue(":user_id",user_id);
//                qry2.exec();
//                if (qry2.next()){
//                    ui->meslog->appendPlainText("second qry executed.");
//                    QString mates = qry2.value(0).toString();
//                    ui->meslog->appendPlainText(mates);
//               }
//           }
//        }
  }


    //4) get usernames from that array of integers.
    //5) display those usernames in the combo box.
    //6) Add 'add friend button'.


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

    if (ui->topiclbl->text() == "chat")
    {
        //1-1 chat: therefore append receiver and sender into the message.
        QString message = "receiver:" + ui->reciever->text() + "sender:" + ui->welcomelbl->text() + ui->messagelbl->text();
        ui->meslog->appendPlainText(ui->welcomelbl->text() + ": " + ui->messagelbl->text() + "\n");
        //Publish function takes 2 arguements, topic and message.
        if (m_client->publish(ui->topiclbl->text(), message.toUtf8()) == -1)
               QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
        //Clear the message:
        ui->messagelbl->setText("");

    } else if (ui->topiclbl->text() == "global")
    {
        //Global chat: therefore only append sender into the message.
        //Only sender becuase there is no specific receivers in the global chat room.
        QString message = ui->welcomelbl->text() + ":" + ui->messagelbl->text() + "\n";
        //Publish function takes 2 arguements, topic and message.
        if (m_client->publish(ui->topiclbl->text(), message.toUtf8()) == -1)
               QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
        //Clear the message:
        ui->messagelbl->setText("");
    }
    else if (ui->topiclbl->text() == "group")
    {
        //Add checked recievers into message.
        //QString rec1 =
    }
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

void Mydialog::loginClicked(QString nomVal)
{
    nomValue = nomVal;
    ui->welcomelbl->setText(nomValue);
}

void Mydialog::on_chat11_clicked()
{

    ui->topiclbl->setText("chat");
    ui->reciever->setText(ui->comboBox->currentText());

    //Display 1-1 Chatroom :
    personalchat *pc = new personalchat;
    connect(this,SIGNAL(chatClicked(QString,QString,int,QString,QString)),pc, SLOT(chatClicked(QString,QString,int,QString,QString)));


    //Emit the signal;
    QString topic = ui->topiclbl->text();
    QString host = ui->hostlbl ->text();
    int port = ui->portlbl->value();
    QString receiver = ui->reciever->text();
    QString sender = ui->welcomelbl->text();

    emit chatClicked(topic, host, port, receiver, sender);

    //this->hide();
    pc->show();

//-----This is the code to chat in this window: use this infomation in the next window.

 //Used to be topiclbl in this function when topic was entered.
//        ui->topiclbl->setText("chat");
//        ui->reciever->setText(ui->comboBox->currentText());
//        auto subscription = m_client->subscribe(ui->topiclbl->text());
//           if (!subscription) {
//               QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
//               return;
//           }
//        ui->meslog->appendPlainText(("You have subscribed to the 1-1 chatroom:\n "));
//        ui->clientn->setText(("Private Chatroom"));

        //Add template to message sent including:
        //[sender: you][reciever: who you want to talk to][message:this is the actual message].
        //ui->messagelbl->setText("receiver:"+ ui->reciever->text() + "message:");
}

bool Mydialog::isPresent(string fullmessage, string sub1)
{
    bool result;
    size_t found;
    found = fullmessage.find(sub1);
    result = false;
    if ((found!=string::npos)){
       result = true;
    }
    return result;
}

void Mydialog::on_logout_clicked()
{
   this->hide();
   MainWindow *mainWindow = new MainWindow();
   mainWindow->show();
}




void Mydialog::showTime()
{
    QTime time = QTime::currentTime();
    QString t = time.toString("hh : mm : ss");
    ui->time->setText(t);
}

QString Mydialog::erase(string fullmessage, string sub)
{
        QString result;
        std::string::size_type i = fullmessage.find(sub);
        if (i != std::string::npos)
           fullmessage.erase(i, sub.length());
        result = QString::fromStdString(fullmessage);
        return result;

}

void Mydialog::subscribe()
{
    auto subscription = m_client->subscribe(ui->topiclbl->text());
        if (!subscription) {
            QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
            return;
        }
}
//End of MyDialog implementation.







