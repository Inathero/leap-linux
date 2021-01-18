#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Leap_Poller = new QTimer;
    Listener = new listener();

    myMenu = new QMenu;
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
}
