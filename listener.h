#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "Leap.h"
#include "logic.h"

using namespace Leap;

class listener : public QObject
{
    Q_OBJECT
public:
    explicit listener(QObject *parent = 0);

    void onPoll();

signals:
    void StartPolling();

public slots:
private slots:
    void Leap_ControllerStatus();
private:
    logic * Logic;
    Controller * Leap_Controller;
    int iPreviousFrameID;
};

#endif // LISTENER_H
