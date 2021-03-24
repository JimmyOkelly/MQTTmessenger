#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include<QDebug>
#include <QObject>



using namespace std;

#include "mydialog.h"

//// database details
#define SQLDB "QMYSQL"
#define HOST "127.0.0.1"
#define USER "admin"
#define PASSWORD "GY1D6NhWPcxN"
#define DB_NAME "mysql"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
     MainWindow(QWidget *paent = nullptr);
    ~MainWindow();
     void init_database();
     bool get_db_status();
     bool db_flag=false;
     QSqlDatabase get_database();
     QSqlDatabase database1;

signals:
    void loginClicked(QString nomVal);


private slots:
    void on_createAccBtn_clicked();

    void on_loginBtn_clicked();

    void on_offBtn_clicked();


private:

    Ui::MainWindow *ui;
    Mydialog *dialog;
};

#endif // MAINWINDOW_H

