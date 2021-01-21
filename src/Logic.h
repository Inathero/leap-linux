#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include "Leap.h"

#include "AudioProgressBarDialog.h"
#include "Commands.h"
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

    void reloadCommandsFromTray()
    {
        _Commands.reloadCommands();
    }

signals:

public slots:
    void processLeapHands         (Leap::HandList Hands);
    void processLeapFingers   (Leap::FingerList Fingers);
    void processLeapGestures      (Leap::GestureList Gestures, Leap::Hand hand);
private:
    inline void logicHandDebug(Leap::Hand hand);

    Commands _Commands;

    bool _bHandKeyRot = false;
    bool _bThumbKeyRot = false;
    bool _bFingersExtended[4];
    int _iFingersExtended;
    bool _bThumbExtended;
    bool _bPinch;
    int _iHandActive;
    bool _bFlatToVerticalPalm = false;

    const int _iTempPinchFrequency = 20;
    const int _iTempPinchModifier = 10;
    int _iGenericCounter;

    Leap::Vector _lvPinchPalmReference;

    AudioProgressBarDialog * _AudioDialog;
    bool _bFistToggle;
    int _iFingerStart;
    bool _bProcessDelay = true;
    int _iCircleRotations ;
    QTimer * _timerGestureUnlock;
};

#endif // LOGIC_H
