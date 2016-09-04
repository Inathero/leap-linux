#include "xkeys.h"

xkeys::xkeys(QObject *parent) : QObject(parent)
{

}

void xkeys::key_down(int iKeyCode)
{
    key_setup();

    // Send a fake key press event to the window.
    XKeyEvent event = createKeyEvent(true, iKeyCode);
    XTestFakeKeyEvent(event.display, event.keycode , True, 0);

    XCloseDisplay(xDisplay);
}

void xkeys::key_up(int iKeyCode)
{
    key_setup();

    // Send a fake key release event to the window.
    XKeyEvent event = createKeyEvent(false, iKeyCode);
    XTestFakeKeyEvent(event.display, event.keycode , False, 0);

    XCloseDisplay(xDisplay);
}

void xkeys::key_setup()
{
    xDisplay = XOpenDisplay(0);
    if(xDisplay == NULL)
        qWarning() << "Unable to open XxDisplay";

    // Get the root window for the current xDisplay.
    xWinRoot = XDefaultRootWindow(xDisplay);

    // Find the window which has the current keyboard focus.
    XGetInputFocus(xDisplay, &xWinFocus, &xRevert);
}

XKeyEvent xkeys::createKeyEvent( bool press, int keycode)
{
       XKeyEvent event;

       event.display     = xDisplay;
       event.window      = xWinFocus;
       event.root        = xWinRoot;
       event.subwindow   = 0;
       event.time        = CurrentTime;
       event.x           = 1;
       event.y           = 1;
       event.x_root      = 1;
       event.y_root      = 1;
       event.same_screen = True;
       event.keycode     = XKeysymToKeycode(xDisplay, keycode);
       event.state       = 0;

       if(press)
          event.type = KeyPress;
       else
          event.type = KeyRelease;

       return event;
}
