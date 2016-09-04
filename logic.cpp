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

          qDebug() << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();

        iFingersExtended  = 0;
        foreach(Finger finger, hand.fingers())
        {
            if (finger.type() != 0)
            {
            if (finger.isExtended())
                iFingersExtended ++;
            }
        }

//        qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();
//        qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str();

        // alms_giver
        if (hand.palmVelocity().magnitude() < 30 && hand.palmNormal().y > 0.6)
        {
            if(Macro->isMacroAvailable())
            {
                int iModeLock = ScriptEngine->runScript("alms_giver");
                qDebug() << "Alms Giver Lock Duration: "<< iModeLock;
                Macro->macroLock(iModeLock);
            }
        }
        // hand_key
        if (hand.sphereRadius() < 55. && iFingersExtended == 0)
        {
            if(Macro->isMacroAvailable())
            {
                if (hand.palmNormal().y < -0.85)
                    bHandKeyRot = true;
                if(hand.palmNormal().y > 0 && bHandKeyRot)
                {
                    int iModeLock = ScriptEngine->runScript("hand_key");
                    qDebug() << "Hand Key Lock Duration: "<< iModeLock;
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

void logic::Leap_Gestures(GestureList Gestures, Hand hand)
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
                        int iModeLock = 0;

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
                        qDebug() << "Circle Lock Duration: "<< iModeLock;
                        Macro->macroLock(iModeLock);
                    }
                }
                break;

            case Gesture::TYPE_SWIPE:
                {
                        SwipeGesture gesture = SwipeGesture(*gl);
                        int iModeLock = 0;
//                        qDebug() << "swiupe: " << gesture.direction().x <<", " << Macro->isMacroAvailable();
                        if(Macro->isMacroAvailable())
                        {
                            if (iFingersExtended > 1)
                            {
                                if ( fabs(hand.palmNormal().x) < 0.5)
                                    return;
                                else
                                {
                                    ScriptEngine->preScript(iFingersExtended);
                                    if(hand.palmNormal().x > 0.30)
                                        iModeLock = ScriptEngine->runScript("swipe_right", FINGER_MOD);
                                    if (hand.palmNormal().x < -0.50)
                                        iModeLock = ScriptEngine->runScript("swipe_left", FINGER_MOD);
                                    if(gesture.direction().y > 0.50)
                                        iModeLock = ScriptEngine->runScript("swipe_up", FINGER_MOD);
                                    if (gesture.direction().y < -0.50)
                                        iModeLock = ScriptEngine->runScript("swipe_down", FINGER_MOD);
                                }
                            }
                            else
                            {
                                if(gesture.direction().x > 0.50)
                                    iModeLock = ScriptEngine->runScript("swipe_right");
                                if (gesture.direction().x < -0.50)
                                    iModeLock = ScriptEngine->runScript("swipe_left");
                                if(gesture.direction().y > 0.50)
                                    iModeLock = ScriptEngine->runScript("swipe_up");
                                if (gesture.direction().y < -0.50)
                                    iModeLock = ScriptEngine->runScript("swipe_down");
                            }
                        qDebug() << "Swipe Lock Duration: "<< iModeLock;
                        Macro->macroLock(iModeLock);
                    }
                }
                break;

            case Gesture::TYPE_KEY_TAP:
                {
                    KeyTapGesture gesture = KeyTapGesture(*gl);

                    if(Macro->isMacroAvailable())
                    {
                        ScriptEngine->preScript(gesture.pointable().id() % 10);
                        int iModeLock = ScriptEngine->runScript("finger_tap", FINGER_MOD);
                        qDebug() << "Tap Lock Duration: "<< iModeLock;
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
