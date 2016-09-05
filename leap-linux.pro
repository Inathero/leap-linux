#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T23:48:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets x11extras

TARGET = leap-linux
TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp \
    listener.cpp \
    logic.cpp \
    xkeys.cpp \
    macro.cpp \
    scriptengine.cpp \
    xmouse.cpp

HEADERS  += mainwindow.h \
    listener.h \
    logic.h \
    xkeys.h \
    macro.h \
    scriptengine.h \
    xmouse.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/leap/lib/x64/release/ -lLeap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/leap/lib/x64/debug/ -lLeap
#else:unix: LIBS += -L$$PWD/leap/lib/x64/ -lLeap -L/usr/X11R6/lib -lX11 -lXtst
else:unix: LIBS += -L/usr/lib/Leap -lLeap -L/usr/X11R6/lib -lX11 -lXtst

INCLUDEPATH += /usr/lib/Leap $$PWD/leap/include $$PWD/leap/util
DEPENDPATH += /usr/lib/Leap
