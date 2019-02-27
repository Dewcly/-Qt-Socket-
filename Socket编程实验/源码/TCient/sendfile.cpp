#include "sendfile.h"
#include "ui_sendfile.h"
#include "mainwindow.h"
#define TIMEOUT (500)
//int flag=0;
//QString YouIP="172.20.10.4";
Sendfile::Sendfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sendfile)
{
    ui->setupUi(this);
    send_init();
    receive_init();
}


Sendfile::~Sendfile()
{
    delete ui;
}

//文件传输--发送文件handletimeout()

void Sendfile::send_init()
{
    send_loadSize =50*1024;
    send_totalBytes = 0;
    send_bytesWritten = 0;
    send_bytesToWrite = 0;
    QString ip = YouIP;
    send_ip.setAddress(ip);
    send_udpSocket = new QUdpSocket(this);
    send_udpSocket->bind(4444,QUdpSocket::ShareAddress);

    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(handletimeout()));
    connect(send_udpSocket,SIGNAL(readyRead()),this,SLOT(send_readmessage()));
    ui->send_pushButton->setEnabled(false);
}


void Sendfile::openFile()
{
    send_fileName = QFileDialog::getOpenFileName(this);
    if(!send_fileName.isEmpty())
    {
        ui->send_pushButton->setEnabled(true);
    }
}

void Sendfile::startTransfer()
{
    send_localFile = new QFile(send_fileName);
    if(!send_localFile->open(QFile::ReadOnly))
    {
        qDebug()<<"open file error!";
        return;
    }
    send_totalBytes = send_localFile->size();
    qDebug()<<send_totalBytes;
    QDataStream sendOut(&send_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_6);
    QString currentFileName = send_fileName.right(send_fileName.size()-send_fileName.lastIndexOf('/')-1);
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    send_totalBytes  += send_outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<send_totalBytes<<qint64((send_outBlock.size()-sizeof(qint64)*2));
    m_pTimer->start(TIMEOUT);
    send_bytesToWrite = send_totalBytes - send_udpSocket->writeDatagram(send_outBlock,send_outBlock.size(),send_ip,7777);
    qDebug()<<send_bytesToWrite;
    send_bytesWritten = send_outBlock.size();
    qDebug()<<send_bytesWritten;
    ui->sendProgressBar->setMaximum(send_totalBytes);
    ui->sendProgressBar->setValue(send_bytesWritten);
    send_outBlock.resize(0);
}

void Sendfile::send()
{
    if(!send_localFile->atEnd())
    {
        line = send_localFile->read(qMin(send_bytesToWrite,send_loadSize));
        send_bytesToWrite -= send_udpSocket->writeDatagram(line,line.size(),send_ip,7777);
        send_bytesWritten = send_bytesWritten + line.size();
        ui->sendProgressBar->setMaximum(send_totalBytes);
        ui->sendProgressBar->setValue(send_bytesWritten);
        qDebug()<<send_bytesWritten;
    }
    else
    {
        send_udpSocket->writeDatagram("over",send_ip,7777);
        m_pTimer->stop();
        ui->sendProgressBar->close();
        send_udpSocket->close();
        send_localFile->close();
    }
    return;
}

void Sendfile::send_readmessage()
{
    while(send_udpSocket->hasPendingDatagrams())
    {
        QByteArray res = "false";
        res.resize(send_udpSocket->pendingDatagramSize());
        send_udpSocket->readDatagram(res.data(),res.size());
        qDebug()<<res;
        if(res == "true")
        {
            flag = 1;
        }

    }
}

void Sendfile::handletimeout()
{
    qDebug()<<flag;
    if(flag == 1)
    {
        send();
        flag = 0;
    }
    else
    {
        send_udpSocket->writeDatagram(line,line.size(),send_ip,7777);
    }
}



//文件传输--接收文件
void Sendfile::receive_init()
{
    receive_totalBytes =0;
    receive_bytesReceived = 0;
    receive_fileNameSize = 0;
    QString ip = YouIP;
    receive_ip.setAddress(ip);
    receive_udpSocket = new QUdpSocket();
    receive_udpSocket->bind(7777,QUdpSocket::ShareAddress);
    connect(receive_udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
}

void Sendfile::processPendingDatagram()
{
    while(receive_udpSocket->hasPendingDatagrams())
    {
        receive_udpSocket->writeDatagram("true",receive_ip,4444);
        receive_inBlock.resize(receive_udpSocket->pendingDatagramSize());
        qDebug()<<receive_inBlock.size();
        receive_udpSocket->readDatagram(receive_inBlock.data(),receive_inBlock.size());
        if(receive_inBlock == "over")
        {
            qDebug()<<receive_inBlock;
            receive_udpSocket->close();
            receive_localFile->close();
            ui->receiveProgressBar->close();
        }
        else
        {
            updateServerProgress();
        }
    }
}

void Sendfile::updateServerProgress()
{
    QDataStream in(&receive_inBlock,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_6);
    if(receive_bytesReceived <=sizeof(qint64)*2)
    {
        if(((unsigned int)receive_inBlock.size() >=sizeof(qint64)*2)
                &&(receive_fileNameSize == 0))
        {
            in>>receive_totalBytes>>receive_fileNameSize;
            qDebug()<<receive_totalBytes<<receive_fileNameSize;
            ui->receiveProgressBar->setMaximum(receive_totalBytes);
            ui->receiveProgressBar->setValue(receive_bytesReceived);
            qDebug()<<receive_bytesReceived;
        }
        if(((unsigned int)receive_inBlock.size() >= receive_fileNameSize)
                &&(receive_fileNameSize !=0))
        {
            in>>receive_fileName;
            qDebug()<<receive_fileName;
            qDebug()<<receive_bytesReceived;
            receive_localFile = new QFile(receive_fileName);
            if(!receive_localFile->open(QFile::WriteOnly))
            {
                qDebug()<<"open file error!";
                return;
            }
        }
        else return;

    }
    if(receive_bytesReceived < receive_totalBytes)
    {
        receive_bytesReceived +=receive_inBlock.size();
        receive_localFile->write(receive_inBlock);
        qDebug()<<receive_localFile->size();
        receive_inBlock.resize(0);
    }
    qDebug()<<receive_bytesReceived;
    ui->receiveProgressBar->setMaximum(receive_totalBytes);
    ui->receiveProgressBar->setValue(receive_bytesReceived);
    }

void Sendfile::on_send_pushButton_clicked()
{
    startTransfer();
}

    void Sendfile::on_open_pushButton_clicked()
    {
        openFile();
    }

