#ifndef AUDIOSINKDIALOG_H
#define AUDIOSINKDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QRegularExpression>
#include "Debug.h"

namespace Ui
{
class AudioSinkDialog;
}

class AudioSinkDialog : public QDialog
{
    Q_OBJECT

signals:
    void newAudioSink(QString);
public:
    explicit AudioSinkDialog(QWidget *parent = nullptr);
    ~AudioSinkDialog();

    void setSink(QString s = QString());

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AudioSinkDialog *ui;
    QString _lastSink;
};

#endif // AUDIOSINKDIALOG_H
