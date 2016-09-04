#ifndef XKEYS_H
#define XKEYS_H

// Simulate Key Press in Linux
// Adapated from Adam Pierce - http://www.doctort.org/adam/

#include <QObject>
#include <QDebug>

#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

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
    XKeyEvent createKeyEvent(bool press, int keycode);

signals:

public slots:
    void key_down(int iKeyCode);
    void key_up(int iKeyCode);
private slots:
    void key_setup();

private:
    Display * xDisplay ;
    Window xWinRoot;
    Window xWinFocus;
    int    xRevert;
};

#endif // XKEYS_H
