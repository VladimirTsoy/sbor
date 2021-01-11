#-------------------------------------------------
#
# Project created by QtCreator 2019-09-20T12:00:12
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoSbor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../class/classbd.cpp \
    ../class/dirfile.cpp \
    ../class/egps.cpp \
    ../class/epoch.cpp \
    ../class/converterjps.cpp \
    ../class/eglonass.cpp \
    ../class/formnumlib.cpp \
    ../class/integritycontrol.cpp \
    ../class/jpsreader.cpp \
    ../class/jpswriter.cpp \
    ../class/obrizm.cpp \
    ../class/renexwriter.cpp \
    ../class/rinexfile.cpp \
    ../class/rinexreader.cpp \
    ../class/rintorinconv.cpp \
    ../class/rnxnconv.cpp \
    ../class/timeconv.cpp \
    ../sbor/fpass.cpp \
    ../compressor/compressor.cpp \
    ../compressor/crx2rnx.cpp \
    ../compressor/rnx2crx.cpp

HEADERS += \
        mainwindow.h \
    ../class/classbd.h \
    ../class/dirfile.h \
    ../class/eglonass.h \
    ../class/egps.h \
    ../class/epoch.h \
    ../class/converterjps.h \
    ../class/formnumlib.h \
    ../class/integritycontrol.h \
    ../class/jpsreader.h \
    ../class/jpswriter.h \
    ../class/obrizm.h \
    ../class/renexwriter.h \
    ../class/rinexfile.h \
    ../class/rinexreader.h \
    ../class/rintorinconv.h \
    ../class/rnxnconv.h \
    ../class/timeconv.h \
    ../sbor/fpass.h \
    ../compressor/rnx2crx.h \
    ../compressor/compressor.h \
    ../compressor/crx2rnx.h

FORMS += \
        mainwindow.ui \
    ../sbor/fpass.ui

RESOURCES += \
    res.qrc

INCLUDEPATH += /home/user/prog/class
INCLUDEPATH += /home/user/prog/sbor
INCLUDEPATH += /home/user/prog/compressor
