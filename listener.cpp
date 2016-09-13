#include "listener.h"

listener::listener(QObject *parent) : QObject(parent)
{

    Logic = new logic;
    Leap_Controller = new Controller;

    QTimer::singleShot(1, this, SLOT(Leap_ControllerStatus()));
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
    if(!Leap_Controller->isConnected())
        QTimer::singleShot(1, this, SLOT(Leap_ControllerStatus()));

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
