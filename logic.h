#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>

#include "Leap.h"
#include "macro.h"
#include "xkeys.h"

using namespace Leap;

class logic : public QObject
{
    Q_OBJECT
public:
    explicit logic(QObject *parent = 0);

signals:

public slots:
    void Leap_Hands     (Leap::HandList Hands);
    void Leap_Gestures  (Leap::GestureList Gestures);
private:
    xkeys * XKeys;
    macro * Macro;
};

#endif // LOGIC_H
