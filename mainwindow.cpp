#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Leap_Poller = new QTimer;
    Listener = new listener;

    connect(Listener, &listener::StartPolling, this, &MainWindow::Leap_StartPolling);
    connect(Leap_Poller, &QTimer::timeout, Listener, &listener::onPoll);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Leap_StartPolling()
{
    Leap_Poller->start(1);
}
