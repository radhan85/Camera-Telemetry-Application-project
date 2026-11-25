#pragma once

#include <QObject>
#include <QTimer>

struct TelemetryData
{
    double temperature = 42.3;
    int speed = 67;
    double latitude = 12.97160;
    double longitude = 77.59460;
    int motorRpm = 3450;
};

class TelemetryGenerator : public QObject
{
    Q_OBJECT

public:
    explicit TelemetryGenerator(QObject *parent = nullptr);
    void start();

signals:
    void telemetryUpdated(const TelemetryData &data);

private slots:
    void generate();

private:
    QTimer timer_;
    TelemetryData data_;
};
