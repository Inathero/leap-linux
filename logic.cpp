#include "logic.h"


logic::logic(QObject *parent) : QObject(parent)
{
    scriptEngine = new scriptengine;
    _bDebugLeftFist = false;
    _bPinch = false;
    scriptEngine->setScriptPath(QApplication::applicationDirPath().append("/scripts"));
    _iGenericCounter = 0;
    _timer = new timer;

    AudioDialog = new AudioProgressBarDialog;
    for (int i = 0; i < 7; i++)
    {
        macro_block_struct a;
        qlMacroBlocks.append(a);
    }

}

inline void logic::logicHandDebug(Hand hand)
{
//    qDebug() <<  hand.pinchStrength();
//         qDebug() << hand.palmNormal().toString().c_str() << " : " << hand.pinchStrength() << iFingersExtended;
    // qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius(); // qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str(
}

// z < -0.5
// pinch = 1
// fingersextended = 3
void logic::leapHands(Leap::HandList Hands)
{

//    if(!Hands.isEmpty() && !_bStillProcessing)
    if(!Hands.isEmpty())
    {
//        _iGenericCounter ++;
        for (auto hand : Hands)
        {
            //        Hand hand = Hands.frontmost();
            _bStillProcessing = true;
            if(Hands.count() == LEAP_HAND_BOTH)
                _iHandActive = hand.isLeft() ? LEAP_HAND_BOTH_LEFT : LEAP_HAND_BOTH_RIGHT;
            else
                _iHandActive = hand.isLeft() ? LEAP_HAND_LEFT : LEAP_HAND_RIGHT;

            //            qDebug() << "HAND:" << iHandActive;
//            scriptEngine->preScript("HandMod", _iHandActive);

            logicHandDebug(hand);

//            leapFingerSetup(hand.fingers());

//            if (hand.isLeft() && _iFingersExtended == 0 && !_bDebugLeftFist)
//            {
//                _bDebugLeftFist = true;
//                //            ScriptEngine->debugMouseDown();
//            }
//            else if (hand.isLeft() && _iFingersExtended == 4 && _bDebugLeftFist)
//            {
//                _bDebugLeftFist = false;
//                //            ScriptEngine->debugMouseUp();
//            }

//            // alms_giver
//            else if (hand.palmVelocity().magnitude() < 30 && hand.palmNormal().y > 0.6 && _iFingersExtended > 3)
//            {
//                if(qlMacroBlocks[0].bEnabled)
//                {
//                    int iModeLock = scriptEngine->runScript("alms_giver");
//                    _timer->AddToQueue(qlMacroBlocks[0], iModeLock);
//                    return;
//                }
//            }
//            // hand_key
//            else if (_iFingersExtended == 0 && !_bThumbExtended)
//            {
//                if(qlMacroBlocks[1].bEnabled)
//                {
//                    if (hand.palmNormal().y < -0.85 && !_bHandKeyRot)
//                        _bHandKeyRot = true;
//                    else if (_bHandKeyRot)
//                    {
//                        int iModeLock = -1;
//                        if (hand.isRight())
//                        {
//                            if(hand.palmNormal().x < -0.60)
//                                iModeLock = scriptEngine->runScript("hand_key_up");
//                            else if(hand.palmNormal().x > 0.60)
//                                iModeLock = scriptEngine->runScript("hand_key_down");
//                        }
//                        else
//                        {
//                            if(hand.palmNormal().x < -0.60)
//                                iModeLock = scriptEngine->runScript("hand_key_down");
//                            else if(hand.palmNormal().x > 0.60)
//                                iModeLock = scriptEngine->runScript("hand_key_up");
//                        }
//                        if (iModeLock != -1)
//                            _timer->AddToQueue(qlMacroBlocks[1], iModeLock);
//                    }
//                }
//            }
//            // thumbs_up | down
//            else if (_iFingersExtended == 0 && _bThumbExtended)
//            {
//                if(qlMacroBlocks[2].bEnabled)
//                {
//                    if (hand.palmNormal().y < -0.70)
//                        _bThumbKeyRot = true;

//                    int iModeLock = 0;
//                    if(hand.palmNormal().x > 0.60 && _bThumbKeyRot)
//                    {
//                        if (hand.isRight())
//                            iModeLock = scriptEngine->runScript("thumb_down");
//                        else
//                            iModeLock = scriptEngine->runScript("thumb_up");
//                        _timer->AddToQueue(qlMacroBlocks[2], iModeLock);
//                    }
//                    if(hand.palmNormal().x < -0.60 && _bThumbKeyRot)
//                    {
//                        if (hand.isRight())
//                            iModeLock = scriptEngine->runScript("thumb_up");
//                        else
//                            iModeLock = scriptEngine->runScript("thumb_down");
//                        _timer->AddToQueue(qlMacroBlocks[2], iModeLock);
//                    }
//                }
//            }

//            // shaka_up | down -- Thumb and pinky extended
//            else if (_iFingersExtended == 1 && _bFingersExtended[3] && _bThumbExtended)
//            {
//                if(qlMacroBlocks[3].bEnabled)
//                {
//                    int iModeLock = 0;
//                    if(hand.palmNormal().y < -0.9)
//                    {
//                        iModeLock = scriptEngine->runScript("shaka_down");
//                        _timer->AddToQueue(qlMacroBlocks[3], iModeLock);
//                        return;
//                    }
//                    if(hand.palmNormal().y > 0.9)
//                    {
//                        iModeLock = scriptEngine->runScript("shaka_up");
//                        _timer->AddToQueue(qlMacroBlocks[3], iModeLock);
//                        return;
//                    }
//                }
//            }


            // pinch
            if (hand.pinchStrength() > 0.8)
            {
                Leap::Vector lvStabPalmPos = hand.stabilizedPalmPosition();
                if(!_bPinch)
                {
                    _bPinch = true;
                    _lvPinchPalmReference = lvStabPalmPos;
                }
                else
                {
                    if(_iGenericCounter % _iTempPinchFrequency == 0)
                    {
                        int iSpeedMultiplier = (lvStabPalmPos.y - _lvPinchPalmReference.y);
                        mouse_button_type_enum iWheelMod;
                        if (iSpeedMultiplier < 0)
                            iWheelMod = xm_wheel_down;
                        else
                            iWheelMod = xm_wheel_up;

                        qDebug() << "pinch - " << iSpeedMultiplier;
                        if(abs(iSpeedMultiplier) != 0)
                        {
                            AudioDialog->addRelativeAudioLevel(iSpeedMultiplier);
                            _lvPinchPalmReference = lvStabPalmPos;
                        }
//                        for(int i = 0; i < (abs(iSpeedMultiplier) > 0); i++)
//                            xmouse::mouse_button_click(iWheelMod);
                    }
                }
            }

            if (hand.pinchStrength() < 0.2)
                _bPinch = false;
        }

        _bStillProcessing = false;
    }
    else
    {
        _bStillProcessing = false;
        _iHandActive = -1;
        _bHandKeyRot = false;
        _bThumbKeyRot= false;
        _iGenericCounter = 0;
    }
}

