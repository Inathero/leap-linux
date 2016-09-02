#include "listener.h"

listener::listener(QObject *parent) : QObject(parent)
{
    Logic = new logic;
}

void listener::onConnect(const Controller &)
{
    qDebug() << "listener::onConnect";
}

void listener::onFrame(const Controller & controller)
{
    const Frame frame = controller.frame();

    Logic->Leap_Hands(frame.hands());
}
