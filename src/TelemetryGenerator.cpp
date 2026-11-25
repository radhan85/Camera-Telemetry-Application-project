#include "TelemetryGenerator.h"

#include <QRandomGenerator>
#include <cmath>

TelemetryGenerator::TelemetryGenerator(QObject *parent)
    : QObject(parent)
{
    connect(&timer_, &QTimer::timeout, this, &TelemetryGenerator::generate);
}

void TelemetryGenerator::start()
{
    generate();
    timer_.start(2000);
}

void TelemetryGenerator::generate()
{
    data_.temperature = 20.0 + QRandomGenerator::global()->generateDouble() * 60.0;
    data_.speed = QRandomGenerator::global()->bounded(121);
    data_.latitude += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.001;
    data_.longitude += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.001;
    data_.motorRpm = QRandomGenerator::global()->bounded(6001);
    emit telemetryUpdated(data_);
}
