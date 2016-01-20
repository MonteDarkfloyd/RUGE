#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T18:31:36
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ruge
TEMPLATE = app


SOURCES += main.cpp\
    trafficwindow.cpp \
    session.cpp \
    newsessiondialog.cpp \
    createsession.cpp \
    sessionloader.cpp \
    udpvalues.cpp \
    tcpvalues.cpp \
    sessionsaver.cpp \
    trafficloader.cpp \
    trafficsaver.cpp \
    extra.cpp \
    tabledelegate.cpp

HEADERS  += \
    trafficwindow.h \
    session.h \
    newsessiondialog.h \
    createsession.h \
    sessionloader.h \
    udpvalues.h \
    tcpvalues.h \
    sessionsaver.h \
    trafficloader.h \
    trafficsaver.h \
    extra.h \
    tabledelegate.h

FORMS    += \
    trafficwindow.ui \
    newsessiondialog.ui \
    createsession.ui
