#ifndef WINMOUSE_H
#define WINMOUSE_H

#include <QObject>
#include <qt_windows.h>
#include <QPoint>

enum xmouse_button_type_enum
{
    xm_left = 1,
    xm_middle,
    xm_right
};

class winmouse : public QObject
{
    Q_OBJECT
public:
    explicit winmouse(QObject *parent = 0);

signals:

public slots:
    void mouse_button_click(xmouse_button_type_enum xmouse_enum);
    void mouse_move(float Leap_Finger_x, float Leap_Finger_y);
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
};

#endif // WINMOUSE_H
