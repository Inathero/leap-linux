#include "AudioSinkDialog.h"
#include "ui_AudioSinkDialog.h"

AudioSinkDialog::AudioSinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioSinkDialog)
{
    ui->setupUi(this);
}

AudioSinkDialog::~AudioSinkDialog()
{
    delete ui;
}

void AudioSinkDialog::on_buttonBox_accepted()
{
    emit newAudioSink(ui->lineEdit->text());
    this->hide();
}

void AudioSinkDialog::on_buttonBox_rejected()
{
    this->hide();
}
