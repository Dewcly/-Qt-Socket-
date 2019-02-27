create database userinfo;
use userinfo;

create table user (
	name varchar(30) not null,
	pwd varchar(99) not null,
	tel varchar(30),
    CIP varchar(30),
    CPort varchar(30),
    question varchar(99),
	answer varchar(99),
	status varchar(4),
	
	primary key(name)
);

create table  msg(
	no int not null,
	name1 varchar(20),
	name2 varchar(30),
	MSG varchar(20),
	primary key(no)
);

INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('Allen', 'G¸�?+»«`\"ëç4ØÊ', '12345678900', '10.12.68.195', '6041', '我最喜欢的球队是�?', '湖人', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('Curry', 'G¸�?+»«`\"ëç4ØÊ', '12345678900', '10.11.166.226', '6113', '我最喜欢的球队是�?', '湖人', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('James', 'G¸�?+»«`\"ëç4ØÊ', '12345678900', '192.168.1.7', '6162', '我的真实姓名是？', 'James', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('Jordan', 'G¸�?+»«`\"ëç4ØÊ', '18571555075', '10.14.126.223', '6077', '我的真实姓名是？', '篮球之神', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('Kobe', 'G¸�?+»«`\"ëç4ØÊ', '12345678900', '192.168.1.7', '6133', '我的生日是？', '7�?15�?', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('Mary', 'Mw	™´þÆK¹°w', '13995922881', '0', '0', '我的真实姓名是？', 'Mary', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('Wade', 'G¸�?+»«`\"ëç4ØÊ', '12345678900', '10.14.125.112', '6110', '我的真实姓名是？', 'Wade', '0');
INSERT INTO `user` (`name`, `pwd`, `tel`, `CIP`, `CPort`, `question`, `answer`, `status`) VALUES ('王旭�?', 'G¸�?+»«`\"ëç4ØÊ', '17771309472', '192.168.0.107', '6081', '我最喜欢的球队是�?', '湖人', '0');
