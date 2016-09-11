#ifndef WINKEYS_H
#define WINKEYS_H

#include <qt_windows.h>
#include <QObject>
#include <QDebug>

class winkeys : public QObject
{
    Q_OBJECT
public:
    explicit winkeys(QObject *parent = 0);
signals:

public slots:
    void key_down(int iKeyCode);
    void key_up(int iKeyCode);

private:
};

#endif // WINKEYS_H
