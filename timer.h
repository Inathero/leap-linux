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
    bool bEnabled;
};

struct queue_struct
{
    macro_block_struct & macro_block;
    int iMilliseconds;
    int iStart;
    queue_struct(macro_block_struct &a1) : macro_block(a1) {}
};


class timer : public QObject
{
    Q_OBJECT
public:
    explicit timer(QObject *parent = 0);

signals:

public slots:
    void AddToQueue(macro_block_struct & macro_block, int iMilliseconds);

private slots:
    void ThreadLoop();



private:
    QTime * Time;
//    QList<QList<QList<bool&>, int>, int> qlMaster;
    QList<queue_struct> qlMaster;
//    QList<bool, int, int> qlMaster;
    QThread Thread;
};

#endif // TIMER_H
