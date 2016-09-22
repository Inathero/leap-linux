#include "scriptengine.h"

scriptengine::scriptengine()
{
#if __unix
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

    slMouseTypeList << "absolute";
    slMouseTypeList << "relative";
    slMouseTypeList << "absolute_to_relative";

    slFingerTypeList << "thumb";
    slFingerTypeList << "index";
    slFingerTypeList << "middle";
    slFingerTypeList << "ring";
    slFingerTypeList << "pinkie";

    iModifiers = 0;

    QFSWatcher = new QFileSystemWatcher;
    LeapMouseRect = new int[4];

    connect(QFSWatcher, &QFileSystemWatcher::fileChanged, this, &scriptengine::scriptFileChanged);
}

void scriptengine::setScriptPath(QString sPathToScriptFolder)
{
    sScriptPath = sPathToScriptFolder;
    QFSWatcher->addPath(sScriptPath + QString("/macros.ina"));
    QFSWatcher->addPath(sScriptPath + QString("/defines.ina"));
    qDebug() << "scriptengine::setScriptPath:QFSWatcher_file_list:"<<QFSWatcher->files();

    setScriptFile(sScriptPath + QString("/macros.ina"));
    setDefinitions(sScriptPath + QString("/defines.ina"));

}

void scriptengine::setScriptFile(QString sPathToScript)
{
    QFile fCheck(sPathToScript);
    if (!fCheck.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "scriptengine::setScriptFile: Unable to find script file or currently open in another program. Can not load macros";
    else {
        baScriptData = fCheck.readAll();
        qDebug() << "scriptengine::setScriptFile: Successfully read script file:" << sPathToScript;
    }

    fCheck.close();

    getScriptModeIndexes();
}

void scriptengine::setDefinitions(QString sPathToDefines)
{
    QByteArray baDefines;
    QFile fDefines(sPathToDefines);
    if (!fDefines.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "scriptengine::setDefinitions: Unable to find defines file or currently open in another program. Can not load defines: " << sPathToDefines;
    else
    {
        qDebug() << "scriptengine::setDefinitions: Successfully read definitions file:" << sPathToDefines;
        baDefines = fDefines.readAll();
    }
    fDefines.close();

    QList<QByteArray> qlDefines = baDefines.split('\n');
    QList<QByteArray> qlConfigBlock;
    bool bConfigBlock = false;
    int iConfigBlockType = 0;
    foreach (QByteArray baDefine, qlDefines)
    {
        // Ignore comments
        if (baDefine.at(0) == '#')
            continue;


        QList<QByteArray> qlTokenize = baDefine.split(' ');
        qlTokenize.removeAll("");

        if (qlTokenize.isEmpty())
            continue;

        QString sComp = qlTokenize.at(0);
        if(bConfigBlock)
        {
            // End of config block
            if(qlTokenize.contains("}"))
            {
                // Copy everything up till }
                foreach(QByteArray a, qlTokenize) {
                    if (!a.contains('}'))
                        qlConfigBlock << a;
                    else
                        break;
                }
                // Reset block
                bConfigBlock = false;

                // Copy block to right config parser
                switch(iConfigBlockType)
                {
                case CONFIG_BLOCK_MOUSE:
                    setupLeapMouse(qlConfigBlock);
                    break;
                }
                continue;
            }
            else {
                foreach(QByteArray a, qlTokenize)
                    qlConfigBlock << a;
                continue;
            }
        }
        // Non-Macro defines
        if (sComp.contains("Leap_Mouse_Config"))
        {
            // The beginning '{' in script is actually completely useless
            // only the ending '}' is important
            bConfigBlock = true;
            iConfigBlockType = CONFIG_BLOCK_MOUSE;
        }
        else if (sComp.contains("Leap_Enable_Gesture_"))
        {
            sComp = sComp.replace("Leap_Enable_Gesture_","");
            if(qlTokenize.size() == 2)
            {
                if(sComp == "Swipe")
                    bLeapGesturesEnable[0] = qlTokenize.at(1).toLower() == "true" ? true : false;
                else if(sComp == "Circle")
                    bLeapGesturesEnable[1] = qlTokenize.at(1).toLower() == "true" ? true : false;
                else if(sComp == "Key_Tap")
                    bLeapGesturesEnable[2] = qlTokenize.at(1).toLower() == "true" ? true : false;
                else if(sComp == "Screen_Tap")
                    bLeapGesturesEnable[3] = qlTokenize.at(1).toLower() == "true" ? true : false;
            }
            else
                qWarning() << QString("Leap_Enable_Gesture_").append(sComp) << " is UNDEFINED";
        }
        else if (sComp.contains("Gesture."))
        {
            if(qlTokenize.size() == 2)
                hLeapGestureMods.insert(sComp, qlTokenize.at(1).toFloat());
            else
                qWarning() << sComp << " is detect but not defined. Will fallback to Leap Controller's Defaults";
        }
        else if (qlTokenize.size() > 1)
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
        int tSize = slCommandList.size();
        bCommand = new bool[tSize];
        //        = {false};
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
#if _WIN32
                            if ( int(cChar.toLatin1()) >= 65 && int(cChar.toLatin1()) <= 90)
                            {
                                Key_Sim->key_down(XK_Shift_L);
                                Key_Sim->key_down(cChar.unicode());
                                Key_Sim->key_up(cChar.unicode());
                                Key_Sim->key_up(XK_Shift_L);
                            }
                            else if ( int(cChar.toLatin1()) >= 97 && int(cChar.toLatin1()) <= 122)
                            {
                                cChar = QChar(int(cChar.toLatin1()) - 32);
                                Key_Sim->key_down(cChar.unicode());
                                Key_Sim->key_up(cChar.unicode());
                            }
#elif __unix
                            if (cChar.unicode() < 91)
                            {
                                Key_Sim->key_down(XK_Shift_L);
                                Key_Sim->key_down(cChar.unicode());
                                Key_Sim->key_up(cChar.unicode());
                                Key_Sim->key_up(XK_Shift_L);
                            }
#endif
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

                    Mouse_Sim->mouse_button_click(mouse_button_type_enum(iMouseButtonIndex+1));

                }
            }

            // commands that have no arguments

        }

        if (bCommand != NULL)
            delete bCommand;
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
        switch (iVar)
        {
        case LEAP_HAND_RIGHT:
            sHandMod = "R_";
            break;
        case LEAP_HAND_LEFT:
            sHandMod = "L_";
            break;
        case LEAP_HAND_BOTH_LEFT:
            sHandMod = "BL_";
            break;
        case LEAP_HAND_BOTH_RIGHT:
            sHandMod = "BR_";
        }
        iModifiers = iModifiers | HAND_MOD;
        break;

    }


}

