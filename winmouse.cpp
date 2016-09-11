#include "winmouse.h"

winmouse::winmouse(QObject *parent) : QObject(parent)
{

}

void winmouse::mouse_button_click(xmouse_button_type_enum xmouse_enum)
{
    INPUT Input;
    Input.type = INPUT_MOUSE;

    switch (xmouse_enum)
    {
        case xm_left:
            Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1,&Input, sizeof(INPUT));
            Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1,&Input, sizeof(INPUT));
        break;
        case xm_middle:
            Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            SendInput(1,&Input, sizeof(INPUT));
            Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            SendInput(1,&Input, sizeof(INPUT));
        break;
        case xm_right:
            Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1,&Input, sizeof(INPUT));
            Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1,&Input, sizeof(INPUT));
        break;
    }
}

void winmouse::mouse_move(float Leap_Finger_x, float Leap_Finger_y)
{
    qDebug() << "winmouse::mouse_move" << Leap_Finger_x << ":" << Leap_Finger_y;
    QPoint xy = mouse_map_leap_to_screen(Leap_Finger_x, Leap_Finger_y);

    SetCursorPos(xy.x(), xy.y());
    // TODO
}

QPoint winmouse::mouse_map_leap_to_screen(float Finger_x, float Finger_y)
{
   float tempX = Finger_x - LeapMouseRect[0];
    // Subtraction is vertical translation (no sub = touching leap controller)
    float tempY = Finger_y - LeapMouseRect[2];
//qDebug() << "A:" << Finger_y;
//qDebug() << "B:" << LeapMouseRect[2];
//qDebug() << "C:" << tempY;
    //Translations from leap to screen coordinates
    tempX = tempX > LeapMouseRect[1] ? LeapMouseRect[1] : tempX < 0 ? 0 : tempX;
    tempX = *DisplayResolution * (tempX / LeapMouseRect[1]);

//    qDebug() << "C1:" << *(LeapMouseRect+3);
    tempY = tempY > LeapMouseRect[3] ? LeapMouseRect[3] : tempY < 0 ? 0 : tempY;
//    qDebug() << "D:" << tempY;
    tempY = *(DisplayResolution+1) - (*(DisplayResolution+1) * (tempY / LeapMouseRect[3]));
//    qDebug() << "E:" << tempY;

    return QPoint(tempX,tempY);
}

void winmouse::mouse_set_leap_ranges(int *iLeapMouseRect)
{
    LeapMouseRect = iLeapMouseRect;

    // Do shifts for easy mapping
    *(LeapMouseRect+1) -= *LeapMouseRect;
    *(LeapMouseRect+3) -= *(LeapMouseRect+2);
//    qDebug() << "C1:" << *(LeapMouseRect+2);

    // Get resolution of default screen
    // TODO: Convert to windows code
    QRect s = QApplication::desktop()->screenGeometry();
    DisplayResolution = new int[1];

    *(DisplayResolution) = s.width();
    *(DisplayResolution+1) = s.height();

}
