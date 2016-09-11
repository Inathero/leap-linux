#include "scriptengine.h"

scriptengine::scriptengine()
{
#if _LINUX
    Key_Sim = new xkeys(this);
    Mouse_Sim = new xmouse(this);
#elif _WIN32
    Key_Sim = new winkeys(this);
    Mouse_Sim = new winmouse(this);
#endif
    slModeList << "swipe_left";
    slModeList << "swipe_right";
    slModeList << "swipe_up";
    slModeList << "swipe_down";
    slModeList << "circle_clockwise";
    slModeList << "circle_counterclockwise";
    slModeList << "finger_tap";
    slModeList << "alms_giver";
    slModeList << "hand_key";
    slModeList << "thumb_up";
    slModeList << "thumb_down";

    slCommandList << "key_down";
    slCommandList << "key_up";
    slCommandList << "key_send";
    slCommandList << "launch";
    slCommandList << "mode_lock";
    slCommandList << "key_press";
    slCommandList << "mouse_click";

    slMouseButtonList << "left";
    slMouseButtonList << "middle";
    slMouseButtonList << "right";

    slPreScriptList << "FingerMod";
    slPreScriptList << "HandMod";

    iModifiers = 0;

    tFileUpdateTimer = new QTimer(this);
    LeapMouseRect = new int[4];
    connect(tFileUpdateTimer, &QTimer::timeout, this, &scriptengine::updateScriptFile);
}

