#include "winkeys.h"

winkeys::winkeys(QObject *parent) : QObject(parent)
{

}

void winkeys::key_down(int iKeyCode)
{
    INPUT Input;
    // Set up a generic keyboard event.
    Input.type = INPUT_KEYBOARD;
    Input.ki.wScan = 0; // hardware scan code for key
    Input.ki.time = 0;
    Input.ki.dwExtraInfo = 0;
    Input.ki.dwFlags = KEYEVENTF_KEYDOWN;

    Input.ki.wVk = iKeyCode;
    SendInput(1, &Input, sizeof(INPUT));
}

void winkeys::key_up(int iKeyCode)
{
    INPUT Input;
    // Set up a generic keyboard event.
    Input.type = INPUT_KEYBOARD;
    Input.ki.wScan = 0; // hardware scan code for key
    Input.ki.time = 0;
    Input.ki.dwExtraInfo = 0;
    Input.ki.dwFlags = KEYEVENTF_KEYUP; 
    Input.ki.wVk = iKeyCode;
    SendInput(1, &Input, sizeof(INPUT));
}
