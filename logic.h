#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include "Leap.h"
#include "macro.h"
#include "scriptengine.h"

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
    bool bFingersExtended[4];
    int iFingersExtended;
    bool bThumbExtended;
    bool bDebugLeftFist;
    int iHandActive;
};

#endif // LOGIC_H