void scriptengine::debug(float x, float y)
{
    Mouse_Sim->mouse_move(x,y);
}

void scriptengine::debugMouseDown()
{
    //    Mouse_Sim->mouse_button_down(xm_left);
}

void scriptengine::debugMouseUp()
{
    //    Mouse_Sim->mouse_button_up(xm_left);
}

void scriptengine::setupLeapMouse(QList<QByteArray> qlLeapBlock)
{
    for(int i = 0; i < qlLeapBlock.size(); i++)
    {
        QString sComp = qlLeapBlock.at(i);

        if(sComp.contains("Track_Finger") ) {
            if (i + 1 < qlLeapBlock.size()) {
            LeapMouseConfig.iTrack_Type = LEAP_MOUSE_TRACK_FINGER;
            LeapMouseConfig.Track_Mods.iFinger = slFingerTypeList.indexOf(qlLeapBlock.at(++i).toLower());
               qWarning() << " Finger ID" << LeapMouseConfig.Track_Mods.iFinger;
            if(LeapMouseConfig.Track_Mods.iFinger == -1){
                qWarning() << "scriptengine::setupLeapMouse: Invalid Finger ID. Defaulting to 'Index' Finger";
                LeapMouseConfig.Track_Mods.iFinger = 1;
            }
            }
            else {
                qWarning() << "scriptengine::setupLeapMouse: Not enough arguments for Track_Finger. Mouse will not track";
            }
                continue;
        }
        if(sComp.contains("Track_Palm") ) {
            if (i + 1 < qlLeapBlock.size()) {
            LeapMouseConfig.iTrack_Type = LEAP_MOUSE_TRACK_PALM;
            QString sHand = qlLeapBlock.at(++i).toLower();
            int iHand = sHand == "left_hand" ? 1 : sHand == "right_hand" ? 2 : -1;
            if (iHand == -1)
            {
                qWarning() << "scriptEngine::setupLeapMouse: Invalid Hand ID. Defaulting to 'Right' Hand";
                LeapMouseConfig.Track_Mods.iHand = iHand;
            }
            else
                LeapMouseConfig.Track_Mods.iHand = slFingerTypeList.indexOf(qlLeapBlock.at(++i).toLower());
            }
            else {
                qWarning() << "scriptengine::setupLeapMouse: Not enough arguments for Track_Palm. Mouse will not track";
            }
            continue;
        }
        if(sComp.contains("Track_Modifier") ) {
            if (i + 1 < qlLeapBlock.size()) {
                QString sMod = qlLeapBlock.at(++i).toLower();
               qWarning() << " mod ID" << sMod;
                if (sMod == "left_hand")
                    LeapMouseConfig.Track_Mods.iHand = LEAP_MOUSE_LEFT;
                else if (sMod == "right_hand")
                    LeapMouseConfig.Track_Mods.iHand = LEAP_MOUSE_RIGHT;
                else if (sMod == "extended")
                    LeapMouseConfig.Track_Mods.bExtended = true;
                else if (sMod == "not_extended")
                    LeapMouseConfig.Track_Mods.bExtended = false;
                else if (sMod == "hand_open")
                    LeapMouseConfig.Track_Mods.bHandOpen = true;
                else if (sMod == "hand_closed")
                    LeapMouseConfig.Track_Mods.bHandOpen = false;
                else if (sMod == "both_hands")
                    LeapMouseConfig.Track_Mods.iHandCount = 2;
                else if (sMod == "one_hand")
                    LeapMouseConfig.Track_Mods.iHandCount = 1;
                else if (sMod == "enable")
                    LeapMouseConfig.Track_Mods.bEnable = true;
                else if (sMod == "disable")
                    LeapMouseConfig.Track_Mods.bEnable = false;
            }
            else {
                qWarning() << "scriptengine::setupLeapMouse: Not enough arguments for Track_Modifier. Mouse will not have the modifier modified";
            }
                continue;
        }
        if(sComp.contains("Leap_Mouse_Width_Range") ) { // Try-catch doesn't work on mingw Qt, how disappointing
            if (i + 2 < qlLeapBlock.size())
            {
                LeapMouseRect[0] = qlLeapBlock.at(++i).toInt();
                LeapMouseRect[1] = qlLeapBlock.at(++i).toInt();
            }
            else
            {
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Width_Range_Min defined with no value. Using -100 as default";
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Width_Range_Max defined with no value. Using 100 as default";
                LeapMouseRect[0] = -100;
                LeapMouseRect[1] = 100;
            }
            continue;
        }
        if(sComp.contains("Leap_Mouse_Height_Range"))
        {
            // Try-catch doesn't work on mingw Qt, how disappointing
            if (i + 2 < qlLeapBlock.size())
            {
                LeapMouseRect[2] = qlLeapBlock.at(++i).toInt();
                LeapMouseRect[3] = qlLeapBlock.at(++i).toInt();
            }
            else
            {
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Height_Min defined with no value. Using 70 as default";
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Height_Max defined with no value. Using 250 as default";
                LeapMouseRect[2] = 70;
                LeapMouseRect[3] = 250;
            }
            continue;
        }
        if(sComp.contains("Leap_Mouse_Type"))
        {
            // Try-catch doesn't work on mingw Qt, how disappointing
            if (i + 1 < qlLeapBlock.size())
            {
                switch(slMouseTypeList.indexOf(qlLeapBlock.at(++i).toLower()))
                {
                case 0:
                    Mouse_Sim->mouse_type =  mouse_type_enum::xm_type_absolute;
                    break;
                case 1:
                    Mouse_Sim->mouse_type =  mouse_type_enum::xm_type_relative;
                    break;
                case 2:
                    Mouse_Sim->mouse_type =  mouse_type_enum::xm_type_absolute_to_relative;
                    break;
                }
            }
            else
            {
                qWarning() << "scriptengine::setDefinitions: Leap_Mouse_Type Undefined. Using Absolute as default";
                Mouse_Sim->mouse_type = mouse_type_enum::xm_type_absolute;
            }
            continue;
        }
    }

}

void scriptengine::scriptFileChanged(QString sFileName)
{
    qDebug() << "scriptengine::scriptFileChanged:"<<sFileName;
    if(sFileName.contains("macros"))
      setScriptFile(sScriptPath + QString("/macros.ina"));
    else if (sFileName.contains("defines"))
        setScriptFile(sScriptPath + QString("/defines.ina"));
    else
      qWarning() << "scriptengine:scriptFileChanged: No Pre-determined Update Path For Script File";
}

