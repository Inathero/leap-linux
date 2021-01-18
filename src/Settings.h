#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    static void saveSetting(QString  name, QVariant  value, QString organization = "JohnCiubucApps", QString application = "LeapLinux");
    static QVariant loadSetting(QString name, QString organization = "JohnCiubucApps", QString application = "LeapLinux");
signals:

};

#endif // SETTINGS_H
