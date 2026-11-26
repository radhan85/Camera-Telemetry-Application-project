#include "CameraController.hpp"
#include "TelemetryGenerator.hpp"
#include "VideoItem.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("Camera Telemetry");
    app.setOrganizationName("Militros");

    qmlRegisterType<VideoItem>("CameraTelemetry", 1, 0, "VideoItem");

    CameraController cameraController;
    TelemetryGenerator telemetryGenerator;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("cameraController", &cameraController);
    engine.rootContext()->setContextProperty("telemetryGenerator", &telemetryGenerator);

    const QUrl mainUrl(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                     [mainUrl](QObject *object, const QUrl &url) {
                         if (!object && url == mainUrl) {
                             QCoreApplication::exit(-1);
                         }
                     },
                     Qt::QueuedConnection);
    engine.load(mainUrl);
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    telemetryGenerator.start();
    return app.exec();
}
