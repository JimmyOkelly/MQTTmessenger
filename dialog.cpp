#include "dialog.h"
#include "ui_dialog.h"
#include "user1.hpp"
#include "publisher.h"
#include<QDebug>

#include <iostream>
using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // Connecting to my MYSQL database
      database = QSqlDatabase::addDatabase("QMYSQL");
      database.setHostName("127.0.0.1");
      database.setUserName("admin");
      database.setPassword("GY1D6NhWPcxN");
      database.setDatabaseName("mysql");

       user1 firstman {":username" , "john1@email.com" , "011000000"};
       cout << firstman.username1 << endl;

       //Change this label.
       //ui->label1->setText("WELCOME JOHN");

}

Dialog::~Dialog()
{
    delete ui;
}
