#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "Leap.h"

using namespace Leap;


#define FLAG_INITIAL_CONNECT 1
#define FLAG_RECONNECT 2

class LeapListener : public QObject
{
    Q_OBJECT
signals:
    void startPolling();
    void stopPolling();

    void onPollHands(Leap::HandList);
    void onPollGestures(GestureList, Hand);

public:
    explicit LeapListener(QObject *parent = 0);
public slots:
    void requestPoll();


private slots:
    void leapControllerStatus();
private:
    Controller * _Controller;
    QTimer * _leapListenTimer;
    int _iPreviousFrameID;
    int _iInitialConnectFlags = 0;
};

#endif // LISTENER_H
