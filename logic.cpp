#include "logic.h"


logic::logic(QObject *parent) : QObject(parent)
{

    Macro = new macro(this);
    ScriptEngine = new scriptengine;
    bDebugLeftFist = false;
    ScriptEngine->setDefinitions(QApplication::applicationDirPath().append("/scripts/defines.ina"));
    ScriptEngine->setScriptFile(QApplication::applicationDirPath().append("/scripts/macros.ina"));
}

inline void logic::logic_hand_debug(Hand hand)
{
    //     qDebug() << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius();
    // qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius(); // qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str(
}

void logic::Leap_Hands(Leap::HandList Hands)
{

    if(!Hands.isEmpty())
    {
        for (auto hand : Hands)
        {
            //        Hand hand = Hands.frontmost();

            if(Hands.count() == LEAP_HAND_BOTH)
                iHandActive = hand.isLeft() ? LEAP_HAND_BOTH_LEFT : LEAP_HAND_BOTH_RIGHT;
            else
                iHandActive = hand.isLeft() ? LEAP_HAND_LEFT : LEAP_HAND_RIGHT;

            //            qDebug() << "HAND:" << iHandActive;
            ScriptEngine->preScript("HandMod", iHandActive);

            logic_hand_debug(hand);

            Leap_FingerSetup(hand.fingers());

            if (hand.isLeft() && iFingersExtended == 0 && !bDebugLeftFist)
            {
                bDebugLeftFist = true;
                //            ScriptEngine->debugMouseDown();
            }
            if (hand.isLeft() && iFingersExtended == 4 && bDebugLeftFist)
            {
                bDebugLeftFist = false;
                //            ScriptEngine->debugMouseUp();
            }

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
                    if (hand.palmNormal().y < -0.85 && !bHandKeyRot)
                        bHandKeyRot = true;
                    else if (bHandKeyRot)
                    {
                        int iModeLock = -1;
                        if (hand.isRight())
                        {
                            if(hand.palmNormal().x < -0.60)
                                iModeLock = ScriptEngine->runScript("hand_key_up");
                            else if(hand.palmNormal().x > 0.60)
                                iModeLock = ScriptEngine->runScript("hand_key_down");
                        }
                        else
                        {
                            if(hand.palmNormal().x < -0.60)
                                iModeLock = ScriptEngine->runScript("hand_key_down");
                            else if(hand.palmNormal().x > 0.60)
                                iModeLock = ScriptEngine->runScript("hand_key_up");
                        }
                        if (iModeLock != -1)
                            Macro->macroLock(iModeLock);
                    }
                }
            }
            // thumbs_up | down
            if (iFingersExtended == 0 && bThumbExtended)
            {
                if(Macro->isMacroAvailable())
                {
                    if (hand.palmNormal().y < -0.70)
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
        iHandActive = -1;
        bHandKeyRot = false;
        bThumbKeyRot= false;
    }
}

void logic::Leap_FingerSetup(FingerList Fingers)
{
    iFingersExtended  = 0;

    foreach(Finger finger, Fingers)
    {
        if(ScriptEngine->LeapMouseConfig.Track_Mods.bEnable &&
                        ScriptEngine->LeapMouseConfig.iTrack_Type == LEAP_MOUSE_TRACK_FINGER &&
                        ScriptEngine->LeapMouseConfig.Track_Mods.bExtended == finger.isExtended() &&
                        ScriptEngine->LeapMouseConfig.Track_Mods.iFinger == finger.type()) {
            if(ScriptEngine->LeapMouseConfig.Track_Mods.iHand == finger.hand().isLeft())
                ScriptEngine->debug(finger.stabilizedTipPosition().x, finger.stabilizedTipPosition().y);

            else if(ScriptEngine->LeapMouseConfig.Track_Mods.iHand - 1 ==  finger.hand().isRight())
                ScriptEngine->debug(finger.stabilizedTipPosition().x, finger.stabilizedTipPosition().y);
        }
        // Get thumb status
        if (finger.type() == 0)
            bThumbExtended = finger.isExtended();

        // Get other fingers status
        if (finger.type() != 0)
        {
            // Correlate index to extended state, for gestures
            bFingersExtended[finger.type()-1] = finger.isExtended();
            //            qDebug() << "fin - "  << bFingersExtended[0] << bFingersExtended[1] << bFingersExtended[2] << bFingersExtended[3] << bFingersExtended[4];

            // Obtain total number of extended fingers
            if (finger.isExtended())
                iFingersExtended ++;

            // This code excerpt here works 100% fine
            // It tracks index finger (when extended) and maps cursor to it
            //
            // Currently not in use, so it is ignored
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
            //            if(iHandActive > LEAP_HAND_BOTH)
            //            {
            //             iHandActive = hand.isLeft() ? LEAP_HAND_BOTH_LEFT : LEAP_HAND_BOTH_RIGHT;
            //             qDebug() << "JANMD"<<iHandActive;
            //                ScriptEngine->preScript("HandMod", iHandActive);
            //            }
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
            if(iHandActive > LEAP_HAND_BOTH)
            {
                iHandActive = gesture.hands().frontmost().isLeft() ? LEAP_HAND_BOTH_LEFT : LEAP_HAND_BOTH_RIGHT;
                ScriptEngine->preScript("HandMod", iHandActive);
            }

            if(Macro->isMacroAvailable())
            {
                qDebug() << gesture.pointable().id() ;
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
