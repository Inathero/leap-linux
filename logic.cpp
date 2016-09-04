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

        iFingersExtended  = 0;
        foreach(Finger finger, hand.fingers())
        {
            if (finger.isExtended())
                iFingersExtended ++;
        }

//        qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();
//        qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str();

        // alms_giver
        if (hand.palmVelocity().magnitude() < 30 && hand.palmNormal().y > 0.6)
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
                        qDebug() << "swiupe: " << gesture.direction().x <<", " << Macro->isMacroAvailable();
                        if(Macro->isMacroAvailable())
                        {
                            ScriptEngine->preScript(iFingersExtended);
                        if(gesture.direction().x > 0.50)
                            iModeLock = ScriptEngine->runScript("swipe_right", FINGER_MOD);
                        if (gesture.direction().x < -0.50)
                            iModeLock = ScriptEngine->runScript("swipe_left", FINGER_MOD);
                        if(gesture.direction().y > 0.50)
                            iModeLock = ScriptEngine->runScript("swipe_up", FINGER_MOD);
                        if (gesture.direction().y < -0.50)
                            iModeLock = ScriptEngine->runScript("swipe_down", FINGER_MOD);
                        qDebug() << "Lock Duration: "<< iModeLock;
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
