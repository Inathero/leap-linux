#include "macro.h"

macro::macro(QObject *parent) : QObject(parent)
{
    MacroDelay = new QTimer(this);
    connect(MacroDelay, &QTimer::timeout, this, &macro::macroRelease);
    MacroDelay->setInterval(500);
}

bool macro::isMacroAvailable()
{
    return !MacroDelay->isActive();
}

void macro::macroLock()
{
    MacroDelay->start();
}

void macro::macroRelease()
{
    MacroDelay->stop();
}
