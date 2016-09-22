#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include "Leap.h"
#include "macro.h"
#include "timer.h"
#include "scriptengine.h"

using namespace Leap;


class logic : public QObject
{
    Q_OBJECT
public:
    explicit logic(QObject *parent = 0);

    scriptengine * ScriptEngine;
signals:

public slots:
    void Leap_Hands         (Leap::HandList Hands);
    void Leap_FingerSetup   (Leap::FingerList Fingers);
    void Leap_Gestures      (Leap::GestureList Gestures, Leap::Hand hand);
private:
    inline void logic_hand_debug(Leap::Hand hand);

    QList<macro_block_struct> qlMacroBlocks;

    macro * Macro;
    bool bHandKeyRot = false;
    bool bThumbKeyRot = false;
    bool bFingersExtended[4];
    int iFingersExtended;
    bool bThumbExtended;
    bool bPinch;
    bool bDebugLeftFist;
    bool bStillProcessing;
    int iHandActive;

    const int iTempPinchFrequency = 20;
    const int iTempPinchModifier = 10;
    int iGenericCounter;

    timer * Timer;

    Leap::Vector lvPinchPalmReference;
};

#endif // LOGIC_H
