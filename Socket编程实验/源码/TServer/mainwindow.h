#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtDebug>
#include <QMessageBox>
#include <QListWidgetItem>      //列表框空间头文件
#include <QFileDialog>          //文件对话框控件
#include <QStringList>          //字符串容器
#include <QDir>                 //目录类头文件
#include <QString>
#include <QList>
#include <QTimer>

#include "mysql.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int checkLogIn(QString name,QString pwd);
    bool checkSignUp(QString name, QString pwd, QString tel,QString question,QString answer);
    int findpwd(QString name,QString tel,QString question,QString answer);

private slots:
    void Connect();   //连接客户端和服务端
    void ReadMSG();  //读取来自客户端的信息
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QTimer *timer;
    QSqlQuery *query;
};

#endif // MAINWINDOW_H
