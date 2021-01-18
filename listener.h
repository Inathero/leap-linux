#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "Leap.h"
#include "logic.h"

using namespace Leap;


#define FLAG_INITIAL_CONNECT 1
#define FLAG_RECONNECT 2

class listener : public QObject
{
    Q_OBJECT
public:
    explicit listener(QObject *parent = 0);

    void onPoll();

    logic * Logic;
signals:
    void StartPolling();
    void StopPolling();

public slots:
private slots:
    void Leap_ControllerStatus();
private:
    Controller * Leap_Controller;
    QTimer * LeapListenTimer;
    int iPreviousFrameID;
    int iInitialConnectFlags;
};

#endif // LISTENER_H