void logic::leapFingerSetup(FingerList Fingers)
{
    _iFingersExtended  = 0;

    foreach(Finger finger, Fingers)
    {
        if(scriptEngine->LeapMouseConfig.Track_Mods.bEnable &&
                scriptEngine->LeapMouseConfig.iTrack_Type == LEAP_MOUSE_TRACK_FINGER &&
                scriptEngine->LeapMouseConfig.Track_Mods.bExtended == finger.isExtended() &&
                scriptEngine->LeapMouseConfig.Track_Mods.iFinger == finger.type())
        {
            if(scriptEngine->LeapMouseConfig.Track_Mods.iHand == finger.hand().isLeft())
                scriptEngine->debug(finger.stabilizedTipPosition().x, finger.stabilizedTipPosition().y);

            else if(scriptEngine->LeapMouseConfig.Track_Mods.iHand - 1 ==  finger.hand().isRight())
                scriptEngine->debug(finger.stabilizedTipPosition().x, finger.stabilizedTipPosition().y);
        }
        // Get thumb status
        if (finger.type() == 0)
            _bThumbExtended = finger.isExtended();

        // Get other fingers status
        if (finger.type() != 0)
        {
            // Correlate index to extended state, for gestures
            _bFingersExtended[finger.type()-1] = finger.isExtended();
            //            qDebug() << "fin - "  << bFingersExtended[0] << bFingersExtended[1] << bFingersExtended[2] << bFingersExtended[3] << bFingersExtended[4];

            // Obtain total number of extended fingers
            if (finger.isExtended())
                _iFingersExtended ++;

            // This code excerpt here works 100% fine
            // It tracks index finger (when extended) and maps cursor to it
            //
            // Currently not in use, so it is ignored
        }
    }
}

