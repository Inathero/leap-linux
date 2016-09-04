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
//        qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();
//        qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str();

        // alms_giver
        if (hand.palmVelocity().magnitude() < 30 && hand.palmNormal().y > 0.9)
        {
            if(Macro->isMacroAvailable())
            {
                int iModeLock = ScriptEngine->runScript("alms_giver");
                Macro->macroLock(iModeLock);
            }
        }
        // hand_key
        if (hand.grabStrength() == 1)
        {
            if(Macro->isMacroAvailable())
            {
                if (hand.palmNormal().y < -0.85)
                    bHandKeyRot = true;
                if(hand.palmNormal().y > 0.2 && bHandKeyRot)
                {
                    int iModeLock = ScriptEngine->runScript("hand_key");
                    Macro->macroLock(iModeLock);
                }
            }
        }

    }
    else
    {
        bHandKeyRot = false;
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

//                        qDebug()<<gesture.progress();
                        if(bDirection) // clockwise
                        {
                            if (gesture.progress() > 1.)
                            iModeLock = ScriptEngine->runScript("circle_clockwise");
                        }
                        else // counterclockwise
                        {
                            if (gesture.progress() > 1.)
                            iModeLock = ScriptEngine->runScript("circle_counterclockwise");
                        }
                        Macro->macroLock(iModeLock);
                    }
                }
                break;

            case Gesture::TYPE_SWIPE:
                {
                        SwipeGesture gesture = SwipeGesture(*gl);
                        int iModeLock;
                        qDebug() << gesture.direction().x;
                        if(Macro->isMacroAvailable())
                        {
                        if(gesture.direction().x > 0.85)
                            iModeLock = ScriptEngine->runScript("swipe_right");
                        if (gesture.direction().x < -0.85)
                            iModeLock = ScriptEngine->runScript("swipe_left");
                        if(gesture.direction().y > 0.85)
                            iModeLock = ScriptEngine->runScript("swipe_up");
                        if (gesture.direction().y < -0.85)
                            iModeLock = ScriptEngine->runScript("swipe_down");

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
