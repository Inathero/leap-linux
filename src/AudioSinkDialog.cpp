#include "AudioSinkDialog.h"
#include "ui_AudioSinkDialog.h"

AudioSinkDialog::AudioSinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioSinkDialog)
{
    ui->setupUi(this);
    ui->lineEdit->hide();
}

AudioSinkDialog::~AudioSinkDialog()
{
    delete ui;
}

void AudioSinkDialog::setSink(QString s)
{
    if(!s.isEmpty())
        _lastSink = s;
    QProcess * p = new QProcess;
    connect(p,&QProcess::readyReadStandardOutput, p, [=]()
    {
        QString out =  p->readAllStandardOutput();
        QRegularExpression exp("name: <(.+)>");
        QRegularExpressionMatchIterator it = exp.globalMatch(out);
        while(it.hasNext())
        {
            auto reg = it.next();
            if(reg.captured(1) == _lastSink)
                ui->comboBox->insertItem(0, _lastSink);
            else
                ui->comboBox->addItem(reg.captured(1));
        }
        ui->comboBox->setCurrentIndex(0);
    });

    p->start("pacmd", QStringList() << "list-sinks");
//    ui->lineEdit->setText(s);
}

void AudioSinkDialog::on_buttonBox_accepted()
{
//    emit newAudioSink(ui->lineEdit->text());
    emit newAudioSink(ui->comboBox->currentText());
    this->hide();
}

void AudioSinkDialog::on_buttonBox_rejected()
{
    this->hide();
}
