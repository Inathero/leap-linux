#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QHash>
#include <QThread>
#include <QPair>
#include <QList>

struct macro_block_struct
{
    bool bEnabled = true;
};

struct queue_struct
{
    macro_block_struct & macro_block;
    int iMilliseconds;
    int iTimeStamp;
    queue_struct(macro_block_struct &a1) : macro_block(a1) {}
};


class timer : public QObject
{
    Q_OBJECT
public:
    explicit timer(QObject *parent = 0);

public slots:
    void AddToQueue(macro_block_struct & macro_block, int iMilliseconds);

private slots:
    void ThreadLoop();

private:
    QTime * Time;
    QList<queue_struct> qlMaster;
    QThread Thread;
    bool bMasterControl;
};

#endif // TIMER_H
