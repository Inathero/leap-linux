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
    MacroDelay->start(iInterval);
}

void macro::macroRelease()
{
    MacroDelay->stop();
}
