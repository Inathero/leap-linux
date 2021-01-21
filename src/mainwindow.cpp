#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _leapPoller = new QTimer;
    _Logic = new Logic;
    _LeapListener = new LeapListener;
    _AudioSinkDialog = new AudioSinkDialog;

    connect(_LeapListener, &LeapListener::startPolling, this, &MainWindow::Leap_StartPolling);
    connect(_LeapListener, &LeapListener::stopPolling, this, &MainWindow::Leap_StopPolling);
    connect(_LeapListener, &LeapListener::onPollHands, _Logic, &Logic::processLeapHands);
    connect(_LeapListener, &LeapListener::onPollGestures, _Logic, &Logic::processLeapGestures);

    connect(_leapPoller, &QTimer::timeout, _LeapListener, &LeapListener::requestPoll);

    _AudioSinkDialog->setSink(Settings::loadSetting("sink").toString());
    connect(_AudioSinkDialog, &AudioSinkDialog::newAudioSink, this, [=](QString sink)
    {
        Settings::saveSetting("sink", sink);
        _Logic->getAudioDialog()->updateSink();
    });
    createTray();
}

MainWindow::~MainWindow()
{
    delete _trayMenu;
    delete _tray;
    delete _leapPoller;
    delete _Logic;
    delete _LeapListener;
    delete _AudioSinkDialog;

    delete ui;
}

void MainWindow::Leap_StartPolling()
{
    _leapPoller->start(1);
}

void MainWindow::Leap_StopPolling()
{
    _leapPoller->stop();
}

void MainWindow::trayAction(QAction *tAction)
{
    if(tAction->text() == "Exit")
        exit(0);
    else if (tAction->text() == "Set Audio Sink")
    {
        _AudioSinkDialog->show();
    }
    else if (tAction->text() == "Reload Commands")
    {
        _Logic->reloadCommandsFromTray();
    }
}

void MainWindow::createTray()
{
    _trayMenu = new QMenu;
    _trayMenu->addAction("Set Audio Sink");
    _trayMenu->addAction("Reload Commands");
    _trayMenu->addAction("Exit");
    connect(_trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(trayAction(QAction *)));
    _tray = new QSystemTrayIcon(QIcon(":/icons/discord_ok.png"));
    _tray->setToolTip("Leap for Linux");
    _tray->setContextMenu(_trayMenu);

    _tray->show();

}
