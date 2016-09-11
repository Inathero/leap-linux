#include "xmouse.h"

xmouse::xmouse(QObject *parent) : QObject(parent)
{
    xDisplay = XOpenDisplay(0);
    if(xDisplay == NULL)
        qWarning() << "xmouse: Unable to open xDisplay";

    // Get the root window for the current xDisplay.
    xWinRoot = XDefaultRootWindow(xDisplay);
}

void xmouse::mouse_button_click(xmouse_button_type_enum xmouse_enum)
{
    Display *dpy = NULL;
    dpy = XOpenDisplay (NULL);

    qDebug() << "xmouse::mouse_button_click:" << xmouse_enum;

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent (dpy, xmouse_enum, True,  CurrentTime);
    XTestFakeButtonEvent (dpy, xmouse_enum, False, CurrentTime);

    XCloseDisplay (dpy);
}

void xmouse::mouse_move(float Leap_Finger_x, float Leap_Finger_y)
{
    Display *dpy = NULL;
    dpy = XOpenDisplay (NULL);
    QPoint xy = mouse_map_leap_to_screen(Leap_Finger_x, Leap_Finger_y);

    /* Fake the mouse move */
    XTestFakeMotionEvent (dpy, -1, xy.x(), xy.y(), CurrentTime);

    XCloseDisplay (dpy);
}

QPoint xmouse::mouse_map_leap_to_screen(float Finger_x, float Finger_y)
{
    float tempX = Finger_x - LeapMouseRect[0];
    // Subtraction is vertical translation (no sub = touching leap controller)
    float tempY = Finger_y - LeapMouseRect[2];

    //Translations from leap to screen coordinates
    tempX = tempX > LeapMouseRect[1] ? LeapMouseRect[1] : tempX < 0 ? 0 : tempX;
    tempX = *DisplayResolution * (tempX / LeapMouseRect[1]);

    tempY = tempY > LeapMouseRect[3] ? LeapMouseRect[3] : tempY < 0 ? 0 : tempY;
    tempY = *(DisplayResolution+1) - (*(DisplayResolution+1) * (tempY / LeapMouseRect[3]));

    return QPoint(tempX,tempY);
}

void xmouse::mouse_set_leap_ranges(int *iLeapMouseRect)
{
    LeapMouseRect = iLeapMouseRect;

    // Do shifts for easy mapping
    *(LeapMouseRect+1) -= *LeapMouseRect;
//    *(LeapMouseRect+3) -= *(LeapMouseRect+4);
    *(LeapMouseRect+3) -= *(LeapMouseRect+2);

    // Get resolution of default screen
    Screen*  s = DefaultScreenOfDisplay(xDisplay);
    DisplayResolution = new int[1];

    *(DisplayResolution) = s->width;
    *(DisplayResolution+1) = s->height;

}

XEvent xmouse::mouse_setup()
{
     xDisplay = NULL;
     XEvent xEvent;

     /* Get the current pointer position */
     XQueryPointer (xDisplay, xWinRoot, &xEvent.xbutton.root,
      &xEvent.xbutton.window, &xEvent.xbutton.x_root,
      &xEvent.xbutton.y_root, &xEvent.xbutton.x, &xEvent.xbutton.y,
      &xEvent.xbutton.state);

     return xEvent;
}

