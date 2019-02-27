#include "tclient.h"
#include "ui_tclient.h"
#include "regist.h"
#include "findpsd.h"
#include "mainwindow.h"
#include <QMovie>
#include <QPixmap>


TClient::TClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TClient)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie("F:/1.gif");
    ui->gif->setMovie(movie);     // 在标签中添加动画
    movie->start();
    ui->userHead->setPixmap(QPixmap("F:/2.png")); //在标签中插入图片
    socket=new QTcpSocket(this);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readMsg()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));   //发生错误时执行displayError函数
}

TClient::~TClient()
{
    delete this->socket;
    delete ui;
}

QString TClient::IP="10.12.68.195";
int TClient::port=6666;

void TClient::readMsg()
{
    QString data=socket->readAll();
    QStringList list=data.split("#");
    qDebug()<<data;
    if(list[0]=="b" && list[2]=="true")
    {
        Mainwindow *w=new Mainwindow;
        w->setWindowTitle(list[1]);
        w->show();
    }
    else if(list[0]=="b" && list[1]=="false"&&list[2]=='0')
            QMessageBox::information(this,"信息提示","登录失败,密码错误!",QMessageBox::Ok);
    else if(list[0]=="b" && list[1]=="false"&&list[2]=='2')
            QMessageBox::information(this,"信息提示","用户已在线，拒绝重复登录!",QMessageBox::Ok);
    else if(list[0]=="b" && list[1]=="false"&&list[2]=='3')
            QMessageBox::information(this,"信息提示","用户不存在，请先注册!",QMessageBox::Ok);
    else
        return;


}

void TClient::on_ForgetPsd_clicked()
{
    FindPsd *n=new FindPsd;
    n->setWindowTitle("FindPsd");
    n->show();//当点击忘记密码时，显示找回密码界面

}

void TClient::on_RegistButton_clicked()
{
    Regist *m=new Regist;
    m->setWindowTitle("Regist");
    m->show(); //当点击注册账户时，弹出注册窗口
}

void TClient::on_Login_clicked()
{

    socket->abort();   //取消已有的连接
    socket->connectToHost(IP,port); //连接服务器
    if(!socket->waitForConnected(30000))
    {
        qDebug() << "Connection failed!";
        QMessageBox::information(this,tr("错误"),tr("服务器未开启，请先开启服务器！"));
        ui->CountInput->clear();  //清空账号栏
        ui->PasswordInput->clear();  //清空密码栏
        ui->CountInput->setFocus();  //将鼠标重新定位到账号栏
        return;
    }
       qDebug() << "Connect successfully!";
       QString userName = ui->CountInput->text();
       QString passward = ui->PasswordInput->text();
       QString bs="b";
       QString data=bs+"#"+userName+"#"+passward;
       socket->write(data.toUtf8());

    if(userName == NULL)  // 输入为空的提示
    {
         QMessageBox::information(this,tr("错误"),tr("账号不能为空！"));
         ui->CountInput->setFocus();  //将鼠标重新定位到账号栏
    }
    else if(passward == NULL)
    {
         QMessageBox::information(this,tr("错误"),tr("密码不能为空！"));
         ui->CountInput->setFocus();
    }
    ui->CountInput->clear();  //清空账号栏
    ui->PasswordInput->clear();  //清空密码栏
    ui->CountInput->setFocus();  //将鼠标重新定位到账号栏
}


