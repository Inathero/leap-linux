#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include "Leap.h"
#include "macro.h"
#include "scriptengine.h"

#define LEAP_HAND_RIGHT 0
#define LEAP_HAND_LEFT  1
#define LEAP_HAND_BOTH  2

using namespace Leap;

class logic : public QObject
{
    Q_OBJECT
public:
    explicit logic(QObject *parent = 0);

signals:

public slots:
    void Leap_Hands         (Leap::HandList Hands);
    void Leap_FingerSetup   (Leap::FingerList Fingers);
    void Leap_Gestures      (Leap::GestureList Gestures, Leap::Hand hand);
private:
    inline void logic_hand_debug(Leap::Hand hand);

    macro * Macro;
    scriptengine * ScriptEngine;
    bool bHandKeyRot = false;
    bool bThumbKeyRot = false;
    bool bFingersExtended[4] = {false};
    int iFingersExtended;
    bool bThumbExtended;
    int iHandActive;
};

#endif // LOGIC_H
