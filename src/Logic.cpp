#include "Logic.h"
#include "Debug.h"


Logic::Logic(QObject *parent) : QObject(parent)
{
    _bPinch = false;
    _iGenericCounter = 0;

    _AudioDialog = new AudioProgressBarDialog;
    _timerGestureUnlock = new QTimer;
    _timerGestureUnlock->setInterval(500);
    connect(_timerGestureUnlock, &QTimer::timeout, this, [=]()
    {
        _Commands.unlockGestures();
    });
}

inline void Logic::logicHandDebug(Hand hand)
{
//    qDebug() <<  hand.pinchStrength();
//    db hand.palmNormal().x;
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
        // Delay prevents gestures and commands from running
        // while user puts hand into field.
        // Also prevents random stuff from activating commands
        if(_bProcessDelay)
        {
            QTimer::singleShot(500, this, [this]()
            {
                _bProcessDelay = false;
            });
        }
//        _iGenericCounter ++;
        for (auto hand : Hands)
        {

            //            qDebug() << "HAND:" << iHandActive;
//            scriptEngine->preScript("HandMod", _iHandActive);

            logicHandDebug(hand);

            processLeapFingers(hand.fingers());

            if(abs(hand.palmNormal().x) < 0.1 && !_bFlatToVerticalPalm && _iFingersExtended > 3)
                _bFlatToVerticalPalm = true;
            if(hand.palmNormal().x < -0.9 && _bFlatToVerticalPalm)
            {
                _bFlatToVerticalPalm = false;

                _Commands.runCommand("flat_palm_to_vertical");
//                QProcess *p = new QProcess;
//                connect(p,static_cast<void (QProcess::*)(int)>(&QProcess::finished), p, &QProcess::deleteLater);
//                p->start("mpc", QStringList() << "toggle");

            }

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
//                    return;willneff

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
            if (hand.pinchStrength() > 0.7)
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


//                        qDebug() << "pinch - " << lvStabPalmPos.y;
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

    }
    else
    {
        _iHandActive = -1;
        _bHandKeyRot = false;
        _bThumbKeyRot= false;
        _bFistToggle = false;
        _bFlatToVerticalPalm = false;
        _bProcessDelay = true;
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
        {
            _bThumbExtended = finger.isExtended();
            _iFingerStart = finger.id();
        }

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
    if(_bProcessDelay) return;
    for(Leap::GestureList::const_iterator gl = Gestures.begin(); gl != Gestures.end(); gl++)
    {

        if((*gl).state() == Gesture::STATE_STOP)
        {
            // Some gestures, like swipe, should only be called once
            // Unfortauntely, this stop state is called multiple times.
            // Setting it on a timer prolongs reset if called multiple times
            _timerGestureUnlock->start();
        }
        switch((*gl).type())
        {
        case Gesture::TYPE_CIRCLE:
        {
            CircleGesture gesture = CircleGesture(*gl);
            bool bDirection = gesture.pointable().direction().angleTo(gesture.normal()) <= Leap::PI/2;// ? "clockwise" : "counterclockwise";

            // New circle rotations will always start at 0
            // so lets use this as the reset event
            if(floor(gesture.progress()) < 0.9)
            {
//                db "reset circle";
                _iCircleRotations = 0;
            }

            if(bDirection) // clockwise
            {
                _Commands.runCommand("circle_clockwise");
                if(_iCircleRotations < floor(gesture.progress()))
                {
//                    db  (gesture.progress()) << " - " <<  _iCircleRotations;
                    _iCircleRotations ++;
                    _Commands.runCommand("circle_clockwise_full_rotation");
                }

            }
            else // counterclockwise
            {
                _Commands.runCommand("circle_counter_clockwise");
                if(_iCircleRotations < floor(gesture.progress()))
                {
//                    db  (gesture.progress()) << " - " <<  _iCircleRotations;
                    _iCircleRotations ++;
                    _Commands.runCommand("circle_counter_clockwise_full_rotation");
                }
            }
        }
        break;

        case Gesture::TYPE_SWIPE:
        {
            SwipeGesture gesture = SwipeGesture(*gl);
            if(gesture.direction().x < -0.9)
                _Commands.runCommand("swipe_left");
            else if(gesture.direction().x  > 0.9)
                _Commands.runCommand("swipe_right");
            else if(gesture.direction().y  > 0.9)
                _Commands.runCommand("swipe_up");
            else if(gesture.direction().y < -0.9)
                _Commands.runCommand("swipe_down");
            else if(gesture.direction().x < -0.45)
            {
                if(gesture.direction().y > 0.45)
                    _Commands.runCommand("swipe_leftup");
                if(gesture.direction().y < -0.45)
                    _Commands.runCommand("swipe_leftdown");
            }
            else if(gesture.direction().x > 0.45)
            {
                if(gesture.direction().y > 0.45)
                    _Commands.runCommand("swipe_rightup");
                if(gesture.direction().y < -0.45)
                    _Commands.runCommand("swipe_rightdown");
            }
        }
        break;

        case Gesture::TYPE_KEY_TAP:
        {
            KeyTapGesture gesture = KeyTapGesture(*gl);
            switch(gesture.pointable().id() - _iFingerStart)
            {
            case 0:
                _Commands.runCommand("thumb_finger_tap");
                break;
            case 1:
                _Commands.runCommand("index_finger_tap");
                break;
            case 2:
                _Commands.runCommand("middle_finger_tap");
                break;
            case 3:
                _Commands.runCommand("ring_finger_tap");
                break;
            case 4:
                _Commands.runCommand("pinkie_finger_tap");
                break;
            }

        }
        break;

        case Gesture::TYPE_SCREEN_TAP:
            db "screen tap";
            break;
        case Gesture::TYPE_INVALID:
            break;
        }
    }
}
