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
            << ", grab strength: " << Hands.frontmost().grabStrength() << std::endl;
    }
    else
        qDebug() << "No Hands";
}
