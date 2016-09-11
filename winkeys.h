#ifndef WINKEYS_H
#define WINKEYS_H

#include <QObject>

class winkeys : public QObject
{
    Q_OBJECT
public:
    explicit winkeys(QObject *parent = 0);
signals:

public slots:
    void key_down(int iKeyCode);
    void key_up(int iKeyCode);
private slots:
    void key_setup();

private:
};

#endif // WINKEYS_H
