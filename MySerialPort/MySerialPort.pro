#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T11:06:49
#
#-------------------------------------------------

QT       += core gui

QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MySerialPort
TEMPLATE = app


SOURCES += main.cpp\
        myserialport.cpp \
    mybutton.cpp \
    receivethread.cpp \
    drawwave.cpp \
    qcustomplot.cpp

HEADERS  += myserialport.h \
    mybutton.h \
    receivethread.h \
    drawwave.h \
    qcustomplot.h

FORMS    += myserialport.ui
