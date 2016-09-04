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
    Logic->Leap_Gestures(frame.gestures());
}

void listener::Leap_ControllerStatus()
{
    // If not ready, check again
    if(!Leap_Controller->isConnected())
        QTimer::singleShot(1, this, SLOT(Leap_ControllerStatus()));

    // Ready, stop checking and start polling
    Leap_Controller->setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
    Leap_Controller->enableGesture(Gesture::TYPE_SWIPE);
    Leap_Controller->enableGesture(Gesture::TYPE_CIRCLE);
    Leap_Controller->enableGesture(Gesture::TYPE_KEY_TAP);
//    qDebug() << Leap_Controller->config().getFloat("Gesture.Swipe.MinLength");
    Leap_Controller->config().setFloat("Gesture.Swipe.MinLength", 100);
    Leap_Controller->config().setFloat("Gesture.Swipe.MinVelocity", 1);
    Leap_Controller->config().save();

//            controller.config().setFloat("Gesture.Circle.MinRadius", 15);
//            controller.config().setFloat("Gesture.Circle.MinArc", 1.7);
    emit StartPolling();
}
