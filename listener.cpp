#include "listener.h"

listener::listener(QObject *parent) : QObject(parent)
{

    Logic = new logic;
    Leap_Controller = new Controller;
    iInitialConnectFlags = 0;
LeapListenTimer = new QTimer(this);
connect(LeapListenTimer, &QTimer::timeout, this, &listener::Leap_ControllerStatus);
LeapListenTimer->start(200);
}

void listener::onPoll()
{
    Leap::Frame frame = Leap_Controller->frame();

    // Controller hasn't updated? kill function
    if (iPreviousFrameID == frame.id())
        return;

    // Save current frame ID
    iPreviousFrameID = frame.id();

    // Process Hands
    Logic->Leap_Hands(frame.hands());

    // Process Gestures
    Logic->Leap_Gestures(frame.gestures(), frame.hands().frontmost());
}

void listener::Leap_ControllerStatus()
{
    // If not ready, check again
    // This is first connection
    if(!Leap_Controller->isConnected() && !(iInitialConnectFlags & FLAG_INITIAL_CONNECT))
    {
        LeapListenTimer->setInterval(3000);
        qWarning() << "Leap Initialization: Controller Not Found. Trying again in three (3) seconds.";
        return;
    }
    // If we lost connection during usage, after initial connection
    else if(!Leap_Controller->isConnected() && (iInitialConnectFlags & FLAG_INITIAL_CONNECT))
    {
        iInitialConnectFlags |= FLAG_RECONNECT;
        emit StopPolling();
        qWarning() << "WARNING: Lost connection to Leap Controller Device. Retrying Connection in five (5) seconds.";
        return;
    }
    // Controller is connected
    else if(Leap_Controller->isConnected())
    {
        LeapListenTimer->setInterval(5000);
        if((iInitialConnectFlags & (FLAG_INITIAL_CONNECT | FLAG_RECONNECT)) == (FLAG_INITIAL_CONNECT | FLAG_RECONNECT))
        {
            qDebug() << "Connection to Leap Controller Device Re-established!";
            iInitialConnectFlags = 0;
        }
        if (!(iInitialConnectFlags & FLAG_INITIAL_CONNECT))
        {
            qDebug() << "Leap Initialization: Leap Controller Found!";
            iInitialConnectFlags = FLAG_INITIAL_CONNECT;
            // Ready, stop checking and start polling
            Leap_Controller->setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

            // Has to be a better way to iterate through something like this
            if(Logic->ScriptEngine->bLeapGesturesEnable[0])
            {
                Leap_Controller->enableGesture(Gesture::TYPE_SWIPE);
                qDebug() << "Leap Gesture: TYPE_SWIPE Enabled";
            }
            if(Logic->ScriptEngine->bLeapGesturesEnable[1])
            {
                Leap_Controller->enableGesture(Gesture::TYPE_CIRCLE);
                qDebug() << "Leap Gesture: TYPE_CIRCLE Enabled";
            }
            if(Logic->ScriptEngine->bLeapGesturesEnable[2])
            {
                Leap_Controller->enableGesture(Gesture::TYPE_KEY_TAP);
                qDebug() << "Leap Gesture: TYPE_KEY_TAP Enabled";
            }
            if(Logic->ScriptEngine->bLeapGesturesEnable[3])
            {
                Leap_Controller->enableGesture(Gesture::TYPE_SCREEN_TAP);
                qDebug() << "Leap Gesture: TYPE_SCREEN_TAP Enabled";
            }
            QHash<QString, float>::iterator i = Logic->ScriptEngine->hLeapGestureMods.begin();
            while (i != Logic->ScriptEngine->hLeapGestureMods.end())
            {
                Leap_Controller->config().setFloat(i.key().toLocal8Bit().data(), i.value());
                qDebug() << "Setting Leap Controller Mods:" << i.key() << "=" << i.value();
                ++i;
            }
            Leap_Controller->config().save();
            emit StartPolling();
        }
    }
}