void logic::leapGestures(GestureList Gestures, Hand hand)
{
    return;
    for(Leap::GestureList::const_iterator gl = Gestures.begin(); gl != Gestures.end(); gl++)
    {

        switch((*gl).type())
        {
        case Gesture::TYPE_CIRCLE:
        {
            if(qlMacroBlocks[4].bEnabled && _iFingersExtended == 1)
            {
                CircleGesture gesture = CircleGesture(*gl);
                bool bDirection = gesture.pointable().direction().angleTo(gesture.normal()) <= Leap::PI/2;// ? "clockwise" : "counterclockwise";
                int iModeLock = 0;

                //                        qDebug()<<gesture.progress();
                if(bDirection) // clockwise
                {
                    if (gesture.progress() > 1.)
                        iModeLock = scriptEngine->runScript("circle_clockwise");
                }
                else // counterclockwise
                {
                    if (gesture.progress() > 1.)
                        iModeLock = scriptEngine->runScript("circle_counterclockwise");
                }
                _timer->AddToQueue(qlMacroBlocks[4], iModeLock);
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
            if(qlMacroBlocks[5].bEnabled)
            {
                if ( fabs(hand.palmNormal().x) < 0.5)
                    return;
                else
                {
                    scriptEngine->preScript("FingerMod", _iFingersExtended);
                    if(hand.palmNormal().x > 0.50)
                        iModeLock = scriptEngine->runScript("swipe_right");
                    if (hand.palmNormal().x < -0.80)
                        iModeLock = scriptEngine->runScript("swipe_left");
                    //                                if(gesture.direction().y > 0.50)
                    //                                    iModeLock = ScriptEngine->runScript("swipe_up");
                    //                                if (gesture.direction().y < -0.50)
                    //                                    iModeLock = ScriptEngine->runScript("swipe_down");
                }
                _timer->AddToQueue(qlMacroBlocks[5], iModeLock);
            }
        }
        break;

        case Gesture::TYPE_KEY_TAP:
        {
            KeyTapGesture gesture = KeyTapGesture(*gl);
            if(_iHandActive > LEAP_HAND_BOTH)
            {
                _iHandActive = gesture.hands().frontmost().isLeft() ? LEAP_HAND_BOTH_LEFT : LEAP_HAND_BOTH_RIGHT;
                scriptEngine->preScript("HandMod", _iHandActive);
            }

//            if(Macro->isMacroAvailable())
            if(qlMacroBlocks[6].bEnabled)
            {
                qDebug() << gesture.pointable().id() ;
                scriptEngine->preScript("FingerMod", gesture.pointable().id() % 10);
                int iModeLock = scriptEngine->runScript("finger_tap");
                _timer->AddToQueue(qlMacroBlocks[6], iModeLock);
//                Macro->macroLock(iModeLock);
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
