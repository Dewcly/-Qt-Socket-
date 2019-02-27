#ifndef MYSQL_H
#define MYSQL_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class MySql
{
public:
     MySql();
     bool initsql();
     int Login(QString name,QString pwd); //登录
     bool Regist(QString names,QString pwds,QString tels,QString question,QString answer); //注册
     int findpwd(QString name,QString tel,QString question,QString answer); //找回密码
private:
    QSqlQuery *query;

};

#endif // MYSQL_H
