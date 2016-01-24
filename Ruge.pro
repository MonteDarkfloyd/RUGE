#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T18:31:36
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ruge
TEMPLATE = app
RC_ICONS  = rugeicon.ico

SOURCES += main.cpp\
    trafficwindow.cpp \
    session.cpp \
    newsessiondialog.cpp \
    createsession.cpp \
    sessionloader.cpp \
    sessionsaver.cpp \
    trafficloader.cpp \
    trafficsaver.cpp \
    tabledelegate.cpp \
    variabledata.cpp

HEADERS  += \
    trafficwindow.h \
    session.h \
    newsessiondialog.h \
    createsession.h \
    sessionloader.h \
    sessionsaver.h \
    trafficloader.h \
    trafficsaver.h \
    tabledelegate.h \
    variabledata.h

FORMS    += \
    trafficwindow.ui \
    newsessiondialog.ui \
    createsession.ui

RESOURCES += \
    resources.qrc
