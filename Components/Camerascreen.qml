pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CameraTelemetry 1.0

Rectangle {
    id: root
    property var theme
    property var cameraController
    property string cameraStatus: "Stopped"
    color: theme.surface
    radius: 8
    border.color: theme.border

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: root.theme.feedBackground
            radius: 4
            clip: true

            VideoItem {
                id: videoItem
                anchors.fill: parent
            }
        }

        Label {
            Layout.fillWidth: true
            text: "Camera status: " + root.cameraStatus
            color: root.theme.text
            font.family: root.theme.fontFamily
            font.pixelSize: root.theme.bodyFontSize
        }
    }

    Connections {
        target: root.cameraController

        function onFrameReady(frame) {
            videoItem.setFrame(frame)
        }

        function onStateChanged(state) {
            root.cameraStatus = state
        }
    }
}
