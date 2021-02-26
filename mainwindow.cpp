#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user1.hpp"
#include<QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   ui->username->setPlaceholderText("Enter Username");
   ui->password->setPlaceholderText("Enter Password");
   ui->email->setPlaceholderText("Enter Email");
   ui->mobile->setPlaceholderText("Enter Mobile");
   ui->usernameL->setPlaceholderText("Please Enter Username");
   ui->passwordL->setPlaceholderText("Please Enter Password");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createAccBtn_clicked()
{
	  // Connecting to my MYSQL database
	    database = QSqlDatabase::addDatabase("QMYSQL");
        database.setHostName("127.0.0.1");
	    database.setUserName("admin");
	    database.setPassword("GY1D6NhWPcxN");
	    database.setDatabaseName("mysql");

    if(database.open()){

        // Get data from the input fields
        QString username = ui->username->text();
        QString password = ui->password->text();
        QString email = ui->email->text();
        QString mobile = ui->mobile->text();

        //now this Query will insert the captured data above in to database

        QSqlQuery qry;

        qry.prepare("INSERT INTO MQTTdb (username,password,email,mobile)"
                    "VALUES (:username,:password,:email,:mobile)");

        qry.bindValue(":username", username);
        qry.bindValue(":password", password);
        qry.bindValue(":email", email);
        qry.bindValue(":mobile", mobile);


        if(qry.exec()){

            QMessageBox::information(this, "Inserted", "Data is Inserted Succesfully");
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
      database = QSqlDatabase::addDatabase("QMYSQL");
      database.setHostName("127.0.0.1");
      database.setUserName("admin");
      database.setPassword("GY1D6NhWPcxN");
      database.setDatabaseName("mysql");

    if (database.open()){
        QMessageBox::information(this,"DATABASE CONNECTED", "Database connection ");

        //Get user data input
        QString username = ui->usernameL->text();
        QString password = ui->passwordL->text();

        //queries db to find user&pass
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare(QString("SELECT * FROM MQTTdb WHERE username = :username AND password = :password"));

        query.bindValue(":username",username);
        query.bindValue(":password",password);

        //results of the query

        //condition below is if condition fails to execute
        //Returns true if the query is executed successfully; otherwise returns false.
        query.exec();

        if(query.next()){
                QMessageBox::information(this,"Success","You are logged in");

                 user1 firstman {":username" , "example@gmail.com" , "011000000"};

                Mydialog dialog;
                dialog.setModal(true);
                this->hide();
                dialog.exec();

          } else{

                 QMessageBox::information(this,"Error","Wrong password or username");

           }



    }else {
        QMessageBox::information(this,"DATABASE FAILED", "Database connection failed");
    }

}

