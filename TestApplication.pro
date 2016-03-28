#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T14:19:55
#
#-------------------------------------------------

QT       += core gui network sql axcontainer

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
    views/startview.cpp \
    views/studentinfoview.cpp \
    tcpclient.cpp \
    views/testbaseview.cpp \
    views/testtabview.cpp \
    views/clienttabview.cpp \
    views/custombutton.cpp \
    views/maintestview.cpp \
    views/settingsview.cpp \
    views/savingsettingstabview.cpp \
    views/statementtestview.cpp \
    views/questiontestview.cpp

HEADERS  += \
    mainwindow.h \
    testfilereader.h \
    stringencryption.h \
    randomgenerator.h \
    global.h \
    views/assureview.h \
    views/resultview.h \
    views/startview.h \
    views/studentinfoview.h \
    tcpclient.h \
    views/testbaseview.h \
    views/testtabview.h \
    views/clienttabview.h \
    views/custombutton.h \
    views/maintestview.h \
    views/settingsview.h \
    views/savingsettingstabview.h \
    views/statementtestview.h \
    views/questiontestview.h

RESOURCES += \
    resources.qrc
