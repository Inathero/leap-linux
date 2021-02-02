#include "Commands.h"
#include "Debug.h"

Commands::Commands(QObject *parent) : QObject(parent)
{
    loadCommands();
}

void Commands::runCommand(QString command)
{
    if(command.contains("swipe"))
    {
        if(_bAllowGestures)
            _bAllowGestures = false;
        else return;
    }

    QProcess *p = new QProcess;
    connect(p,static_cast<void (QProcess::*)(int)>(&QProcess::finished), p, &QProcess::deleteLater);

    for(const auto &s : qAsConst(_CommandList))
    {
        if(s.commandName == command)
        {
            QStringList copy = s.commandProcess;
            db "Command - " << command;
            if(copy.size() > 0)
            {
                QString process = copy.takeFirst();
                p->start(process, copy);
            }
            else
                db "Warning - Command is empty";
        }
    }
    if(p->state() == QProcess::NotRunning)
        p->deleteLater();
}

void Commands::reloadCommands()
{
    _CommandList.clear();
    loadCommands();
}

void Commands::loadCommands()
{
    QFile file(QCoreApplication::applicationDirPath().append("/commands.txt"));
    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QStringList coms = in.readLine().split(' ');
            CommandStruct s;
            s.commandName = coms.takeFirst();
            s.commandProcess = coms;
            _CommandList.append(s);
        }
    }
    else
        db "WARNING - commands.txt not FOUND";
}
