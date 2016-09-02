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
}

void listener::Leap_ControllerStatus()
{
    if(!Leap_Controller->isConnected())
        QTimer::singleShot(1, this, SLOT(Leap_ControllerStatus()));

    Leap_Controller->setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
    emit StartPolling();
}
