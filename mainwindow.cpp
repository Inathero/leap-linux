#include "mainwindow.h"
#include "ui_mainwindow.h"


bool killDetectionLoop = false;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Leap runs on a static interface
    // Stick it in a thread to prevent premature end
    QtConcurrent::run(this, &MainWindow::Leap_ControllerThread);
}

void MainWindow::Leap_ControllerThread()
{
    // Create controller and link to listener
    Controller controller;



    controller.addListener(Listener);
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

    // Basic loop to prevent death
    while (!killDetectionLoop)
     {
       sleep(5);
       if (!controller.isPolicySet(Leap::Controller::POLICY_BACKGROUND_FRAMES))
         controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}
