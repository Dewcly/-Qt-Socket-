#ifndef SENDFILE_H
#define SENDFILE_H

#include <QDialog>
#include <QtNetwork>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include "mainwindow.h"

namespace Ui {
class Sendfile;
}

class Sendfile : public QDialog
{
    Q_OBJECT


public:
    explicit Sendfile(QWidget *parent = 0);
    ~Sendfile();
    void send_init();
    void openFile();
    void startTransfer();
    void send();

    void receive_init();
    void updateServerProgress();

    int flag=0;
//    QString YouIP=Mainwindow::YIP;
    QString YouIP="192.168.1.7";

    QByteArray line;
    QFile *send_localFile;
    QFile *receive_localFile;
    qint64 send_totalBytes;
    qint64 receive_totalBytes;
    qint64 send_bytesWritten;
    qint64 send_bytesToWrite;
    qint64 receive_bytesReceived;
    qint64 receive_fileNameSize;
    qint64 send_loadSize;
    QString send_fileName;
    QString receive_fileName;
    QByteArray send_outBlock;
    QByteArray receive_inBlock;
    QUdpSocket *send_udpSocket;
    QUdpSocket *receive_udpSocket;
    QHostAddress send_ip;
    QHostAddress receive_ip;
    QTimer *m_pTimer;


private slots:
    void on_open_pushButton_clicked();
    void on_send_pushButton_clicked();
    void handletimeout();
    void send_readmessage();
    void processPendingDatagram();

private:
    Ui::Sendfile *ui;
};

#endif // SENDFILE_H




