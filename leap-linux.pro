#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T23:48:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix: QT += x11extras

TARGET = leap-linux
TEMPLATE = app

CONFIG += c++11 release

SOURCES += main.cpp\
        mainwindow.cpp \
    listener.cpp \
    logic.cpp \
    macro.cpp \
    scriptengine.cpp

HEADERS  += mainwindow.h \
    listener.h \
    logic.h \ 
    macro.h \
    scriptengine.h

unix: HEADERS += xkeys.h \
                xmouse.h

unix: SOURCES += xkeys.cpp \
                xmouse.cpp

win32: HEADERS += winkeys.h \
                  winmouse.h
win32: SOURCES += winkeys.cpp \
                  winmouse.cpp

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/leap/lib/x64 -lLeap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/leap/lib/x64 -lLeap
#else:unix: LIBS += -L$$PWD/leap/lib/x64/ -lLeap -L/usr/X11R6/lib -lX11 -lXtst
else:unix: LIBS += -L/usr/lib/Leap -lLeap -L/usr/X11R6/lib -lX11 -lXtst


unix: INCLUDEPATH += /usr/lib/Leap $$PWD/leap/include $$PWD/leap/util
unix: DEPENDPATH += /usr/lib/Leap

win32: INCLUDEPATH += $$PWD/leap/include/orion
win32: DEPENDPATH += $$PWD/leap/include/orion
