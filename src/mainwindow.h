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
#include <QSystemTrayIcon>
#include <QMenu>


#include "Listener.h"
#include "Leap.h"
#include "Logic.h"
#include "AudioSinkDialog.h"

using namespace Leap;

namespace Ui
{
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
    void Leap_StopPolling();

    void trayAction(QAction * tAction);
private:
    void createTray();
    Ui::MainWindow *ui;
    LeapListener * _LeapListener;
    Logic * _Logic;
    QTimer * _leapPoller;
    QMenu * _trayMenu;
    QSystemTrayIcon * _tray;
    AudioSinkDialog * _AudioSinkDialog;

};

#endif // MAINWINDOW_H
