#include "timer.h"

timer::timer(QObject *parent) : QObject(parent)
{
    Time = new QTime;
    Time->start();
    bMasterControl = true;
    connect(&Thread, SIGNAL(started()), this, SLOT(ThreadLoop()));
//    connect(this, SIGNAL(emptyQueue()), this, SLOT(stopThread()));
    moveToThread(&Thread);
    Thread.start();
}

void timer::AddToQueue(macro_block_struct &macro_block, int iMilliseconds)
{
    qDebug() << "timer::AddToQueue:"<<iMilliseconds;
    macro_block.bEnabled = false;
    queue_struct qStruct(macro_block);
    qStruct.iMilliseconds = iMilliseconds;
    qStruct.iTimeStamp = Time->elapsed();

//    if(qlMaster.isEmpty())
//    {
//        qlMaster.append(qStruct);
//    }
//    else
        qlMaster.append(qStruct);
}

void timer::ThreadLoop()
{
    while(bMasterControl)
    {
        QThread::msleep(100);
        while(!qlMaster.isEmpty())
        {
            QThread::msleep(1);

            int iTimeStamp = Time->elapsed();

            for (int i = 0; i < qlMaster.size(); i++)
            {
                if(iTimeStamp - qlMaster.at(i).iTimeStamp >= qlMaster.at(i).iMilliseconds)
                {
                    qDebug() << "timer::ThreadLoopClear";
                    qlMaster.at(i).macro_block.bEnabled = true;
                    qlMaster.removeAt(i);
                    i--;
                }
            }
        }
    }

}
