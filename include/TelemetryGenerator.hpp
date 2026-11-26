#pragma once

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QTimer>

class TelemetryGenerator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double temperature READ temperature NOTIFY telemetryUpdated)
    Q_PROPERTY(int speed READ speed NOTIFY telemetryUpdated)
    Q_PROPERTY(double latitude READ latitude NOTIFY telemetryUpdated)
    Q_PROPERTY(double longitude READ longitude NOTIFY telemetryUpdated)
    Q_PROPERTY(int motorRpm READ motorRpm NOTIFY telemetryUpdated)
    Q_PROPERTY(QString lastUpdate READ lastUpdate NOTIFY telemetryUpdated)

public:
    explicit TelemetryGenerator(QObject *parent = nullptr);

    double temperature() const;
    int speed() const;
    double latitude() const;
    double longitude() const;
    int motorRpm() const;
    QString lastUpdate() const;

    void start();

signals:
    void telemetryUpdated();

private slots:
    void generate();

private:
    QTimer timer_;
    double temperature_ = 42.3;
    int speed_ = 67;
    double latitude_ = 12.97160;
    double longitude_ = 77.59460;
    int motorRpm_ = 3450;
    QDateTime lastUpdate_;
};
