#-------------------------------------------------
#
# Project created by QtCreator 2017-10-21T16:31:19
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCient
TEMPLATE = app

SOURCES += main.cpp\
        tclient.cpp \
    regist.cpp \
    mainwindow.cpp \
    findpsd.cpp \
    sendfile.cpp

HEADERS  += tclient.h \
    regist.h \
    mainwindow.h \
    findpsd.h \
    sendfile.h

FORMS    += tclient.ui \
    regist.ui \
    mainwindow.ui \
    findpsd.ui \
    sendfile.ui

RESOURCES += \
    images.qrc
