#ifndef MACRO_H
#define MACRO_H

#include <QObject>
#include <QTimer>
#include <QDebug>

// Currently just used for timing
// In future, need to implement xkeys in it

class macro : public QObject
{
    Q_OBJECT
public:
    explicit macro(QObject *parent = 0);

signals:

public slots:
    bool isMacroAvailable();
    void macroLock();
private slots:
    void macroRelease();
private:
    QTimer * MacroDelay;
};

#endif // MACRO_H
