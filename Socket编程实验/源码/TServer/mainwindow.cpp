#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mysql.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->server = new QTcpServer(this);
    this->socket = new QTcpSocket(this);
    int port= 6666;
    server->listen(QHostAddress::Any,port);//监听本机的某个端口
    //连接信号槽
    connect(server,SIGNAL(newConnection()),this,SLOT(Connect()));  //接受来自客户端的信号并连接
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
                this,SLOT(displayError(QAbstractSocket::SocketError)));
//    connect(socket,SIGNAL(readyRead()),this,SLOT(ReadMSG())); //连接成功则引发槽函数
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclicked(QListWidgetItem*))); //双击触发事件

    //加载用户列表
    QString str=NULL;
    query=new QSqlQuery;
    query->exec("select name from user ");

    while(query->next())
    {
        QString str=query->value(0).toString();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(str);      //设置列表项的文本
        ui->listWidget->addItem(item);     //加载列表项到列表框
    }



}

MainWindow::~MainWindow()
{
    server->close();
    delete ui;
}

int MainWindow::checkLogIn(QString name,QString pwd)
{
    MySql *sql=new MySql();
    int ret=sql->Login(name,pwd);
    return ret;
}

bool MainWindow::checkSignUp(QString name, QString pwd, QString tel,QString question,QString answer)
{
    MySql *sql=new MySql();
    bool ret=sql->Regist(name,pwd,tel,question,answer);
    return ret;
}

int MainWindow::findpwd(QString name,QString tel,QString question,QString answer)
{
    MySql *sql=new MySql();
    int temp=sql->findpwd(name,tel,question,answer);
    return temp;
}

void MainWindow::Connect()   //连接客户端和服务端
{
    socket=server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(ReadMSG()));   //获取来自客户端的数据
}

void MainWindow::ReadMSG()  //读取来自客户端的数据
{
    QString data = socket->readAll();   //读取客户端发送的数据
    qDebug()<<data;
    QStringList list=data.split("#");
    bool ret=0;
    int temp=0;
    QString sendData=list[0];
    QString password,str1,str2,str3,str4;
    if(list[0]=="a")     //注册
    {
        ret=checkSignUp(list[1],list[2],list[3],list[4],list[5]);
        if(ret)
        sendData=sendData+"#true";
        else
        sendData=sendData+"#false";
        socket->write(sendData.toLatin1());
    }
    else if(list[0]=="b")  //登录
    {
        temp=checkLogIn(list[1],list[2]);
        if(temp==1)
        {
            query=new QSqlQuery;
            query->exec(QString("update user set status='%1' where name='%2'").arg(1).arg(list[1])); //设置数据库中的status为1
            sendData=sendData+"#"+list[1]+"#true";
        }
        else if(temp==0)  sendData=sendData+"#false"+"#0";
        else if(temp==2)  sendData=sendData+"#false"+"#2";
        else if(temp==3)  sendData=sendData+"#false"+"#3";
        socket->write(sendData.toUtf8());

    }
    else if(list[0]=="c")  //找回密码
    {
        temp=findpwd(list[1],list[2],list[3],list[4]);
        if(temp==0) sendData=sendData+"#false"+"#0";
        else if(temp==1) sendData=sendData+"#false"+"#1";
        else if(temp==2) sendData=sendData+"#false"+"#2";
        else if(temp==4) sendData=sendData+"#false"+"#4";
        else
        {
            QString str=QString("select AES_DECRYPT(pwd,'key') from user where name='%1'").arg(list[1]);
            query=new QSqlQuery;
            query->exec(str);
            while(query->next())
            {
                password=query->value(0).toString();
            }
            sendData=sendData+"#true"+"#"+password;
        }
        socket->write(sendData.toUtf8());
    }
    else if(list[0]=="d")  //退出登录
    {
        query=new QSqlQuery;
        query->exec(QString("update user set status='%1' where name='%2'").arg(0).arg(list[1])); //设置数据库中的status为0

    }
    else if(list[0]=="e")  //返回用户列表
    {
        query=new QSqlQuery;
        query->exec(QString("update user set CIP='%1' , CPort='%2' where name='%3'").arg(list[2]).arg(list[3]).arg(list[1]));
        query=new QSqlQuery;
        query->exec("select name from user");
        QString str6=NULL;

        while(query->next())
        {
            str1=query->value(0).toString();
            str6=str6+"#"+str1;
        }

        query=new QSqlQuery;
        query->exec(QString("select name1,MSG from msg where name2='%1'").arg(list[1]));
        while(query->next())
        {
            str3=query->value(0).toString();
            str4=query->value(1).toString();
        }
        if(str4!=NULL)
        {
            str3=str3+":"+str4;
        }
        sendData=sendData+str6+"#"+"end"+"#"+str3;
        socket->write(sendData.toUtf8());
    }
    else if(list[0]=="f")  //群聊消息
    {
        //获取系统时间
        QDateTime time=QDateTime::currentDateTime();
        QString Time=time.toString("yyyy-MM-dd ddd hh:mm:ss");
        ui->textBrowser->append(Time);
        ui->textBrowser->append(list[1]+":"+list[2]);
        sendData=sendData+"#"+list[1]+"#"+list[2];
        query=new QSqlQuery;
        int str5;
        query->exec(QString("select CIP, CPort from user where status = '%1'").arg(1));
        while(query->next())
        {
            QString str1=query->value(0).toString();
            str5=query->value(1).toInt();
            socket = new QTcpSocket();
            socket->abort();   //取消已有的连接
            socket->connectToHost(str1,str5); //连接到客户端
            socket->waitForConnected();

            socket->write(sendData.toUtf8());
            qDebug()<<sendData<<str1<<str5;
        }
    }
    else if(list[0]=="g")  //私聊
    {
        QString Status;
        query=new QSqlQuery;
        query->exec(QString("select status from user where name='%1'").arg(list[1]));
        while(query->next())
        {
            Status=query->value(0).toString();
        }
        if(Status=="1")
        {
            query->exec(QString("select CIP, CPort from user where name = '%1'").arg(list[1]));
            while(query->next())
            {
                str1=query->value(0).toString();
                str2=query->value(1).toString();
            }
            sendData=sendData+"#"+list[1]+"#"+"1"+"#"+str1+"#"+str2;
        }
        else
        {
           sendData=sendData+"#"+list[1]+"#"+"0";
        }
        socket->write(sendData.toUtf8());
    }
    else if(list[0]=="s")  //获取信息以发送文件
    {

        query->exec(QString("select CIP, CPort from user where name = '%1'").arg(list[1]));
        while(query->next())
        {
            str1=query->value(0).toString();
            str2=query->value(1).toString();
        }
        sendData=sendData+"#"+list[1]+"#"+"1"+"#"+str1+"#"+str2;   
        socket->write(sendData.toUtf8());    
    }
    else if(list[0]=="i")  //离线消息
    {
        query=new QSqlQuery;
        query->exec(QString("insert into msg (name1, name2, MSG) values('%1','%2','%3')").arg(list[1]).arg(list[2]).arg(list[3]));
    }
    else
        return;
}


void MainWindow::on_pushButton_2_clicked()  //刷新用户列表
{
    //刷新用户列表
    QString str=NULL;
    query=new QSqlQuery;
    query->exec("select name from user ");
    ui->listWidget->clear();
    while(query->next())
    {
        QString str=query->value(0).toString();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(str);      //设置列表项的文本
        ui->listWidget->addItem(item);     //加载列表项到列表框
    }

}
