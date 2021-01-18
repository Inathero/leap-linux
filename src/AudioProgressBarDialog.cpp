#include "AudioProgressBarDialog.h"
#include "ui_AudioProgressBarDialog.h"


AudioProgressBarDialog::AudioProgressBarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioProgressBarDialog)
{
    ui->setupUi(this);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    _desktopHeight = screenGeometry.height();
    _desktopWidth = screenGeometry.width();

    _hiddenTimer = new QTimer(this);
    connect(_hiddenTimer, &QTimer::timeout, this, &AudioProgressBarDialog::hideDialog);
    _hiddenTimer->setInterval(2000);
}

AudioProgressBarDialog::~AudioProgressBarDialog()
{
    delete ui;
}

void AudioProgressBarDialog::addRelativeAudioLevel(int difference)
{
    ui->progressBar->setValue(ui->progressBar->value() + difference);

    if(!this->isVisible())
    {
        this->show();
        this->setGeometry(_desktopWidth - this->width(),
                          _desktopHeight - this->height(),
                          this->width(),
                          this->height());
    }
    _hiddenTimer->start();
}

void AudioProgressBarDialog::setAudioLevel(int value, int max)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

void AudioProgressBarDialog::hideDialog()
{
    this->hide();
}
