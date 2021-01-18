#ifndef AUDIOPROGRESSBARDIALOG_H
#define AUDIOPROGRESSBARDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QScreen>
#include <QTimer>
#include <QProcess>

#include "Settings.h"

namespace Ui
{
class AudioProgressBarDialog;
}

class AudioProgressBarDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AudioProgressBarDialog(QWidget *parent = nullptr);
    ~AudioProgressBarDialog();

public slots:
    void setRelativeAudioLevel(int difference);
    void setAudioLevel(int value, int max = 100);
    void toggleMute();
    void updateSink();
private slots:
    void hideDialog();
    void getVolumeLevel();
private:
    Ui::AudioProgressBarDialog *ui;
    int _desktopHeight;
    int _desktopWidth;
    QTimer * _hiddenTimer;
    QTimer * _getAudioLevelTimer;
    bool _muteBlock = false;
    QString _sink;
};

#endif // AUDIOPROGRESSBARDIALOG_H
