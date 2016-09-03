#ifndef XKEYS_H
#define XKEYS_H

// Simulate Key Press in Linux
// Adapated from Adam Pierce - http://www.doctort.org/adam/

#include <QObject>
#include <QDebug>

#include <QX11Info>
#include <X11/Xlib.h>

#undef Bool
#ifdef None
#undef None
#endif
#ifdef FrameFeature
#undef FrameFeature
#endif
#undef None

#include <X11/keysym.h>
class xkeys : public QObject
{
    Q_OBJECT
public:
    explicit xkeys(QObject *parent = 0);
    XKeyEvent createKeyEvent(bool press, int keycode, int modifiers);

signals:

public slots:
    void key_down( int iKeyCode, int iModifiers = 0);
    void key_up( int iKeyCode, int iModifiers = 0);
private slots:
    void key_setup();

private:
    Display * xDisplay ;
    Window xWinRoot;
    Window xWinFocus;
    int    xRevert;
};

#endif // XKEYS_H
