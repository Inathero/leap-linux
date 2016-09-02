#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <QMainWindow>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

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
    void Leap_ControllerThread();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    listener Listener;
};

#endif // MAINWINDOW_H
