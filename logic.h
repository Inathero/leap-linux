#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include "Leap.h"
#include "timer.h"
#include "scriptengine.h"

#include "AudioProgressBarDialog.h"
using namespace Leap;


class logic : public QObject
{
    Q_OBJECT
public:
    explicit logic(QObject *parent = 0);

    scriptengine * scriptEngine;
signals:

public slots:
    void leapHands         (Leap::HandList Hands);
    void leapFingerSetup   (Leap::FingerList Fingers);
    void leapGestures      (Leap::GestureList Gestures, Leap::Hand hand);
private:
    inline void logicHandDebug(Leap::Hand hand);

    QList<macro_block_struct> qlMacroBlocks;

    bool _bHandKeyRot = false;
    bool _bThumbKeyRot = false;
    bool _bFingersExtended[4];
    int _iFingersExtended;
    bool _bThumbExtended;
    bool _bPinch;
    bool _bDebugLeftFist;
    bool _bStillProcessing;
    int _iHandActive;

    const int _iTempPinchFrequency = 20;
    const int _iTempPinchModifier = 10;
    int _iGenericCounter;

    timer * _timer;

    Leap::Vector _lvPinchPalmReference;

    AudioProgressBarDialog * AudioDialog;
};

#endif // LOGIC_H
