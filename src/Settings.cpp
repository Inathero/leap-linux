#include "Settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{

}

void Settings::saveSetting(QString name, QVariant value, QString organization, QString application)
{
    QSettings settings(organization, application);
    settings.beginGroup("Settings");
    settings.setValue(name, value);
    settings.endGroup();
}

QVariant Settings::loadSetting(QString name, QString organization, QString application)
{
    QSettings settings(organization, application);
    settings.beginGroup("Settings");
    QVariant value = settings.value(name);
    settings.endGroup();
    return value;
}
