【说明】
MySql：
实验中用的是3306号端口，若您MySql数据库端口不是3306，那么请您按照下述网址更改您数据库的端口。
附上网址：https://jingyan.baidu.com/article/bad08e1ee2ffe709c951217c.html

MySql表文件夹：
里面是本次实验用到的数据库与表文件包含脚本文件user.sql、msg.sql及表文件msg.xlsx、user.xlsx，导入方式如下：
1.打开您的MySql命令行终端，输入密码登录；
2.此时终端显示mysql>；
3.输入source （msg.sql文件路径），敲回车；
4.输入source （user.sql文件路径），敲回车；
5.完成

mysql-connector-c-6.1.11-win32.msi：
若您的Mysql为64位，而您的Qt编译器为32位，则需要安装此连接器，将安装的动态链接库dll文件拷贝到您的Mysql安装目录的bin文件夹下面。