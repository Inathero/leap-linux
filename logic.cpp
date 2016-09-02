#include "logic.h"

logic::logic(QObject *parent) : QObject(parent)
{

}

void logic::Leap_Hands(Leap::HandList Hands)
{
    if(!Hands.isEmpty())
    {
        std::cout  << ", hands: " << Hands.count()
            << ", fingers: " << Hands.frontmost().fingers().count()
            << ", tools: " << Hands.frontmost().tools().count()
            << ", grab strength: " << Hands.frontmost().grabStrength()
            << ", pinch strength: " << Hands.frontmost().pinchStrength() << std::endl;
    }
    else
        qDebug() << "No Hands";
}

void logic::Leap_Gestures(GestureList Gestures)
{
    for(Leap::GestureList::const_iterator gl = Gestures.begin(); gl != Gestures.end(); gl++)
    {
        Gesture gesture = *gl;
        switch(gesture.type())
        {
            case Gesture::TYPE_CIRCLE:
                {
                    CircleGesture cGesture = CircleGesture(gesture);
                    QString direction = cGesture.pointable().direction().angleTo(cGesture.normal()) <= Leap::PI/2 ? "clockwise" : "counterclockwise";
                    qDebug() << "Gesture Circle, Duration: " << cGesture.duration()
                        << ", Direction: " << direction;
                }
                break;

            case Gesture::TYPE_SWIPE:
                {
                    SwipeGesture sGesture = SwipeGesture(gesture);
                    qDebug() << "Gesture Swipe, Duration: " << sGesture.duration()
                        << ", Direction: " << sGesture.direction().toString().c_str()
                        << ", Speed: " << sGesture.speed();
                }
                break;

            case Gesture::TYPE_KEY_TAP:
                break;

            case Gesture::TYPE_SCREEN_TAP:
                break;
        }
    }
}
