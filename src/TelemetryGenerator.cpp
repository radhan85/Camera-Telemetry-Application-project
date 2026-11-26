#include "TelemetryGenerator.hpp"

#include <QRandomGenerator>
#include <cmath>

TelemetryGenerator::TelemetryGenerator(QObject *parent)
    : QObject(parent)
{
    connect(&timer_, &QTimer::timeout, this, &TelemetryGenerator::generate);
}

double TelemetryGenerator::temperature() const
{
    return temperature_;
}

int TelemetryGenerator::speed() const
{
    return speed_;
}

double TelemetryGenerator::latitude() const
{
    return latitude_;
}

double TelemetryGenerator::longitude() const
{
    return longitude_;
}

int TelemetryGenerator::motorRpm() const
{
    return motorRpm_;
}

QString TelemetryGenerator::lastUpdate() const
{
    return lastUpdate_.toString("hh:mm:ss");
}

void TelemetryGenerator::start()
{
    generate();
    timer_.start(2000);
}

void TelemetryGenerator::generate()
{
    temperature_ = 20.0 + QRandomGenerator::global()->generateDouble() * 60.0;
    speed_ = QRandomGenerator::global()->bounded(121);
    latitude_ += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.001;
    longitude_ += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.001;
    motorRpm_ = QRandomGenerator::global()->bounded(6001);
    lastUpdate_ = QDateTime::currentDateTime();
    emit telemetryUpdated();
}
