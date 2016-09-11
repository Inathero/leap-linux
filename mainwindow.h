#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if __unix
#include <unistd.h>
#endif
#include <QMainWindow>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QMetaType>
#include <QTimer>

#include "listener.h"
#include "Leap.h"

using namespace Leap;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

    void Leap_StartPolling();

private:
    Ui::MainWindow *ui;
    listener * Listener;
    QTimer * Leap_Poller;
};

#endif // MAINWINDOW_H
