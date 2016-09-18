#include "macro.h"

macro::macro(QObject *parent) : QObject(parent)
{
    MacroDelay = new QTimer(this);
    connect(MacroDelay, &QTimer::timeout, this, &macro::macroRelease);
}

bool macro::isMacroAvailable()
{
    return !MacroDelay->isActive();
}

void macro::macroLock( int iInterval )
{
    if (iInterval > 0)
    {
        MacroDelay->start(iInterval);
        qDebug() << "macro::macroLock:"<<iInterval;
    }
}

void macro::macroRelease()
{
    qDebug() << "macro::macroRelease";
    MacroDelay->stop();
}
