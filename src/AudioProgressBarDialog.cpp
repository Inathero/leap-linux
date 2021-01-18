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

    _getAudioLevelTimer = new QTimer(this);
    connect(_getAudioLevelTimer, &QTimer::timeout, this, &AudioProgressBarDialog::getVolumeLevel);
    getVolumeLevel();
    _getAudioLevelTimer->start(15000);
}

AudioProgressBarDialog::~AudioProgressBarDialog()
{
    delete ui;
}

void AudioProgressBarDialog::addRelativeAudioLevel(int difference)
{
    ui->progressBar->setValue(ui->progressBar->value() + difference);

    ui->label->setText(QString::number(ui->progressBar->value()));
    if(!this->isVisible())
    {
        this->show();
        this->setGeometry(_desktopWidth - this->width(),
                          _desktopHeight - this->height(),
                          this->width(),
                          this->height());
    }
    _hiddenTimer->start();
//    qDebug() << QString::number(difference) + "%";
    QProcess *p = new QProcess;
    p->start("pactl", QStringList() << "set-sink-volume" << "jack_out" << (difference > 0 ? "+" : "") + QString::number(difference) + "%");
    connect(p,static_cast<void (QProcess::*)(int)>(&QProcess::finished), p, &QProcess::deleteLater);
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

void AudioProgressBarDialog::getVolumeLevel()
{
    QProcess * p = new QProcess;
    connect(p, &QProcess::readyReadStandardOutput, this, [=]()
    {
        setAudioLevel(p->readAllStandardOutput().simplified().toInt(), 150);
        p->deleteLater();
    });
    p->start("pamixer", QStringList() << "--get-volume");
}
