#include "mainwindow.h"
#include <QApplication>
#include "ui_mainwindow.h"
#include <QDateTime>
#include "sendfile.h"
Mainwindow::Mainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);
    socket=new QTcpSocket();
    server=new QTcpServer();

    CIP=getIP();

    QTime current_time = QTime::currentTime();
    int second = current_time.second(); //系统当前秒数
    qsrand(second);
    Cport = QString::number(qrand()%1024+6000);
    CPort=Cport.toInt();
    server->listen(QHostAddress::Any,CPort);//监听本机的某个端口
    connect(server,SIGNAL(newConnection()),this,SLOT(Connect()));  //接受来自其他客户端的信号并连
    connect(socket,SIGNAL(readyRead()),this,SLOT(Read_Data()));   //接受来自服务器的数据
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
               this,SLOT(displayError(QAbstractSocket::SocketError)));   //发生错误时执行displayError函数


}

Mainwindow::~Mainwindow()
{
    delete ui;
    delete this->socket;
}

void Mainwindow::Connect()   //连接客户端
{
    socket=server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(Read_Data()));   //获取来自服务器的数据
}


QString Mainwindow::getIP()
{
    QString localHostName = QHostInfo::localHostName();//本机用户名
    QString IP;
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
         if(address.protocol() == QAbstractSocket::IPv4Protocol)
         {
//             if (address.toString().contains("192.168.")){
//                 continue;
//             }
             IP = address.toString();//本机ip地址
             return IP;
         }
    }
    return NULL;
}


void Mainwindow::closeEvent(QCloseEvent *event)
{
    QString title=windowTitle();
    socket->abort();   //取消已有的连接
    socket->connectToHost(IP,port); //连接服务器
    socket->waitForConnected();
    QString ds="d";
    QString data=ds+"#"+title;
    socket->write(data.toUtf8());
    close();
}

void Mainwindow::Read_Data()
{
    QString data = socket->readAll();   //读取服务器发送的数据
    QStringList list=data.split("#");
    QString title=windowTitle();
    qDebug()<<data;
    if(list[0]=="e")     //加载用户列表
    {
        ui->listWidget->clear();
        for(int i=1;i<list.length();i++)
        {
          if(list[i]!="end")
          {
              QString str = list[i];
              QListWidgetItem *item = new QListWidgetItem;
              item->setText(str);
              ui->listWidget->addItem(item);
          }
          else ui->textBrowser->append(list[++i]);
        }
    }
    else if(list[0]=="f") //群发消息功能实现
    {
        if(list[1]!=title)
        {
            QDateTime time=QDateTime::currentDateTime();
            QString Time=time.toString("yyyy-MM-dd ddd hh:mm:ss ");
            ui->textBrowser->append(Time);
            ui->textBrowser->append(list[1]+" : "+list[2]);
        }

    }

    else if(list[0]=="g") //私聊
    {

        if(list[2]=="1")
        {
            cport=list[4].toInt();
            socket->abort();   //取消已有的连接
            YIP=list[3];
            socket->connectToHost(YIP,cport); //连接到客户端B
            socket->waitForConnected();//等待连接
            p=1;
            QDateTime time=QDateTime::currentDateTime();
            QString Time=time.toString("yyyy-MM-dd ddd hh:mm:ss ");
            ui->textBrowser->append(Time);
            ui->textBrowser->append("已连接到"+list[1]+":"+list[3]+" "+list[4]);
        }
        else if(list[2]=="0")
        {
            p=2;
        }

    }
    else if(list[0]=="h") //点对点消息功能实现
    {
        QDateTime time=QDateTime::currentDateTime();
        QString Time=time.toString("yyyy-MM-dd ddd hh:mm:ss ");
        ui->textBrowser->append(Time);
        ui->textBrowser->append(list[1]+" : "+list[2]);
    }
    else if(list[0]=="s") //发送文件获取ip和端口
    {

          YIP=list[3];
          cport=(list[4]).toInt();
          socket->abort();   //取消已有的连接
          socket->connectToHost(YIP,cport); //连接到客户端B
          socket->waitForConnected();
          QString ms="m";
          QString data=ms+"#"+title;
          socket->write(data.toUtf8());
    }
    else if(list[0]=="m") //发文件
    {
        Sendfile *m1  = new  Sendfile();
        m1->show();
        m1->setWindowTitle(title);
    }
     else  return;

}


void Mainwindow::on_pushButton_2_clicked() //获取用户列表，并将自己的ip和端口号发给服务器
{
    QString title=windowTitle();
    socket->abort();   //取消已有的连接
    //连接服务器
    socket->connectToHost(IP, port);
    //等待连接成功
    if(!socket->waitForConnected(30000))
    {
        qDebug() << "Connection failed!";
        return;
    }
    else
    qDebug() << "Connect successfully!";
    QString es="e";
    QString data=es+"#"+title+"#"+CIP+"#"+Cport;
    socket->write(data.toUtf8());
}

void Mainwindow::on_pushButton_clicked()   //点击“发送”
{
    QString title=windowTitle();
    QString fs;
    QString msg=ui->textEdit->toPlainText();
    if(p==0)  //群聊
    {
        socket->abort();   //取消已有的连接
        //连接服务器
        socket->connectToHost(IP, port);
         //等待连接成功
        if(!socket->waitForConnected(30000))
        {
           qDebug() << "Connection failed!";
           return;
        }
        else
        qDebug() << "Connect successfully!";
        fs="f";//群聊
        QString data=fs+"#"+title+"#"+msg;
        socket->write(data.toUtf8());
    }
    else if(p==1)  //点对点私聊
    {
        fs="h";//私聊
        QString data=fs+"#"+title+"#"+msg;
        socket->write(data.toUtf8());
        qDebug()<<data;
    }
    else if(p==2)  //离线消息
    {
        socket->abort();   //取消已有的连接
        //连接服务器
        socket->connectToHost(IP, port);
         //等待连接成功
        if(!socket->waitForConnected(30000))
        {
           qDebug() << "Connection failed!";
           return;
        }
        else
        qDebug() << "Connect successfully!";
        fs="i";//离线消息
        QString Name=ui->listWidget->currentItem()->text();
        QString data=fs+"#"+title+"#"+Name+"#"+msg;
        socket->write(data.toUtf8());
    }
    QDateTime time=QDateTime::currentDateTime();
    QString Time=time.toString("yyyy-MM-dd ddd hh:mm:ss ");
    ui->textBrowser->append(Time);
    ui->textBrowser->append(title+" : "+msg);
    ui->textEdit->clear();

}

void Mainwindow::on_pushButton_3_clicked()  //点击发送文件
{
      QString title=windowTitle();
      socket->abort();
      socket->connectToHost(IP, port); //连接到服务器
      socket->waitForConnected();
      QString name=ui->listWidget->currentItem()->text();
      QString ss="s#";
      QString data=ss+name;
      socket->write(data.toUtf8());
      Sendfile *m1  = new  Sendfile();
      m1->show();
      m1->setWindowTitle(title);

}



void Mainwindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    socket->abort();
    //连接服务器
    socket->connectToHost(IP, port);
    socket->waitForConnected();
    QString Name=item->text();
    QString gs="g";
    QString data=gs+"#"+Name;
    socket->write(data.toUtf8());
}
