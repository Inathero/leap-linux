#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <qdatastream.h>
#include <QDataStream>
#include <QObject>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QProcess>
#include <QHash>
#include <QTimer>

#if __unix
#include "xkeys.h"
#include "xmouse.h"
#elif _WIN32
#include <qt_windows.h>
#include "winkeys.h"
#include "winmouse.h"
#endif

#define FINGER_MOD  1
#define HAND_MOD    2

#if _WIN32
#define XK_Shift_L VK_SHIFT
#endif

#define LEAP_HAND_RIGHT         0
#define LEAP_HAND_LEFT          1
#define LEAP_HAND_BOTH          2
#define LEAP_HAND_BOTH_LEFT     3
#define LEAP_HAND_BOTH_RIGHT    4

// if you add more commands. change size of bool in runScript
enum script_command_enums
{
    com_key_down,
    com_key_up,
    com_key_send,
    com_launch,
    com_mode_lock,
    com_key_press,
    com_mouse_click
};

class scriptengine : public QObject
{
    Q_OBJECT
public:
    scriptengine();
    void setScriptFile(QString sPathToScript);
    void setDefinitions(QString sPathToDefines);
public slots:
    int runScript(QString mode_id);
    void preScript(QString sVarName, int iVar);
    void debug(float x, float y);
    void debugMouseDown();
    void debugMouseUp();
private slots:
    void updateScriptFile();
    QList<QByteArray> getScriptSection(QString base_mode_id);
    void getScriptModeIndexes();
private:
    QByteArray baScriptData;
    QHash<QString,int> hDefines;
    QString sScriptFile;

    bool * bCommand;
    QList<int> ilModeIndexes;

    QStringList slModeList;
    QStringList slCommandList;
    QStringList slMouseButtonList;
    QStringList slPreScriptList;
    QStringList slMouseTypeList;
#if __unix
    xkeys * Key_Sim;
    xmouse * Mouse_Sim;
#elif _WIN32
    winkeys * Key_Sim;
    winmouse * Mouse_Sim;
#endif
    QTimer * tFileUpdateTimer;

    int * LeapMouseRect;

    int iModifiers;
    QString sFingerMod;
    QString sHandMod;
};

#endif // SCRIPTENGINE_H
