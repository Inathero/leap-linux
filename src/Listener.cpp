#include "Listener.h"

LeapListener::LeapListener(QObject *parent) : QObject(parent)
{
    _Controller = new Controller;
    _leapListenTimer = new QTimer(this);

    connect(_leapListenTimer, &QTimer::timeout, this, &LeapListener::leapControllerStatus);

    _leapListenTimer->start(200);
}

void LeapListener::requestPoll()
{
    Leap::Frame frame = _Controller->frame();

    // Controller hasn't updated? kill function
    if (_iPreviousFrameID == frame.id())
        return;

    // Save current frame ID
    _iPreviousFrameID = frame.id();

    // Process Hands
    emit onPollHands(frame.hands());

    // Process Gestures
    emit onPollGestures(frame.gestures(), frame.hands().frontmost());
}

void LeapListener::leapControllerStatus()
{
    // If not ready, check again
    // This is first connection
    if(!_Controller->isConnected() && !(_iInitialConnectFlags & FLAG_INITIAL_CONNECT))
    {
        _leapListenTimer->setInterval(3000);
        qWarning() << "Leap Initialization: Controller Not Found. Trying again in three (3) seconds.";
        return;
    }
    // If we lost connection during usage, after initial connection
    else if(!_Controller->isConnected() && (_iInitialConnectFlags & FLAG_INITIAL_CONNECT))
    {
        _iInitialConnectFlags |= FLAG_RECONNECT;
        emit stopPolling();
        qWarning() << "WARNING: Lost connection to Leap Controller Device. Retrying Connection in five (5) seconds.";
        return;
    }
    // Controller is connected
    else if(_Controller->isConnected())
    {
        _leapListenTimer->setInterval(5000);
        if((_iInitialConnectFlags & (FLAG_INITIAL_CONNECT | FLAG_RECONNECT)) == (FLAG_INITIAL_CONNECT | FLAG_RECONNECT))
        {
            qDebug() << "Connection to Leap Controller Device Re-established!";
            _iInitialConnectFlags = 0;
        }
        if (!(_iInitialConnectFlags & FLAG_INITIAL_CONNECT))
        {
            qDebug() << "Leap Initialization: Leap Controller Found!";
            _iInitialConnectFlags = FLAG_INITIAL_CONNECT;
            // Ready, stop checking and start polling
            _Controller->setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

            // Has to be a better way to iterate through something like this
//            if(Logic->scriptEngine->bLeapGesturesEnable[0])
//            {
            _Controller->enableGesture(Gesture::TYPE_SWIPE);
            qDebug() << "Leap Gesture: TYPE_SWIPE Enabled";
//            }
//            if(Logic->scriptEngine->bLeapGesturesEnable[1])
//            {
            _Controller->enableGesture(Gesture::TYPE_CIRCLE);
            qDebug() << "Leap Gesture: TYPE_CIRCLE Enabled";
//            }
//            if(Logic->scriptEngine->bLeapGesturesEnable[2])
//            {
            _Controller->enableGesture(Gesture::TYPE_KEY_TAP);
            qDebug() << "Leap Gesture: TYPE_KEY_TAP Enabled";
//            }
//            if(Logic->scriptEngine->bLeapGesturesEnable[3])
//            {
            _Controller->enableGesture(Gesture::TYPE_SCREEN_TAP);
            qDebug() << "Leap Gesture: TYPE_SCREEN_TAP Enabled";
//            }

            emit startPolling();
        }
    }
}
