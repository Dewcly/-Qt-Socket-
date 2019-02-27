#include "regist.h"
#include "ui_regist.h"


Regist::Regist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regist)
{
    ui->setupUi(this);
    socket = new QTcpSocket();

    //连接信号槽
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMSG()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
               this,SLOT(displayError(QAbstractSocket::SocketError)));   //发生错误时执行displayError函数


}

Regist::~Regist()
{
    delete this->socket;
    delete ui;
}
void Regist::on_pushButton_clicked()
{
    socket->abort();  //取消已有的连接
    socket->connectToHost(IP, port);//连接服务器

    //等待连接成功
    if(!socket->waitForConnected(30000))
    {
        qDebug() << "连接失败!";
        return;
    }
    qDebug() << "连接成功!";

    QString string1 = ui->lineEdit->text();//账号
    QString string2 = ui->lineEdit_2->text();//密码
    QString string3 = ui->lineEdit_3->text();//确认密码
    QString string4 = ui->lineEdit_4->text();//电话
//    QString string8 = ui->lineEdit_8->text();//密保问题
    QString string8 = ui->comboBox->currentText();//密保问题
    QString string7 = ui->lineEdit_7->text();//密保答案

    int num = string4.length();
    if(string1 == NULL)
        QMessageBox::information(this,tr("错误"),tr("账号不能为空!"));
    else if(string2 == NULL)
        QMessageBox::information(this,tr("错误"),tr("密码不能为空!"));
    else if(string3 == NULL)
        QMessageBox::information(this,tr("错误"),tr("请确认密码!"));
    else if(string2!=string3)
    {
        QMessageBox::information(this,tr("错误"),tr("两次密码不一致!"));
        ui->lineEdit_3->clear();  //清空密码栏
        ui->lineEdit_3->setFocus();  //将鼠标重新定位到账号栏
    }
    else if(string4 == NULL)
        QMessageBox::information(this,tr("错误"),tr("请输入手机号!"));
    else if(string8 == NULL)
        QMessageBox::information(this,tr("错误"),tr("请输入密保问题!"));
    else if(string7 == NULL)
        QMessageBox::information(this,tr("错误"),tr("请输入密保答案!"));
    else if(num != 11)
    {
        QMessageBox::information(this,tr("错误"),tr("手机号格式不对!"));
        return;
    }
    QString as="a";
    QString data=as+"#"+string1+"#"+string2+"#"+string4+"#"+string8+"#"+string7;
    socket->write(data.toUtf8());

}

void Regist::on_pushButton_2_clicked()
{
    Regist::close();
}

void Regist::readMSG()
{

       QString data = socket->readAll();
       QStringList list=data.split("#");
       if(list[0]=="a" && list[1]=="true")
       {
           QMessageBox::information(this,"信息提示","注册成功!",QMessageBox::Ok);
           Regist::close();
       }
       else if(list[0]=="a" && list[1]=="false")
           QMessageBox::information(this,"信息提示","注册失败,用户名已经被注册!",QMessageBox::Ok);
}
