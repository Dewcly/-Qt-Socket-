#ifndef REGIST_H
#define REGIST_H

#include <QDialog>
#include "tclient.h"

namespace Ui {
class Regist;
}

class Regist : public QDialog
{
    Q_OBJECT

public:
    explicit Regist(QWidget *parent = 0);
    ~Regist();
    QString IP=TClient::IP;
    int port=TClient::port;

private:
    Ui::Regist *ui;
    QTcpSocket *socket;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void readMSG();
};

#endif // REGIST_H
