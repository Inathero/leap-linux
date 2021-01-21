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

SOURCES += main.cpp\
        mainwindow.cpp \
    listener.cpp \
    logic.cpp \
    scriptengine.cpp \
    src/AudioProgressBarDialog.cpp \
    src/AudioSinkDialog.cpp \
    src/Settings.cpp \
    timer.cpp

HEADERS  += mainwindow.h \
    listener.h \
    logic.h \  \
    src/AudioProgressBarDialog.h \
    scriptengine.h \
    mouseenums.h \
    leapmouseconfig.h \
    src/AudioSinkDialog.h \
    src/Settings.h \
    timer.h

unix: HEADERS += xkeys.h \
                xmouse.h
unix: SOURCES += xkeys.cpp \
                xmouse.cpp

win32: HEADERS += winkeys.h \
                  winmouse.h
win32: SOURCES += winkeys.cpp \
                  winmouse.cpp

FORMS    += mainwindow.ui \
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
