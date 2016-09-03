#include "xkeys.h"

xkeys::xkeys(QObject *parent) : QObject(parent)
{

}

void xkeys::key_down(int iKeyCode, int iModifiers)
{
    key_setup();

    // Send a fake key press event to the window.
    XKeyEvent event = createKeyEvent(true, iKeyCode, iModifiers);
    XSendEvent(event.display, event.window, True, KeyPressMask | iModifiers, (XEvent *)&event);

    XCloseDisplay(xDisplay);
}

void xkeys::key_up(int iKeyCode, int iModifiers)
{
    key_setup();

    // Send a fake key release event to the window.
    XKeyEvent event = createKeyEvent(false, iKeyCode, iModifiers);
    XSendEvent(event.display, event.window, True, KeyPressMask | iModifiers, (XEvent *)&event);

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

XKeyEvent xkeys::createKeyEvent( bool press, int keycode, int modifiers)
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
       event.state       =  modifiers;

       if(press)
          event.type = KeyPress;
       else
          event.type = KeyRelease;

       return event;
}
