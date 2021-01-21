#include "Logic.h"


Logic::Logic(QObject *parent) : QObject(parent)
{
    _bDebugLeftFist = false;
    _bPinch = false;
    _iGenericCounter = 0;

    _AudioDialog = new AudioProgressBarDialog;

}

inline void Logic::logicHandDebug(Hand hand)
{
//    qDebug() <<  hand.pinchStrength();
//         qDebug() << hand.palmNormal().toString().c_str() << " : " << hand.pinchStrength() << iFingersExtended;
    // qDebug() << hand.grabStrength() << " : " << hand.palmNormal().toString().c_str() << " : " << hand.sphereRadius(); // qDebug() << hand.palmVelocity().magnitude() <<  ", " << hand.palmNormal().toString().c_str(
}

// z < -0.5
// pinch = 1
// fingersextended = 3
void Logic::processLeapHands(Leap::HandList Hands)
{

//    if(!Hands.isEmpty() && !_bStillProcessing)
    if(!Hands.isEmpty())
    {
//        _iGenericCounter ++;
        for (auto hand : Hands)
        {
            //        Hand hand = Hands.frontmost();
            _bStillProcessing = true;

            //            qDebug() << "HAND:" << iHandActive;
//            scriptEngine->preScript("HandMod", _iHandActive);

//            logicHandDebug(hand);

            processLeapFingers(hand.fingers());

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
            if (hand.pinchStrength() > 0.5)
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


                        qDebug() << "pinch - " << lvStabPalmPos.y;
                        if(abs(iSpeedMultiplier) != 0)
                        {
                            _AudioDialog->setRelativeAudioLevel(iSpeedMultiplier);
                            _lvPinchPalmReference = lvStabPalmPos;
                        }
//                        for(int i = 0; i < (abs(iSpeedMultiplier) > 0); i++)
//                            xmouse::mouse_button_click(iWheelMod);
                    }
                }
            }
            // Fist
            if(_iFingersExtended == 0 && !_bFistToggle && hand.stabilizedPalmPosition().y > 220)
            {
                _bFistToggle = true;
                _AudioDialog->toggleMute();
            }
            else if (_bFistToggle && _iFingersExtended != 0)
                _bFistToggle = false;

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
        _bFistToggle = false;
        _iGenericCounter = 0;
    }
}

void Logic::processLeapFingers(FingerList Fingers)
{
    _iFingersExtended  = 0;

    foreach(Finger finger, Fingers)
    {
        // Get thumb status
        if (finger.type() == 0)
            _bThumbExtended = finger.isExtended();

        // Get other fingers status
        if (finger.type() != 0)
        {
            // Correlate index to extended state, for gestures
            _bFingersExtended[finger.type()-1] = finger.isExtended();

            // Obtain total number of extended fingers
            if (finger.isExtended())
                _iFingersExtended ++;
        }
    }
}

void Logic::processLeapGestures(GestureList Gestures, Hand hand)
{
    return;
    for(Leap::GestureList::const_iterator gl = Gestures.begin(); gl != Gestures.end(); gl++)
    {

        switch((*gl).type())
        {
        case Gesture::TYPE_CIRCLE:
        {
            CircleGesture gesture = CircleGesture(*gl);
            bool bDirection = gesture.pointable().direction().angleTo(gesture.normal()) <= Leap::PI/2;// ? "clockwise" : "counterclockwise";
            int iModeLock = 0;

            //                        qDebug()<<gesture.progress();
            if(bDirection) // clockwise
            {
            }
            else // counterclockwise
            {
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

//                    if(hand.palmNormal().x > 0.50)
//                        iModeLock = scriptEngine->runScript("swipe_right");
//                    if (hand.palmNormal().x < -0.80)
//                        iModeLock = scriptEngine->runScript("swipe_left");

        }
        break;

        case Gesture::TYPE_KEY_TAP:
        {
            KeyTapGesture gesture = KeyTapGesture(*gl);

        }
        break;

        case Gesture::TYPE_SCREEN_TAP:
            break;
        case Gesture::TYPE_INVALID:
            break;
        }
    }
}
