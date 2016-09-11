#ifndef XMOUSE_H
#define XMOUSE_H


//#include <QApplication>
//#include <QDesktopWidget>
//#include <QRect>
#include <QPoint>
#include <QObject>
#include <QDebug>
#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#undef Bool
#ifdef None
#undef None
#endif
#ifdef FrameFeature
#undef FrameFeature
#endif
#ifdef Status
#undef Status
#endif

enum xmouse_button_type_enum
{
    xm_left = 1,
    xm_middle,
    xm_right
};

class xmouse : public QObject
{
    Q_OBJECT
public:
    explicit xmouse(QObject *parent = 0);

signals:

public slots:
    void mouse_button_click(xmouse_button_type_enum xmouse_enum);
    void mouse_move(float Leap_Finger_x, float Leap_Finger_y);
    QPoint mouse_map_leap_to_screen(float Finger_x, float Finger_y);

    void mouse_set_leap_ranges(int * iLeapMouseRect);
private slots:
    XEvent mouse_setup();
private:
    Display * xDisplay ;
    Window xWinRoot;
    Window xWinFocus;
    int * LeapMouseRect;
    int * DisplayResolution;
};

#endif // XMOUSE_H
