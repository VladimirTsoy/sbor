include(gtest_dependency.pri)

QT       += core gui sql network printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle        # Помещает исполняемый файл в пакет (по умолчанию).
CONFIG += thread

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

HEADERS += \
        tst_ident_kadrs.h \
        ../../../Statful/statfullfile.h \
    ../../class/classbd.h \
    ../../class/converter.h \
    ../../class/converterjps.h \
    ../../class/dirfile.h \
    ../../class/eglonass.h \
    ../../class/egps.h \
    ../../class/obrizm.h \
    ../../class/jpswriter.h \
    ../../class/jpsreader.h \
    ../../class/integritycontrol.h \
    ../../class/formnumlib.h \
    ../../class/epoch.h \
    ../../class/renexwriter.h \
    ../../class/rinexfile.h \
    ../../class/rinexreader.h \
    ../../class/rintorinconv.h \
    ../../class/rnxnconv.h \
    ../../class/timeconv.h \
    ../../compressor/compressor.h \
    ../../compressor/crx2rnx.h \
    ../../compressor/rnx2crx.h

SOURCES += \
        main.cpp \
        ../../../Statful/statfullfile.cpp \
    ../../class/classbd.cpp \
    ../../class/converter.cpp \
    ../../class/converterjps.cpp \
    ../../class/dirfile.cpp \
    ../../class/eglonass.cpp \
    ../../class/egps.cpp \
    ../../class/obrizm.cpp \
    ../../class/jpswriter.cpp \
    ../../class/jpsreader.cpp \
    ../../class/integritycontrol.cpp \
    ../../class/formnumlib.cpp \
    ../../class/epoch.cpp \
    ../../class/renexwriter.cpp \
    ../../class/rinexfile.cpp \
    ../../class/rinexreader.cpp \
    ../../class/rintorinconv.cpp \
    ../../class/rnxnconv.cpp \
    ../../class/timeconv.cpp \
    ../../compressor/compressor.cpp \
    ../../compressor/crx2rnx.cpp \
    ../../compressor/rnx2crx.cpp

INCLUDEPATH += ../../../Statful \
               ../../../class \
               ../../compressor
