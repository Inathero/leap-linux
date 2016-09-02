#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>

#include "Leap.h"

using namespace Leap;

class logic : public QObject
{
    Q_OBJECT
public:
    explicit logic(QObject *parent = 0);

signals:

public slots:
    void Leap_Hands(Leap::HandList Hands);
};

#endif // LOGIC_H
