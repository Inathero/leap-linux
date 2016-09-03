#include "logic.h"

logic::logic(QObject *parent) : QObject(parent)
{

    Macro = new macro(this);
    ScriptEngine = new scriptengine;
    ScriptEngine->setDefinitions(QApplication::applicationDirPath().append("/scripts/defines.ina"));
    ScriptEngine->setScriptFile(QApplication::applicationDirPath().append("/scripts/macros.ina"));
}

void logic::Leap_Hands(Leap::HandList Hands)
{
    if(!Hands.isEmpty())
    {
        Hand hand = Hands.frontmost();
//        qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str();

        // alms_giver
        if (hand.palmVelocity().magnitude() < 30 && hand.palmNormal().y > 0.1)
        {
            if(Macro->isMacroAvailable())
            {
                int iModeLock = ScriptEngine->runScript("alms_giver");
                Macro->macroLock(iModeLock);
            }
        }
    }
}

void logic::Leap_Gestures(GestureList Gestures)
{
    for(Leap::GestureList::const_iterator gl = Gestures.begin(); gl != Gestures.end(); gl++)
    {
        switch((*gl).type())
        {
            case Gesture::TYPE_CIRCLE:
                {
                    if(Macro->isMacroAvailable())
                    {
                        CircleGesture gesture = CircleGesture(*gl);
                        bool bDirection = gesture.pointable().direction().angleTo(gesture.normal()) <= Leap::PI/2;// ? "clockwise" : "counterclockwise";
                        int iModeLock;

                        if(bDirection) // clockwise
                        {
                            iModeLock = ScriptEngine->runScript("circle_clockwise");
                        }
                        else // counterclockwise
                        {
                            iModeLock = ScriptEngine->runScript("circle_counterclockwise");
                        }
                        Macro->macroLock(iModeLock);
                    }
                }
                break;

            case Gesture::TYPE_SWIPE:
                {
                    SwipeGesture gesture = SwipeGesture(*gl);
                    if(Macro->isMacroAvailable())
                    {
                        int iModeLock;
                        switch(gesture.direction().x > 0)
                        {

                        case 1: // Right
                            iModeLock = ScriptEngine->runScript("swipe_right");
                            break;
                        case 0: //  Left
                            iModeLock = ScriptEngine->runScript("swipe_left");
                            break;
                         }
                        Macro->macroLock(iModeLock);
                    }
                }
                break;

            case Gesture::TYPE_KEY_TAP:
                {
                    KeyTapGesture gesture = KeyTapGesture(*gl);

                    if(Macro->isMacroAvailable())
                    {
                        int iModeLock = ScriptEngine->runScript("finger_tap");
                        Macro->macroLock(iModeLock);

                    }
                }
                break;

            case Gesture::TYPE_SCREEN_TAP:
                break;
            case Gesture::TYPE_INVALID:
                break;
        }
    }
}
