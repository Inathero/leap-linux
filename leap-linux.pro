#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T23:48:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = leap-linux
TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/leap/lib/x64/release/ -lLeap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/leap/lib/x64/debug/ -lLeap
else:unix: LIBS += -L$$PWD/leap/lib/x64/ -lLeap

INCLUDEPATH += $$PWD/leap/lib/x64 $$PWD/leap/include $$PWD/leap/util
DEPENDPATH += $$PWD/leap/lib/x64
