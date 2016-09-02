#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Leap_Poller = new QTimer(this);
    Listener = new listener;

    connect(Leap_Poller, &QTimer::timeout, Listener, &listener::onPoll);

    Leap_Poller->start(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
