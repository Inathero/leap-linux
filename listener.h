#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QDebug>
#include "Leap.h"

using namespace Leap;

class listener : public QObject, public Leap::Listener
{
    Q_OBJECT
public:
    explicit listener(QObject *parent = 0);

    void onConnect(const Controller&);
    void onFrame(const Controller&);

signals:

public slots:
};

#endif // LISTENER_H