void scriptengine::setScriptFile(QString sPathToScript)
{
    sScriptFile = sPathToScript;
    QFile fCheck(sScriptFile);
    if (!fCheck.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "Unable to find script file. Can not load macros";
    else
        baScriptData = fCheck.readAll();

    fCheck.close();
    tFileUpdateTimer->start(5000);

    getScriptModeIndexes();
}

void scriptengine::setDefinitions(QString sPathToDefines)
{
    QByteArray baDefines;
    QFile fDefines(sPathToDefines);
    if (!fDefines.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "Unable to find defines file. Can not load defines: " << sPathToDefines;
    else
    {
        qDebug() << "Defines File Located";
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

        if (qlTokenize.isEmpty())
                    continue;

        // Non-Macro defines
        if (qlTokenize.at(0) == "Leap_Mouse_Width_Range")
        {
            // Try-catch doesn't work on mingw Qt, how disappointing
            if (qlTokenize.size() != 3)
            {
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Width_Range_Min defined with no value. Using -100 as default";
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Width_Range_Max defined with no value. Using 100 as default";
                LeapMouseRect[0] = -100;
                LeapMouseRect[1] = 100;
            }
            else
            {
                LeapMouseRect[0] = qlTokenize.at(1).toInt();
                LeapMouseRect[1] = qlTokenize.at(2).toInt();
            }
            continue;
        }
        if (qlTokenize.at(0) == "Leap_Mouse_Height_Range")
        {

            // Try-catch doesn't work on mingw Qt, how disappointing
            if (qlTokenize.size() != 3)
            {
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Height_Min defined with no value. Using 70 as default";
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Height_Max defined with no value. Using 250 as default";
                LeapMouseRect[2] = 70;
                LeapMouseRect[3] = 250;

            }
            else
            {
                LeapMouseRect[2] = qlTokenize.at(1).toInt();
                LeapMouseRect[3] = qlTokenize.at(2).toInt();
            }
            continue;
        }
        if (qlTokenize.size() > 1)
            hDefines.insert(qlTokenize.at(0), qlTokenize.at(1).toUInt(NULL, 16));
        else
            qWarning() << "Incomplete define. Unable to add to Defines: " << qlTokenize;
    }
    Mouse_Sim->mouse_set_leap_ranges(LeapMouseRect);
}

QList<QByteArray> scriptengine::getScriptSection(QString base_mode_id)
{
    QList<QByteArray> slScripts = baScriptData.split('\n');
    QList<QByteArray> slScriptSection;
    QString mode_id = base_mode_id;

    // HACK: Because I don't want to remove my `+ ++iIterate` crazyness
    // add a blank mode at the of slScripts to prevent crash
    // Crash is due to checking for next 'mode'. If current mode is last mode, it'll check out of bounds = crash
    slScripts.append("mode");

    if ( iModifiers & FINGER_MOD )
        mode_id.append(sFingerMod);

    if ( iModifiers & HAND_MOD )
        mode_id.prepend(sHandMod);

    qDebug() << "scriptengine::getScriptSection:" << mode_id;

    int xIterate = 2;
    do
    {
        for (int i = 0; i < ilModeIndexes.size(); i++)
        {
            // Map cached indexes to actual script
            QByteArray baScript = slScripts.at(ilModeIndexes.at(i));

            QList<QByteArray> qlTokenize = baScript.split(' ');
            qlTokenize.removeAll("");

            int iCut = qlTokenize.at(1).indexOf(mode_id.toLocal8Bit());
            // Found mode of interest
            if (iCut == 0)
            {
                // Go one line past mode, into section
                int iIterate = 0;
                baScript = slScripts.at(ilModeIndexes.at(i) + ++iIterate);

                qlTokenize = baScript.split(' ');
                qlTokenize.removeAll("");

                // While we don't reach the next mode
                while (qlTokenize.at(0) != "mode")
                {
                    // Ignore comments when reading through mode
                    // Rip all information per line for mode
                    if (baScript.at(0) != '#')
                    {
                        foreach (QByteArray baScriptData, qlTokenize)
                            slScriptSection.append(baScriptData);
                    }

                    // Iterate to next line in section
                    baScript = slScripts.at(ilModeIndexes.at(i) + ++iIterate);

                    qlTokenize = baScript.split(' ');
                    qlTokenize.removeAll("");
                }
                return slScriptSection;
            }
        }
        xIterate --;
        mode_id = base_mode_id;
        // strip iModifiers as a fallback to other mods
        switch (xIterate)
        {
            case 1:
            if ( iModifiers & FINGER_MOD )
                mode_id.append(sFingerMod);
            qDebug() << "scriptengine::getScriptSection:" << mode_id;
            break;
        case 0:
            // The previous modifier didn't exist, so we already did a search with no mods and found nothing
            // Therefore return nothing.
            // else, if the previous modifier existed, we haven't done a search with no mods
            // so let loop continue as is
            if ( !(iModifiers & FINGER_MOD) )
                return slScriptSection;
            qDebug() << "scriptengine::getScriptSection:" << mode_id;
            break;
        }
    } while (xIterate >= 0);

return slScriptSection;
}

void scriptengine::getScriptModeIndexes()
{
    QList<QByteArray> slScripts = baScriptData.split('\n');
    QList<int> tilModeIndexes;
    for (int i = 0; i < slScripts.size(); i++)
    {
        QByteArray baScript = slScripts.at(i);

        // Ignore comments
        if (baScript.at(0) == '#')
            continue;

        QList<QByteArray> qlTokenize = baScript.split(' ');
        qlTokenize.removeAll("");
        // Found a mode
        if (qlTokenize.size() > 1 && qlTokenize.at(0) == "mode")
            tilModeIndexes << i;
    }

    // Copy modes over
    ilModeIndexes = tilModeIndexes;
}

int scriptengine::runScript(QString mode_id)
{
    qDebug() << "scriptengine::runScript:" << mode_id;
    int iModeLock = -1;
    QList<QByteArray> slScriptSection = getScriptSection(mode_id);

    // We found the mode
    if (!slScriptSection.isEmpty())
    {
        bool bCommand[slCommandList.size()] = {false};
        bool * bCPointer = bCommand;

        foreach(QByteArray baScript, slScriptSection)
        {
            int iCommand = slCommandList.indexOf(baScript);

            // Found a command
            if (iCommand != -1)
            {
                // Reset previous commands
                for (int i = 0; i < slCommandList.size(); i++)
                    *(bCPointer + i) = false;

                // Activate current command
                bCommand[iCommand] = true;
            }

            // No command found, therefore arguments to command
            // arguments with no command = nothing happens
            else
            {
                if(bCommand[com_key_down])
                {
                    // Check if we use define or not
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_down: Definitions:"<<baScript<<":"<<hDefines.value(baScript);
                        Key_Sim->key_down (hDefines.value(baScript));
                    }

                    // No values, single key statement instead
                    else
                    {
                        qDebug() <<"key_down:"<<baScript.at(0);
                        if (baScript.at(0) < 90)
                        {
                            Key_Sim->key_down(XK_Shift_L);
                            Key_Sim->key_down(baScript.at(0));
                        }
                        else
                        {
                        Key_Sim->key_down(baScript.at(0));
                        }
                    }

                }
                if(bCommand[com_key_up])
                {
                    // Check if we use define or not
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_up: Definitions:"<<baScript<<":"<<hDefines.value(baScript);
                        Key_Sim->key_up (hDefines.value(baScript));
                    }

                    // No values, single key statement instead
                    else
                    {
                        qDebug() <<"key_up:"<<baScript.at(0);
                        if (baScript.at(0) < 90)
                        {
                            Key_Sim->key_up(XK_Shift_L);
                            Key_Sim->key_up(baScript.at(0));
                        }
                        else
                        Key_Sim->key_up(baScript.at(0));
                    }
                }
                if(bCommand[com_key_send])
                {
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_send: Definitions:"<<baScript<<":"<<hDefines.value(baScript);
                        Key_Sim->key_down (hDefines.value(baScript));
                        Key_Sim->key_up (hDefines.value(baScript));
                    }
                    else
                    {
                        qDebug() <<"key_send:"<<baScript;
                    foreach(QChar cChar, baScript)
                    {
                        if (cChar.unicode() < 90)
                        {
                            Key_Sim->key_down(XK_Shift_L);
                            Key_Sim->key_down(cChar.unicode());
                            Key_Sim->key_up(cChar.unicode());
                            Key_Sim->key_up(XK_Shift_L);
                        }
                        else
                        {
                        Key_Sim->key_down(cChar.unicode());
                        Key_Sim->key_up(cChar.unicode());
                        }
                    }
                    }
                }
                if(bCommand[com_launch])
                {
                    qDebug() <<"launch:"<<baScript;
                    QProcess::startDetached(baScript);
                }
                if(bCommand[com_mode_lock])
                {
                    iModeLock = baScript.toInt();
                }
                if(bCommand[com_key_press])
                {
                    // Check if we use define or not
                    if(hDefines.contains(baScript))
                    {
                        qDebug() <<"key_press: Definitions:"<<baScript<<":"<<hDefines.value(baScript);
                        Key_Sim->key_down (hDefines.value(baScript));
                        Key_Sim->key_up (hDefines.value(baScript));
                    }

                    // No values, single key statement instead
                    else
                    {

                        qDebug() <<"key_press:"<<baScript.at(0);
                        if (baScript.at(0) < 90)
                        {
                            Key_Sim->key_down(XK_Shift_L);
                            Key_Sim->key_down(baScript.at(0));
                            Key_Sim->key_up(baScript.at(0));
                            Key_Sim->key_up(XK_Shift_L);
                        }
                        else
                        {
                        Key_Sim->key_down(baScript.at(0));
                        Key_Sim->key_up(baScript.at(0));
                        }
                    }
                }
                if(bCommand[com_mouse_click])
                {

                    qDebug() << "mouse_click:" << baScript;

                    int iMouseButtonIndex = slMouseButtonList.indexOf(baScript.toLower());

                    Mouse_Sim->mouse_button_click(Mouse_Sim_button_type_enum(iMouseButtonIndex+1));

                    }
            }

            // commands that have no arguments

        }
    }
    sFingerMod = "";
    sHandMod = "";
    iModifiers = 0;
    return iModeLock;
}

void scriptengine::preScript(QString sVarName, int iVar)
{
    switch(slPreScriptList.indexOf(sVarName))
    {
        // Finger Mods
        case 0:
        {
            iModifiers = iModifiers | FINGER_MOD;
            switch (iVar)
            {
                case 0:
                case 5:
                sFingerMod = "";
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                sFingerMod = "_";
                sFingerMod.append(QString::number(iVar));
                break;

            }
        }
        break;
        // Hand Mods
        case 1:
            sHandMod = iVar == 2 ? "B_" : iVar == 1 ? "L_" : "R_";
            iModifiers = iModifiers | HAND_MOD;
        break;

    }


}

void scriptengine::debug(float x, float y)
{
    Mouse_Sim->mouse_move(x,y);
}

void scriptengine::updateScriptFile()
{
 setScriptFile(sScriptFile);
}

