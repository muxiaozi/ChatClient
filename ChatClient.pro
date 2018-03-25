#-------------------------------------------------
#
# Project created by QtCreator 2017-10-30T18:38:39
#
#-------------------------------------------------

QT       += core gui network sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp\
        chatclient.cpp \
    login.cpp \
    netdelegate.cpp

HEADERS  += chatclient.h \
    login.h \
    netdelegate.h

FORMS    += chatclient.ui \
    logindialog.ui

RESOURCES += \
    resource.qrc
