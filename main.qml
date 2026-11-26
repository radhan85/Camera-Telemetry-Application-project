pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import "Components"

ApplicationWindow {
    id: window
    width: 1180
    height: 720
    minimumWidth: 900
    minimumHeight: 600
    visible: true
    title: "Camera Telemetry"
    color: themePalette.background
    property var cameraControllerObject: cameraController
    property var telemetryGeneratorObject: telemetryGenerator

    Theme {
        id: themePalette
    }

    Loader {
        id: screenLoader
        anchors.fill: parent
        sourceComponent: splashComponent
    }

    Component {
        id: splashComponent

        SplashScreen {
            theme: themePalette
            onFinished: screenLoader.sourceComponent = mainScreenComponent
        }
    }

    Component {
        id: mainScreenComponent

        MainScreen {
            theme: themePalette
            cameraController: window.cameraControllerObject
            telemetryGenerator: window.telemetryGeneratorObject
        }
    }
}
