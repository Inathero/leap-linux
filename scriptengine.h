#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QProcess>
#include <QHash>


#include "xkeys.h"

// if you add more commands. change size of bool in runScript
enum script_command_enums
{
    com_key_down,
    com_key_up,
    com_key_send,
    com_launch,
    com_mode_lock,
    com_key_press
};

class scriptengine : public QObject
{
    Q_OBJECT
public:
    scriptengine();
    void setScriptFile(QString sPathToScript);
    void setDefinitions(QString sPathToDefines);
private:
    QList<QByteArray> getScriptSection(QString mode_id);

public slots:
    int runScript(QString mode_id);
private:
    QByteArray baScriptData;
    QHash<QString,int> hDefines;
    QString sScriptFile;
    QStringList slModeList;
    QStringList slCommandList;
    xkeys * XKeys;
};

#endif // SCRIPTENGINE_H
