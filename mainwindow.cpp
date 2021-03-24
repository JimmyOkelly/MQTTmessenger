#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>
#include "mydialog.h"
#include <QObject>
#include <QTest>


using namespace std;

QString password;
QString username;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   //Sets placeholders for the correct labels
   ui->username->setPlaceholderText("Enter Username");
   ui->password->setPlaceholderText("Enter Password");
   ui->email->setPlaceholderText("Enter Email");
   ui->mobile->setPlaceholderText("Enter Mobile");
   ui->usernameL->setPlaceholderText("Please Enter Username");
   ui->passwordL->setPlaceholderText("Please Enter Password");
  //Show date:
   QDate date=QDate::currentDate();
   QString d = date.toString(" dddd, d   MMMM, yyyy");
   ui->date->setText(d);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_database()
{
      database1 = QSqlDatabase::addDatabase(SQLDB);
      database1.setHostName(HOST);
      database1.setUserName(USER);
      database1.setPassword(PASSWORD);
      database1.setDatabaseName(DB_NAME);
      MainWindow::db_flag=true;
}

QSqlDatabase MainWindow::get_database()
{
    return database1;
}

bool MainWindow::get_db_status()
{
     return MainWindow::db_flag;
}

void MainWindow::on_createAccBtn_clicked()
{
    // Connecting to my MYSQL database
       if(!MainWindow::db_flag)
       {
           MainWindow::init_database();
       }

    if(database1.open()){

        // Get data from the input fields
        QString username = ui->username->text();
        QString password = ui->password->text();
        QString email = ui->email->text();
        QString mobile = ui->mobile->text();

        //now this Query will insert the captured data above in to database
       QSqlQuery qry(QSqlDatabase::database("MyConnect"));

        qry.prepare("INSERT INTO user_list (username,password,email,mobile)"
                    "VALUES (:username,:password,:email,:mobile)");

        //bindind the values for the sql query.
        qry.bindValue(":username", username);
        qry.bindValue(":password", password);
        qry.bindValue(":email", email);
        qry.bindValue(":mobile", mobile);


        if(qry.exec()){
            QMessageBox::information(this, "Inserted", "Data is Inserted Succesfully");
            ui->username->setText("");
            ui->password->setText("");
            ui->email->setText("");
            ui->mobile->setText("");
            //database1.close();
            //QSqlDatabase::removeDatabase("qt_sql_default_connection"); // correct
        }else {
            QMessageBox::information(this, "NOT Inserted", "Data is NOT Inserted Succesfully");
        }



    }else {

        QMessageBox::information(this,"Not Connected","Database is not connected");
    }


}

void MainWindow::on_loginBtn_clicked()
{
    // Connecting to my MYSQL database
       if(!MainWindow::db_flag)
       {
           MainWindow::init_database();
       }



    if (database1.open()){
        QMessageBox::information(this,"DATABASE CONNECTED", "Database connection ");

        //Get user data input
         username = ui->usernameL->text();
         password = ui->passwordL->text();

        //queries db to find user&pass
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare(QString("SELECT * FROM user_list WHERE username = :username AND password = :password"));

        query.bindValue(":username",username);
        query.bindValue(":password",password);

        //results of the query

        //condition below is if condition fails to execute
        //Returns true if the query is executed successfully; otherwise returns false.
        query.exec();




        if(query.next()){


            QMessageBox::information(this,"Success","You are logged in");

                //QString password = ui->password->text();
                //Create object of class user1.
                //user1 obj1(string username);

               // QString name = ui->usernameL->text();
               // name.toStdString();
               // user thisUser;
                //thisUser.username = name;

                dialog = new Mydialog;
                //creatiing the signal and slot for the values.
                connect(this,SIGNAL(loginClicked(QString)),dialog,SLOT(loginClicked(QString)));

                //Emit the signal;
                QString nom = ui->usernameL->text();
                emit loginClicked(nom);
                //dialog->setModal(true);
                this->hide();
                dialog->show();


          } else{
                 QMessageBox::information(this,"Error","Wrong password or username");
           }
    }else {
        QMessageBox::information(this,"DATABASE FAILED", "Database connection failed");
    }



};

void MainWindow::on_offBtn_clicked()
{QApplication::quit();}






