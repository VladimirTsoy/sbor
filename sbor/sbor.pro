#-------------------------------------------------
#
# Project created by QtCreator 2019-09-20T11:07:16
#
#-------------------------------------------------

QT       += core gui sql network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sbor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    fdds.cpp \
    fds.cpp \
    threadobr12.cpp \
    feditfull.cpp \
    feditnos.cpp \
    feditsba.cpp \
    feditserv.cpp \
    fedvid.cpp \
    ffull.cpp \
    fkolseansds.cpp \
    fkos.cpp \
    fnosd.cpp \
    fpass.cpp \
    fpolz.cpp \
    fpolzedit.cpp \
    fpotr.cpp \
    frnxi.cpp \
    frnxv.cpp \
    frpom.cpp \
    fsba.cpp \
    fseansdds.cpp \
    fseansds.cpp \
    fseansppsi.cpp \
    fseansvrv.cpp \
    fserv.cpp \
    fsist.cpp \
    fstatdds.cpp \
    fstatfull.cpp \
    fstatserv.cpp \
    fstatvrv.cpp \
    ftxtfileview.cpp \
    fust.cpp \
    fustzapr.cpp \
    fvrv.cpp \
    fzapr.cpp \
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
    ../class/converter.cpp \
    ../compressor/compressor.cpp \
    ../compressor/crx2rnx.cpp \
    ../compressor/rnx2crx.cpp \
    fppsi.cpp \
    fustnf.cpp \
    fcogi.cpp \
    fnfview.cpp \
    fcup.cpp \
    fautozapr.cpp \
    fsp3.cpp \
    fddsview.cpp \
    fpotdds.cpp \
    fsol.cpp \
    ../ppsi/ppsi.cpp \
    ../Statful/statfullfile.cpp \

HEADERS += \
        mainwindow.h \
    fdds.h \
    fds.h \
    threadobr12.h \
    feditfull.h \
    feditnos.h \
    feditsba.h \
    feditserv.h \
    fedvid.h \
    ffull.h \
    fkolseansds.h \
    fkos.h \
    fnosd.h \
    fpass.h \
    fpolz.h \
    fpolzedit.h \
    fpotr.h \
    frnxi.h \
    frnxv.h \
    frpom.h \
    fsba.h \
    fseansdds.h \
    fseansds.h \
    fseansppsi.h \
    fseansvrv.h \
    fserv.h \
    fsist.h \
    fstatdds.h \
    fstatfull.h \
    fstatserv.h \
    fstatvrv.h \
    ftxtfileview.h \
    fust.h \
    fustzapr.h \
    fvrv.h \
    fzapr.h \
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
    ../class/converter.h \
    ../compressor/rnx2crx.h \
    ../compressor/compressor.h \
    ../compressor/crx2rnx.h \
    fppsi.h \
    fustnf.h \
    fcogi.h \
    fnfview.h \
    fcup.h \
    fautozapr.h \
    fsp3.h \
    fddsview.h \
    fpotdds.h \
    fsol.h \
    ../ppsi/ppsi.h \
    ../Statful/statfullfile.h \

FORMS += \
        mainwindow.ui \
    fdds.ui \
    fds.ui \
    feditfull.ui \
    feditnos.ui \
    feditsba.ui \
    feditserv.ui \
    fedvid.ui \
    ffull.ui \
    fkolseansds.ui \
    fkos.ui \
    fnosd.ui \
    fpass.ui \
    fpolz.ui \
    fpolzedit.ui \
    fpotr.ui \
    frnxi.ui \
    frnxv.ui \
    frpom.ui \
    fsba.ui \
    fseansdds.ui \
    fseansds.ui \
    fseansppsi.ui \
    fseansvrv.ui \
    fserv.ui \
    fsist.ui \
    fstatdds.ui \
    fstatfull.ui \
    fstatserv.ui \
    fstatvrv.ui \
    ftxtfileview.ui \
    fust.ui \
    fustzapr.ui \
    fvrv.ui \
    fzapr.ui \
    fppsi.ui \
    fustnf.ui \
    fcogi.ui \
    fnfview.ui \
    fcup.ui \
    fautozapr.ui \
    fsp3.ui \
    fddsview.ui \
    fpotdds.ui \
    fsol.ui \
    ../ppsi/ppsi.ui \
    ../ppsi/zona.ui

INCLUDEPATH += /home/user/prog/class
INCLUDEPATH += /home/user/prog/Sbor
INCLUDEPATH += /home/user/prog/compressor
INCLUDEPATH += /home/user/prog/Statful
INCLUDEPATH += /user/include/qwt
INCLUDEPATH +=/usr/local/qwtpolar-1.1.1/include

LIBS += -lqwtpolar
LIBS += -L/usr/lib/ -lqwt-qt5
QTPLUGIN +=/usr/lib/x86_64-linux-gnu/qt5/plugins/libQtcDbViewer.so
CONFIG += QtcDbViewer
#LIBS += /usr/lib/x86_64-linux-gnu/qt5/plugins/libQtcDbViewer.so
CONFIG += qwt warn_on

RESOURCES += \
    res.qrc

CONFIG(debug, debug|release){
    DESTDIR = ../../bin/debug
    OBJECTS_DIR = ../../build/debug/obj
    MOC_DIR = ../../build/debug/moc
    UI_DIR = ../../build/debug/ui
    RCC_DIR = ../../build/debug/rcc
} else {
    DESTDIR = ../../bin/release
    OBJECTS_DIR = ../../build/release/obj
    MOC_DIR = ../../build/release/moc
    UI_DIR = ../../build/release/ui
    RCC_DIR = ../../build/release/rcc
}

!exists( main.cpp ) {                       # Если не существует файл main.cpp, то сказать об этом.
    error( "No main.cpp file found" )
}
