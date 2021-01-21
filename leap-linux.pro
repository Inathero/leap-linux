#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T23:48:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#unix: QT += x11extras

TARGET = leap-linux
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += src \
            src/Utilities

SOURCES += src/main.cpp\
    src/Commands.cpp \
        src/mainwindow.cpp \
    src/Listener.cpp \
    src/Logic.cpp \
    src/AudioProgressBarDialog.cpp \
    src/AudioSinkDialog.cpp \
    src/Settings.cpp

HEADERS  += src/mainwindow.h \
    src/Commands.h \
    src/Listener.h \
    src/Logic.h \  \
    src/AudioProgressBarDialog.h \
    src/AudioSinkDialog.h \
    src/Settings.h

FORMS    += src/mainwindow.ui \
    src/AudioProgressBarDialog.ui \
    src/AudioSinkDialog.ui

unix: LIBS += -L$$PWD/leap/lib/x64 -lLeap -L/usr/X11R6/lib -lX11 -lXtst
unix: INCLUDEPATH += $$PWD/leap/include $$PWD/leap/util
unix: DEPENDPATH += $$PWD/leap/util

win32: LIBS += -L$$PWD/leap/lib/x86/orion/ -lLeap -luser32
win32: INCLUDEPATH += $$PWD/leap/lib/x86/orion $$PWD/leap/include/orion
win32: DEPENDPATH += $$PWD/leap/lib/x86/orion

RESOURCES += \
    resources.qrc

DISTFILES += \
    script/commands.txt
