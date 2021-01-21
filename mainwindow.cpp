#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Leap_Poller = new QTimer;
    Listener = new listener();
    _AudioSinkDialog = new AudioSinkDialog;
    _AudioSinkDialog->setSink(Settings::loadSetting("sink").toString());
    connect(_AudioSinkDialog, &AudioSinkDialog::newAudioSink, this, [=](QString sink)
    {
        Settings::saveSetting("sink", sink);
        Listener->Logic->getAudioDialog()->updateSink();
    });

    myMenu = new QMenu;
    myMenu->addAction("Set Audio Sink");
    myMenu->addAction("Exit");
    connect(myMenu, SIGNAL(triggered(QAction*)), this, SLOT(trayAction(QAction *)));
    sTray = new QSystemTrayIcon(QIcon(":/icons/discord_ok.png"));
    sTray->setToolTip("Leap for Linux");
    sTray->setContextMenu(myMenu);

    sTray->show();


    connect(Listener, &listener::StartPolling, this, &MainWindow::Leap_StartPolling);
    connect(Listener, &listener::StopPolling, this, &MainWindow::Leap_StopPolling);
    connect(Leap_Poller, &QTimer::timeout, Listener, &listener::onPoll);
    QTimer::singleShot(1, this, &MainWindow::hide);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Leap_StartPolling()
{
    Leap_Poller->start(1);
}

void MainWindow::Leap_StopPolling()
{
    Leap_Poller->stop();
}

void MainWindow::trayAction(QAction *tAction)
{
    if(tAction->text() == "Exit")
        exit(0);
    else if (tAction->text() == "Set Audio Sink")
    {
        _AudioSinkDialog->show();
    }
}
