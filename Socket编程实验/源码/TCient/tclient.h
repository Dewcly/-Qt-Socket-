#ifndef TCLIENT_H
#define TCLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <iostream>
#include <QtDebug>
#include <QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include <QList>

namespace Ui {
class TClient;
}

class TClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TClient(QWidget *parent = 0);
    ~TClient();
    static QString IP;
    static int port;

private:
    Ui::TClient *ui;
    QTcpSocket *socket;


private slots:
    void on_Login_clicked();  //点击登录按钮分别对应的槽函数
    void on_ForgetPsd_clicked();  //点击忘记密码对应的槽函数
    void on_RegistButton_clicked();  //这个槽函数是点击注册账户后进入的页面
    void readMsg(); //接受来自服务器的信息
};

#endif // TCLIENT_H
