#include "mysql.h"
#include <QDebug>

MySql::MySql()
{

}

bool MySql::initsql()   //初始化并建立一个数据库连接
{

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("127.0.0.1");
//    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("userinfo");
    if( db.open())
    {
        qDebug() << "数据库连接成功！";
        return true;
    }
    else
    {
        qDebug()<<"数据库连接失败!";
        return false;
    }
}

int MySql::Login(QString name, QString pwd)
{
    QString str1,str2;
    query=new QSqlQuery;
    query->exec(QString("select AES_DECRYPT(pwd,'key'),status from user where name='%1'").arg(name));
    while(query->next())
    {
        str1=query->value(0).toString();  //密码
        str2=query->value(1).toString();  //在线状态
    }
    if(str2==NULL)  return 3;   //用户不存在
    else if(str2=='1')  return 2;  //用户已经在线
    else if((str2=='0')&&(str1==pwd)) return 1; //用户名和密码都正确
    else if((str2=='0')&&(str1!=pwd)) return 0; //密码错误
}


bool MySql::Regist(QString names,QString pwds,QString tels,QString question,QString answer)
{
    QString str=QString("select * from user where name='%1'").arg(names);
    query=new QSqlQuery;
    query->exec(str);
    query->last();
    int record=query->at()+1;
    if(record!=0)
        return false;
    str=QString("insert into user value('%1',AES_ENCRYPT('%2','key'),'%3','%4','%5','%6','%7','%8')").arg(names).arg(pwds).arg(tels).arg(NULL).arg(NULL).arg(question).arg(answer).arg(0);
    bool ret=query->exec(str);
    return ret;
}


int MySql::findpwd(QString name,QString tel,QString question,QString answer) //找回密码
{
    QString str1,str2,str3;
    query=new QSqlQuery;
    query->exec(QString("select tel,question,answer from user where name='%1'").arg(name));
    while(query->next())
    {
        str1=query->value(0).toString();  //电话
        str2=query->value(1).toString();  //密保问题
        str3=query->value(2).toString(); //密保答案
    }
    if((str1==tel)&&(str2==question)&&(str3==answer)) return 3; //信息正确
    else if(str1==NULL)   return 4;  //用户不存在
    else if(tel!=NULL&&str1!=tel)   return 0;   //手机号错误
    else if(str2!=question)  return 1;   //密保问题错误
    else  return 2;  //密保答案错误
}
