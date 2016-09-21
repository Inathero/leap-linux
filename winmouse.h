#ifndef WINMOUSE_H
#define WINMOUSE_H

#include <QObject>
#include <qt_windows.h>
#include <QPoint>
#include <QRect>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

#include "mouseenums.h"

class winmouse : public QObject
{
    Q_OBJECT
public:
    explicit winmouse(QObject *parent = 0);

    mouse_type_enum mouse_type;
signals:

public slots:
    static void mouse_button_click(mouse_button_type_enum xmouse_enum);
    static void mouse_move(float Leap_Finger_x, float Leap_Finger_y);
    static void mouse_button_down(mouse_button_type_enum xmouse_enum);
    static void mouse_button_up(mouse_button_type_enum xmouse_enum);
    QPoint mouse_map_leap_to_screen(float Finger_x, float Finger_y);

    void mouse_set_leap_ranges(int * iLeapMouseRect);
private:
#if __unix
    Display * xDisplay ;
    Window xWinRoot;
    Window xWinFocus;
#elif _WIN32
#endif
    int * LeapMouseRect;
    int * DisplayResolution;
    QPoint prevPoint;
};

#endif // WINMOUSE_H

