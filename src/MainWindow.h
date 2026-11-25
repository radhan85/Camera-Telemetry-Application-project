#pragma once

#include "CameraController.h"
#include "TelemetryGenerator.h"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QResizeEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    QLabel *feedLabel_ = nullptr;
    QLabel *temperatureLabel_ = nullptr;
    QLabel *speedLabel_ = nullptr;
    QLabel *latitudeLabel_ = nullptr;
    QLabel *longitudeLabel_ = nullptr;
    QLabel *rpmLabel_ = nullptr;
    QLabel *lastUpdateLabel_ = nullptr;
    QLabel *statusLabel_ = nullptr;
    QLabel *resolutionLabel_ = nullptr;
    QComboBox *resolutionCombo_ = nullptr;
    QPushButton *playButton_ = nullptr;
    QPushButton *stopButton_ = nullptr;
    QPushButton *pauseButton_ = nullptr;
    QPushButton *resumeButton_ = nullptr;
    QPushButton *forwardButton_ = nullptr;
    QPushButton *rewindButton_ = nullptr;
    CameraController camera_;
    TelemetryGenerator telemetry_;
    QPixmap currentFrame_;

    void onFrame(const QImage &frame);
    void onTelemetry(const TelemetryData &data);
    void resizeEvent(QResizeEvent *event) override;
};
