#ifndef FINDPSD_H
#define FINDPSD_H

#include <QDialog>
//#include <TClient.h>
#include "tclient.h"

namespace Ui {
class FindPsd;
}

class FindPsd : public QDialog
{
    Q_OBJECT

public:
    explicit FindPsd(QWidget *parent = 0);
    ~FindPsd();
    QString IP=TClient::IP;
    int port=TClient::port;

private slots:
    void on_pushButton_clicked();
    void readMsg(); //接受来自服务器的信息

private:
    Ui::FindPsd *ui;
    QTcpSocket *socket;
};

#endif // FINDPSD_H
