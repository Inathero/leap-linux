#include "scriptengine.h"

scriptengine::scriptengine()
{
    slModeList << "swipe_left";
    slModeList << "swipe_right";
    slModeList << "swipe_up";
    slModeList << "swipe_down";
    slModeList << "circle_clockwise";
    slModeList << "circle_counterclockwise";
    slModeList << "finger_tap";
    slModeList << "alms_giver";
    slModeList << "hand_key";

    slCommandList << "key_down";
    slCommandList << "key_up";
    slCommandList << "key_send";
    slCommandList << "launch";
    slCommandList << "mode_lock";
    slCommandList << "key_press";


    XKeys = new xkeys(this);
    tFileUpdateTimer = new QTimer(this);
    connect(tFileUpdateTimer, &QTimer::timeout, this, &scriptengine::updateScriptFile);
}

void scriptengine::setScriptFile(QString sPathToScript)
{
    sScriptFile = sPathToScript;
    QFile fCheck(sScriptFile);
    if (!fCheck.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "Unable to find script file. Can not load macros";
    else
    {
        qDebug() << "Script file located";
        baScriptData = fCheck.readAll();
    }
    fCheck.close();
    tFileUpdateTimer->start(10000);
}

void scriptengine::setDefinitions(QString sPathToDefines)
{
    QByteArray baDefines;
    QFile fDefines(sPathToDefines);
    if (!fDefines.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "Unable to find defines file. Can not load defines: " << sPathToDefines;
    else
    {
        qDebug() << "defines file located";
        baDefines = fDefines.readAll();
    }
    fDefines.close();

    QList<QByteArray> qlDefines = baDefines.split('\n');
    foreach (QByteArray baDefine, qlDefines)
    {
        // Ignore comments
        if (baDefine.at(0) == '#')
            continue;
        QList<QByteArray> qlTokenize = baDefine.split(' ');
        qlTokenize.removeAll("");


        if (qlTokenize.size() > 1)
            hDefines.insert(qlTokenize.at(0), qlTokenize.at(1).toUInt(NULL, 16));
        else
            qWarning() << "Incomplete define. Unable to add to Defines: " << qlTokenize;
    }
}

QList<QByteArray> scriptengine::getScriptSection(QString mode_id)
{
    QList<QByteArray> slScripts = baScriptData.split('\n');
    QList<QByteArray> slScriptSection;
    bool bMode = false;
    foreach (QByteArray baScript, slScripts)
    {
        // Ignore comments
        if (baScript.at(0) == '#')
            continue;

        QList<QByteArray> qlTokenize = baScript.split(' ');
        qlTokenize.removeAll("");

        // We are currently on the mode we want:
        if (bMode)
        {
            // End of current mode, entering another mode
            if (qlTokenize.size() > 1 && qlTokenize.at(0) == "mode")
                return slScriptSection;
            foreach (QByteArray baScriptData, qlTokenize)
                slScriptSection.append(baScriptData);
        }


        // Found a mode
        if (qlTokenize.size() > 1 && qlTokenize.at(0) == "mode")
            // We have the correct mode
            if(qlTokenize.at(1).contains(mode_id.toLocal8Bit()))
                bMode = true;
    }
    return slScriptSection;
}

int scriptengine::runScript(QString mode_id)
{
    qDebug() << "scriptengine::runScript:" << mode_id;
    int iModeLock = 0;
    QList<QByteArray> slScriptSection = getScriptSection(mode_id);

    // We found the mode
    if (!slScriptSection.isEmpty())
    {
        bool bCommand[slCommandList.size()] = {false};

        foreach(QByteArray baScript, slScriptSection)
        {
            int iCommand = slCommandList.indexOf(baScript);

            // Found a command
            if (iCommand != -1)
                bCommand[iCommand] = true;

            // No command found, therefore arguments to command
            // arguments with no command = nothing happens
            else
            {
                if(bCommand[com_key_down])
                {
                    bCommand[com_key_down] = false;

                    // Check if we use define or not
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_down: defines :"<<baScript<<":"<<hDefines.value(baScript);
                        XKeys->key_down (hDefines.value(baScript));
                    }

                    // No values, single key statement instead
                    else
                    {
                        qDebug() <<"key_down: "<<baScript.at(0);
                        if (baScript.at(0) < 90)
                        {
                            XKeys->key_down(XK_Shift_L);
                            XKeys->key_down(baScript.at(0));
                        }
                        else
                        {
                        XKeys->key_down(baScript.at(0));
                        }
                    }

                }
                if(bCommand[com_key_up])
                {
                    bCommand[com_key_up] = false;

                    // Check if we use define or not
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_up: defines :"<<baScript<<":"<<hDefines.value(baScript);
                        XKeys->key_up (hDefines.value(baScript));
                    }

                    // No values, single key statement instead
                    else
                    {
                        qDebug() <<"key_up: "<<baScript.at(0);
                        if (baScript.at(0) < 90)
                        {
                            XKeys->key_up(XK_Shift_L);
                            XKeys->key_up(baScript.at(0));
                        }
                        else
                        XKeys->key_up(baScript.at(0));
                    }
                }
                if(bCommand[com_key_send])
                {

                    qDebug() <<"key_send: "<<baScript;
                    bCommand[com_key_send] = false;
                    foreach(QChar cChar, baScript)
                    {
                        if (cChar.unicode() < 90)
                        {
                            qDebug() << "shift - "<< ShiftMask;
                            XKeys->key_down(XK_Shift_L);
                            XKeys->key_down(cChar.unicode());
                            XKeys->key_up(cChar.unicode());
                            XKeys->key_up(XK_Shift_L);
                        }
                        else
                        {
                        XKeys->key_down(cChar.unicode());
                        XKeys->key_up(cChar.unicode());
                        }
                    }
                }
                if(bCommand[com_launch])
                {
                    bCommand[com_launch] = false;

                    qDebug() <<"launch: "<<baScript;
                    QProcess::startDetached(baScript);
                }
                if(bCommand[com_mode_lock])
                {
                    bCommand[com_mode_lock] = false;

                    qDebug() <<"mode_lock: "<<baScript.toInt();
                    iModeLock = baScript.toInt();
                }
                if(bCommand[com_key_press])
                {
                    bCommand[com_key_press] = false;

                    // Check if we use define or not
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_press: defines :"<<baScript<<":"<<hDefines.value(baScript);
                        XKeys->key_down (hDefines.value(baScript));
                        XKeys->key_up (hDefines.value(baScript));
                    }

                    // No values, single key statement instead
                    else
                    {

                        qDebug() <<"key_press: "<<baScript.at(0);
                        if (baScript.at(0) < 90)
                        {
                            XKeys->key_down(XK_Shift_L);
                            XKeys->key_down(baScript.at(0));
                            XKeys->key_up(baScript.at(0));
                            XKeys->key_up(XK_Shift_L);
                        }
                        else
                        {
                        XKeys->key_down(baScript.at(0));
                        XKeys->key_up(baScript.at(0));
                        }
                    }
                }
            }
        }
    }

    return iModeLock;
}

void scriptengine::updateScriptFile()
{
 setScriptFile(sScriptFile);
}

