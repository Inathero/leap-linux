#include "timer.h"

timer::timer(QObject *parent) : QObject(parent)
{
    Time = new QTime;
    Time->start();
    moveToThread(&Thread);
}

void timer::AddToQueue(macro_block_struct &macro_block, int iMilliseconds)
{
    queue_struct qStruct(macro_block);
//    qStruct.bReference = macro_block;
    qStruct.iMilliseconds = iMilliseconds;
    qStruct.iStart = Time->elapsed();

    if(qlMaster.isEmpty())
    {
        qlMaster.append(qStruct);
//        Thread.start();
    }
    else
        qlMaster.append(qStruct);
//        qStruct.bReference = true;
//    qlMaster.first().bReference = true;

    qStruct.macro_block.bEnabled = true;

//    bool b = &(qlMaster.first().first.first);
//    b = true;
//    = true;

//bBlocker = true;
//    qDebug() << "in thread: " << qlMaster.first().macro_block.bEnabled <<"," << qStruct;
}

void timer::ThreadLoop()
{

}
