#-------------------------------------------------
#
# Project created by QtCreator 2019-03-03T13:30:28
#
#-------------------------------------------------

QT       += core gui network widgets
RC_ICONS = images/icon.ico

# Translations
TRANSLATIONS += ts/DrCOM_zh_CN.ts

TARGET = DrCOM_JLU_Qt
TEMPLATE = app
CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    encrypt/EncryptData.cpp \
    main.cpp \
    mainwindow.cpp \
    dogcomcontroller.cpp \
    interruptiblesleeper.cpp \
    dogcom.cpp \
    encrypt/md4.cpp \
    encrypt/md5.cpp \
    encrypt/sha1.cpp \
    DogcomSocket.cpp

HEADERS += \
    encrypt/EncryptData.h \
    mainwindow.h \
    dogcomcontroller.h \
    constants.h \
    interruptiblesleeper.h \
    dogcom.h \
    encrypt/md4.h \
    encrypt/md5.h \
    encrypt/sha1.h \
    DogcomSocket.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    DrCOM_JLU_Qt.qrc

# Single Application implementation
include(singleinstance/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

VERSION = 1.0.0.7

win32:LIBS += -lwsock32
win32:LIBS += -lcrypt32

win32-msvc {
    QMAKE_CXXFLAGS += /FIiterator
    DEFINES += _SILENCE_STDEXT_CHECKED_ARRAY_ITERATOR_DEPRECATION_WARNING
}

macx {
    QMAKE_LIBS_OPENGL -= -framework AGL
    LIBS -= -framework AGL
}
