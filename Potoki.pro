#-------------------------------------------------
#
# Project created by QtCreator 2019-04-12T17:05:32
#
#-------------------------------------------------

QT       += core gui

TARGET = OOP_TrueProcess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    mythread.h

FORMS    += mainwindow.ui


QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
