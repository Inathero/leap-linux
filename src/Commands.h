#ifndef COMMANDS_H
#define COMMANDS_H

#include <QCoreApplication>
#include <QObject>
#include <QProcess>
#include <QFile>
#include <QTextStream>

class Commands : public QObject
{
    Q_OBJECT

    struct CommandStruct
    {
        QString commandName;
        QStringList commandProcess;
    };

public:
    explicit Commands(QObject *parent = nullptr);

    void runCommand(QString command);
    void reloadCommands();
    void unlockGestures()
    {
        _bAllowGestures = true;
    }
signals:
private:
    void loadCommands();
    QList<CommandStruct> _CommandList;
    bool _bAllowGestures = true;
};

#endif // COMMANDS_H
