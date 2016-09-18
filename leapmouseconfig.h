#ifndef LEAPMOUSECONFIG_H
#define LEAPMOUSECONFIG_H

#define LEAP_MOUSE_TRACK_FINGER 1
#define LEAP_MOUSE_TRACK_PALM 	2
#define LEAP_MOUSE_LEFT 		1
#define LEAP_MOUSE_RIGHT 		2

struct stLeapMouseConfig
{
    int iTrack_Type = LEAP_MOUSE_TRACK_FINGER;
    struct stLeapMouseTrackMods
    {
        int iFinger = 1;
        bool bExtended = true;
        bool bHandOpen = true;
        int iHandCount = 1;
        bool bEnable = false;
        int iHand = LEAP_MOUSE_RIGHT;
    } Track_Mods;
};


#endif // LEAPMOUSECONFIG_H
