#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Camera Telemetry");
    app.setOrganizationName("Militros");

    MainWindow window;
    window.show();
    return app.exec();
}
