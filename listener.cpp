#include "listener.h"

listener::listener(QObject *parent) : QObject(parent)
{

}

void listener::onConnect(const Controller &)
{
    qDebug() << "listener::onConnect";
}

void listener::onFrame(const Controller &)
{

}
