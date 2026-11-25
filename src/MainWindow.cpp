#include "MainWindow.h"

#include <QVariant>
#include <QDateTime>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QGridLayout>

namespace {
void styleValue(QLabel *label)
{
    label->setMinimumWidth(110);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
}

MainWindow::MainWindow()
{
    setWindowTitle("Camera Telemetry");
    resize(1180, 720);

    auto *central = new QWidget(this);
    auto *splitter = new QSplitter(Qt::Horizontal, central);

    auto *telemetryPanel = new QGroupBox("Telemetry", splitter);
    telemetryPanel->setMinimumWidth(220);
    auto *telemetryLayout = new QVBoxLayout(telemetryPanel);

    auto *grid = new QWidget(telemetryPanel);
    auto *gridLayout = new QGridLayout(grid);
    gridLayout->setContentsMargins(8, 8, 8, 8);
    gridLayout->setSpacing(12);

    temperatureLabel_ = new QLabel("42.3 °C");
    speedLabel_ = new QLabel("67 km/h");
    latitudeLabel_ = new QLabel("12.97160");
    longitudeLabel_ = new QLabel("77.59460");
    rpmLabel_ = new QLabel("3450 rpm");

    const QString names[] = {"Temperature", "Speed", "GPS Latitude", "GPS Longitude", "Motor RPM"};
    QLabel *values[] = {temperatureLabel_, speedLabel_, latitudeLabel_, longitudeLabel_, rpmLabel_};
    for (int i = 0; i < 5; ++i) {
        auto *name = new QLabel(names[i]);
        gridLayout->addWidget(name, i, 0);
        styleValue(values[i]);
        gridLayout->addWidget(values[i], i, 1);
    }
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    telemetryLayout->addWidget(grid);
    telemetryLayout->addStretch();

    lastUpdateLabel_ = new QLabel("Last update: --");
    telemetryLayout->addWidget(lastUpdateLabel_);

    auto *cameraPanel = new QWidget(splitter);
    auto *cameraLayout = new QVBoxLayout(cameraPanel);
    cameraLayout->setContentsMargins(8, 8, 8, 8);

    feedLabel_ = new QLabel("Camera feed stopped");
    feedLabel_->setMinimumSize(640, 360);
    feedLabel_->setAlignment(Qt::AlignCenter);
    feedLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    feedLabel_->setStyleSheet("border: 1px solid #aab4c0; background: #f7f9fb;");
    cameraLayout->addWidget(feedLabel_, 1);

    statusLabel_ = new QLabel("Status: Stopped");
    cameraLayout->addWidget(statusLabel_);

    auto *controls = new QHBoxLayout();
    playButton_ = new QPushButton("Play");
    stopButton_ = new QPushButton("Stop");
    pauseButton_ = new QPushButton("Pause");
    resumeButton_ = new QPushButton("Resume");
    rewindButton_ = new QPushButton("Rewind");
    forwardButton_ = new QPushButton("Forward");
    controls->addWidget(playButton_);
    controls->addWidget(stopButton_);
    controls->addWidget(pauseButton_);
    controls->addWidget(resumeButton_);
    controls->addWidget(rewindButton_);
    controls->addWidget(forwardButton_);
    cameraLayout->addLayout(controls);

    auto *settings = new QGroupBox("Camera Feed Settings");
    auto *settingsLayout = new QHBoxLayout(settings);
    resolutionCombo_ = new QComboBox();
    resolutionCombo_->addItem("640 x 480");
    resolutionCombo_->addItem("1280 x 720");
    resolutionCombo_->addItem("1920 x 1080");
    resolutionCombo_->setCurrentIndex(1);
    resolutionLabel_ = new QLabel("Current resolution: 1280 x 720");
    settingsLayout->addWidget(resolutionCombo_);
    settingsLayout->addWidget(resolutionLabel_);
    settingsLayout->addStretch();
    cameraLayout->addWidget(settings);

    splitter->addWidget(telemetryPanel);
    splitter->addWidget(cameraPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    auto *centralLayout = new QHBoxLayout(central);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->addWidget(splitter);
    setCentralWidget(central);
    statusBar()->showMessage("Ready");

    connect(playButton_, &QPushButton::clicked, &camera_, &CameraController::play);
    connect(stopButton_, &QPushButton::clicked, &camera_, &CameraController::stop);
    connect(pauseButton_, &QPushButton::clicked, &camera_, &CameraController::pause);
    connect(resumeButton_, &QPushButton::clicked, &camera_, &CameraController::resume);
    connect(forwardButton_, &QPushButton::clicked, this, [this]() { camera_.forward(5); });
    connect(rewindButton_, &QPushButton::clicked, this, [this]() { camera_.rewind(5); });
    connect(resolutionCombo_, &QComboBox::currentIndexChanged, this, [this]() {
        const QString text = resolutionCombo_->currentText();
        const auto parts = text.split('x');
        if (parts.size() == 2) {
            camera_.setResolution(parts[0].trimmed().toInt(), parts[1].trimmed().toInt());
        } else {
            camera_.setResolution(-1, -1);
        }
    });

    connect(&camera_, &CameraController::frameReady, this, &MainWindow::onFrame);
    connect(&camera_, &CameraController::resolutionChanged, this, [this](int width, int height) {
        resolutionLabel_->setText(QString("Current resolution: %1 x %2").arg(width).arg(height));
    });
    connect(&camera_, &CameraController::errorOccurred, this, [this](const QString &message) {
        statusBar()->showMessage(message);
        QMessageBox::warning(this, "Camera Error", message);
    });
    connect(&camera_, &CameraController::stateChanged, this, [this](const QString &state) {
        statusLabel_->setText("Status: " + state);
        if (state == "Stopped") {
            currentFrame_ = QPixmap();
            feedLabel_->clear();
            feedLabel_->setText("Camera feed stopped");
        }
    });
    connect(&telemetry_, &TelemetryGenerator::telemetryUpdated, this, &MainWindow::onTelemetry);

    telemetry_.start();
}

void MainWindow::onFrame(const QImage &frame)
{
    currentFrame_ = QPixmap::fromImage(frame);
    feedLabel_->setPixmap(currentFrame_.scaled(feedLabel_->size(),
                                               Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::onTelemetry(const TelemetryData &data)
{
    temperatureLabel_->setText(QString::number(data.temperature, 'f', 1) + " °C");
    speedLabel_->setText(QString::number(data.speed) + " km/h");
    latitudeLabel_->setText(QString::number(data.latitude, 'f', 5));
    longitudeLabel_->setText(QString::number(data.longitude, 'f', 5));
    rpmLabel_->setText(QString::number(data.motorRpm) + " rpm");
    lastUpdateLabel_->setText("Last update: " +
                              QDateTime::currentDateTime().toString("hh:mm:ss"));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (!currentFrame_.isNull()) {
        feedLabel_->setPixmap(currentFrame_.scaled(feedLabel_->size(),
                                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
