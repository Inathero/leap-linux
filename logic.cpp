#include "logic.h"


logic::logic(QObject *parent) : QObject(parent)
{

    Macro = new macro(this);
    ScriptEngine = new scriptengine;
    ScriptEngine->setDefinitions(QApplication::applicationDirPath().append("/scripts/defines.ina"));
    ScriptEngine->setScriptFile(QApplication::applicationDirPath().append("/scripts/macros.ina"));
}

inline void logic::logic_hand_debug(Hand hand)
{
//     qDebug() << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();
    // qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();
    // qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str();
}

void logic::Leap_Hands(Leap::HandList Hands)
{
    if(!Hands.isEmpty())
    {
        for (auto hand : Hands)
        {
//        Hand hand = Hands.frontmost();

            ScriptEngine->preScript("HandMod", hand.isRight());

        logic_hand_debug(hand);

        Leap_FingerSetup(hand.fingers());

        // alms_giver
        if (hand.palmVelocity().magnitude() < 30 && hand.palmNormal().y > 0.6 && iFingersExtended > 3)
        {
            if(Macro->isMacroAvailable())
            {
                int iModeLock = ScriptEngine->runScript("alms_giver");
                Macro->macroLock(iModeLock);
            }
        }
        // hand_key
        if (iFingersExtended == 0 && !bThumbExtended)
        {
            if(Macro->isMacroAvailable())
            {
                if (hand.palmNormal().y < -0.85)
                    bHandKeyRot = true;
                if(hand.palmNormal().x < -0.60 && bHandKeyRot)
                {
                    int iModeLock = ScriptEngine->runScript("hand_key");
                    Macro->macroLock(iModeLock);
                }
            }
        }
        // thumbs_up | down
        if (hand.sphereRadius() < 65. && iFingersExtended == 0 && bThumbExtended)
        {
            if(Macro->isMacroAvailable())
            {
                if (hand.palmNormal().y < -0.85)
                    bThumbKeyRot = true;

                int iModeLock = 0;
                if(hand.palmNormal().x > 0.60 && bThumbKeyRot)
                {
                    if (hand.isRight())
                        iModeLock = ScriptEngine->runScript("thumb_down");
                    else
                        iModeLock = ScriptEngine->runScript("thumb_up");
                    Macro->macroLock(iModeLock);
                }
                if(hand.palmNormal().x < -0.60 && bThumbKeyRot)
                {
                    if (hand.isRight())
                        iModeLock = ScriptEngine->runScript("thumb_up");
                    else
                        iModeLock = ScriptEngine->runScript("thumb_down");
                    Macro->macroLock(iModeLock);
                }
            }
        }

        // shaka_up | down -- Thumb and pinky extended
        if (iFingersExtended == 1 && bFingersExtended[3] && bThumbExtended)
        {
            if(Macro->isMacroAvailable())
            {
                int iModeLock = 0;
                if(hand.palmNormal().y < -0.9)
                {
                    iModeLock = ScriptEngine->runScript("shaka_down");
                    Macro->macroLock(iModeLock);
                }
                if(hand.palmNormal().y > 0.9)
                {
                    iModeLock = ScriptEngine->runScript("shaka_up");
                    Macro->macroLock(iModeLock);
                }
            }
        }
        }

    }
    else
    {
        bHandKeyRot = false;
        bThumbKeyRot= false;
    }
}

void logic::Leap_FingerSetup(FingerList Fingers)
{
    iFingersExtended  = 0;

    foreach(Finger finger, Fingers)
    {
        // Get thumb status
        if (finger.type() == 0)
            bThumbExtended = finger.isExtended();

        // Get other fingers status
        if (finger.type() != 0)
        {
            // Correlate index to extended state, for gestures
            bFingersExtended[finger.type()-1] = finger.isExtended();

            // Obtain total number of extended fingers
            if (finger.isExtended())
                iFingersExtended ++;

            // This code excerpt here works 100% fine
            // It tracks index finger (when extended) and maps cursor to it
            //
            // Currently not in use, so it is ignored
            //if (finger.type() == 1 && finger.isExtended())
            //{
                //ScriptEngine->debug(finger.stabilizedTipPosition().x, finger.stabilizedTipPosition().y);
            //}
        }
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
                    if(Macro->isMacroAvailable() && iFingersExtended == 1)
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
                            if ( fabs(hand.palmNormal().x) < 0.5)
                                return;
                            else
                            {
                                ScriptEngine->preScript("FingerMod", iFingersExtended);
                                if(hand.palmNormal().x > 0.50)
                                    iModeLock = ScriptEngine->runScript("swipe_right");
                                if (hand.palmNormal().x < -0.80)
                                    iModeLock = ScriptEngine->runScript("swipe_left");
//                                if(gesture.direction().y > 0.50)
 //                                    iModeLock = ScriptEngine->runScript("swipe_up");
//                                if (gesture.direction().y < -0.50)
//                                    iModeLock = ScriptEngine->runScript("swipe_down");
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
                        ScriptEngine->preScript("FingerMod", gesture.pointable().id() % 10);
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
