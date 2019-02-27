#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T00:16:33
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        mysql.cpp

HEADERS  += mainwindow.h \
    mysql.h

FORMS    += mainwindow.ui
