#include "findpsd.h"
#include "ui_findpsd.h"
#include "tclient.h"

FindPsd::FindPsd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindPsd)
{
    ui->setupUi(this);

    socket=new QTcpSocket(this);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readMsg()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));   //发生错误时执行displayError函数

    ui->lineEdit_4->setFocus();  //鼠标定位
}

FindPsd::~FindPsd()
{
    delete this->socket;
    delete ui;
}

void FindPsd::on_pushButton_clicked()
{
    socket->abort();   //取消已有的连接
    socket->connectToHost(IP,port); //连接服务器
    //等待连接成功
    if(!socket->waitForConnected(30000))
    {
        qDebug() << "连接失败!";
        return;
    }
    qDebug() << "连接成功!";

    QString string1 = ui->lineEdit_4->text();//用户名
    QString string2 = ui->lineEdit_5->text();//手机号
    QString string3 = ui->comboBox->currentText();//密保问题
    QString string4 = ui->lineEdit_6->text();//密保答案
    QString cs="c";
    QString data=cs+"#"+string1+"#"+string2+"#"+string3+"#"+string4;
    socket->write(data.toUtf8());
}

void FindPsd::readMsg()
{
       QString data = socket->readAll();
       QStringList list=data.split("#");
       if(list[0]=="c" && list[1]=="true")
       {
           QString str=list[2];
           qDebug()<<list;
           QMessageBox::information(this,"找回密码",str,QMessageBox::Ok);
           FindPsd::close();
       }
       else if(list[0]=="c" && list[1]=="false"&&list[2]=="0")
       {
           QMessageBox::information(this,"信息提示","手机号错误,请重新输入!",QMessageBox::Ok);
           ui->lineEdit_5->clear();  //清空错误栏
           ui->lineEdit_5->setFocus();  //将鼠标重新定位
       }
       else if(list[0]=="c" && list[1]=="false"&&list[2]=="1")
       {
           QMessageBox::information(this,"信息提示","密保问题错误!",QMessageBox::Ok);
           ui->comboBox->setFocus();
       }
       else if(list[0]=="c" && list[1]=="false"&&list[2]=="2")
       {
            QMessageBox::information(this,"信息提示","密保答案错误!",QMessageBox::Ok);
            ui->lineEdit_6->clear();
            ui->lineEdit_6->setFocus();
       }
       else if(list[0]=="c" && list[1]=="false"&&list[2]=="4")
       {
           QMessageBox::information(this,"信息提示","用户不存在，请重新输入!",QMessageBox::Ok);
           ui->lineEdit_4->clear();
           ui->lineEdit_5->clear();
           ui->lineEdit_6->clear();
           ui->lineEdit_4->setFocus();

       }

}
