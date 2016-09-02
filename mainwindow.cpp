#include "mainwindow.h"
#include "ui_mainwindow.h"


bool killDetectionLoop = false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QtConcurrent::run(this, &MainWindow::Leap_ControllerThread);


}

void MainWindow::Leap_ControllerThread()
{
    Controller controller;

    controller.addListener(Listener);

    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
    while (!killDetectionLoop)
     {
       sleep(1);
       if (!controller.isPolicySet(Leap::Controller::POLICY_BACKGROUND_FRAMES))
         controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}
