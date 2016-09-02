#include "listener.h"

listener::listener(QObject *parent) : QObject(parent)
{
    Logic = new logic;
    Leap_Controller = new Controller;

    Leap_Controller->setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
}

void listener::onPoll()
{
    Leap::Frame frame = Leap_Controller->frame();

    if (iPreviousFrameID == frame.id())
        return;

    iPreviousFrameID = frame.id();
    Logic->Leap_Hands(frame.hands());
}
