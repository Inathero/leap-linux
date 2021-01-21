#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include "Leap.h"

#include "AudioProgressBarDialog.h"
using namespace Leap;


class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = 0);


    AudioProgressBarDialog * getAudioDialog()
    {
        return _AudioDialog;
    }
signals:

public slots:
    void processLeapHands         (Leap::HandList Hands);
    void processLeapFingers   (Leap::FingerList Fingers);
    void processLeapGestures      (Leap::GestureList Gestures, Leap::Hand hand);
private:
    inline void logicHandDebug(Leap::Hand hand);


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

    Leap::Vector _lvPinchPalmReference;

    AudioProgressBarDialog * _AudioDialog;
    bool _bFistToggle;
};

#endif // LOGIC_H
