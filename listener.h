#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QDebug>

#include "Leap.h"
#include "logic.h"

using namespace Leap;

class listener : public QObject, public Leap::Listener
{
    Q_OBJECT
public:
    explicit listener(QObject *parent = 0);

    void onConnect(const Controller & );
    void onFrame(const Controller & controller);

signals:


public slots:

private:
    logic * Logic;
};

#endif // LISTENER_H
