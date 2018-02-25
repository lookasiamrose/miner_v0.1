#-------------------------------------------------
#
# Project created by QtCreator 2018-01-14T17:40:53
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPUMinerLibsTests
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    miner.cpp \
    connectionmanager.cpp \
    stratumsocket.cpp \
    globals.cpp

HEADERS  += mainwindow.h \
    miner.h \
    connectionmanager.h \
    stratumsocket.h \
    globals.h

FORMS    += mainwindow.ui

LIBS += -L"C:/curl/compiled/curl-7.58.0/AMD64" -llibcurl
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64" -lws2_32
