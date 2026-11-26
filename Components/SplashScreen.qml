pragma ComponentBehavior: Bound

import QtQuick

Item {
    id: root
    property var theme
    signal finished()

    Rectangle {
        anchors.fill: parent
        color: root.theme.background
    }

    Column {
        anchors.centerIn: parent
        spacing: root.theme.spacing

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "CAMERA TELEMETRY"
            color: root.theme.text
            font.family: root.theme.fontFamily
            font.pixelSize: 28
            font.bold: true
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Initializing live dashboard..."
            color: root.theme.mutedText
            font.family: root.theme.fontFamily
            font.pixelSize: root.theme.bodyFontSize
        }

        Rectangle {
            width: 240
            height: 4
            radius: 2
            color: root.theme.surfaceRaised
            clip: true

            Rectangle {
                id: progressFill
                height: parent.height
                width: 0
                radius: parent.radius
                color: root.theme.accent
            }

            NumberAnimation {
                target: progressFill
                property: "width"
                from: 0
                to: 240
                duration: 1000
                running: true
            }
        }
    }

    Timer {
        interval: 1200
        running: true
        onTriggered: root.finished()
    }
}
