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
        switch((*gl).type())
        {
            case Gesture::TYPE_CIRCLE:
                {
                    CircleGesture gesture = CircleGesture(*gl);
                    QString direction = gesture.pointable().direction().angleTo(gesture.normal()) <= Leap::PI/2 ? "clockwise" : "counterclockwise";
                    qDebug() << "Gesture Circle, Duration: " << gesture.duration()
                        << ", Direction: " << direction;
                }
                break;

            case Gesture::TYPE_SWIPE:
                {
                    SwipeGesture gesture = SwipeGesture(*gl);
                    qDebug() << "Gesture Swipe, Duration: " << gesture.duration()
                        << ", Direction: " << gesture.direction().toString().c_str()
                        << ", Speed: " << gesture.speed();
                }
                break;

            case Gesture::TYPE_KEY_TAP:
                break;

            case Gesture::TYPE_SCREEN_TAP:
                break;
            case Gesture::TYPE_INVALID:
                break;
        }
    }
}
