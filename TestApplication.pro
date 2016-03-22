#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T14:19:55
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestApplication
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    testfilereader.cpp \
    stringencryption.cpp \
    randomgenerator.cpp \
    views/assureview.cpp \
    views/resultview.cpp \
    views/testview.cpp \
    views/startview.cpp \
    views/studentinfoview.cpp \
    tcpclient.cpp \
    views/testbaseview.cpp \
    views/testtabview.cpp \
    views/clienttabview.cpp \
    views/custombutton.cpp \
    views/settingstabview.cpp \
    views/maintesttabview.cpp

HEADERS  += \
    mainwindow.h \
    testfilereader.h \
    stringencryption.h \
    randomgenerator.h \
    global.h \
    views/assureview.h \
    views/resultview.h \
    views/testview.h \
    views/startview.h \
    views/studentinfoview.h \
    tcpclient.h \
    views/testbaseview.h \
    views/testtabview.h \
    views/clienttabview.h \
    views/custombutton.h \
    views/settingstabview.h \
    views/maintesttabview.h

RESOURCES += \
    resources.qrc
