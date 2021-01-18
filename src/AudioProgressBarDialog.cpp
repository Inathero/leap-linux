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

//    _getAudioLevelTimer = new QTimer(this);
//    connect(_getAudioLevelTimer, &QTimer::timeout, this, &AudioProgressBarDialog::getVolumeLevel);
    updateSink();
    getVolumeLevel();
//    _getAudioLevelTimer->start(30000);
}

AudioProgressBarDialog::~AudioProgressBarDialog()
{
    delete ui;
}

void AudioProgressBarDialog::setRelativeAudioLevel(int difference)
{
    if(!this->isVisible())
    {
        getVolumeLevel();
        this->show();
        this->setGeometry(_desktopWidth - this->width(),
                          _desktopHeight - this->height(),
                          this->width(),
                          this->height());
    }

    int newValue = ui->progressBar->value() + difference;
    if(newValue > 150 || newValue < 0) return;

    ui->progressBar->setValue(newValue);

    ui->label->setText(QString::number(newValue));
    _hiddenTimer->start();
    QProcess *p = new QProcess;
    connect(p,static_cast<void (QProcess::*)(int)>(&QProcess::finished), p, &QProcess::deleteLater);
    p->start("pactl", QStringList() << "set-sink-volume" << _sink << (difference > 0 ? "+" : "") + QString::number(difference) + "%");
}

void AudioProgressBarDialog::setAudioLevel(int value, int max)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

void AudioProgressBarDialog::toggleMute()
{
//    if(!_muteBlock)
    {
//        _muteBlock = true;
        QProcess *p = new QProcess;
        connect(p,static_cast<void (QProcess::*)(int)>(&QProcess::finished), p, [=]()
        {
            p->deleteLater();
//            QTimer::singleShot(1500, this, [=]()
//            {
//                _muteBlock = false;
//            });
        });
        p->start("pactl", QStringList() << "set-sink-mute" << _sink << "toggle");
    }

}

void AudioProgressBarDialog::updateSink()
{
    _sink = Settings::loadSetting("sink").toString();
    qDebug() << this << "Sink - " << _sink;
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
    p->start("pamixer", QStringList() << "--sink" << _sink << "--get-volume");
}
