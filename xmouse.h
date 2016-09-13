#ifndef XMOUSE_H
#define XMOUSE_H


//#include <QApplication>
//#include <QDesktopWidget>
//#include <QRect>
#include <QPoint>
#include <QObject>
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "mouseenums.h"

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

class xmouse : public QObject
{
    Q_OBJECT
public:
    explicit xmouse(QObject *parent = 0);

     mouse_type_enum mouse_type;
signals:

public slots:
    void mouse_button_click(mouse_button_type_enum xmouse_enum);
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
