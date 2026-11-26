pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var cameraController
    property var telemetryGenerator
    property string bannerText: "Ready"
    property string bannerType: "info"

    RowLayout {
        anchors.fill: parent
        anchors.margins: root.theme.spacing
        spacing: root.theme.spacing

        Telemetrydata {
            Layout.preferredWidth: 280
            Layout.minimumWidth: 260
            Layout.fillHeight: true
            theme: root.theme
            telemetry: root.telemetryGenerator
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: root.theme.spacing

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 36
                radius: 6
                color: root.bannerType === "error" ? "#7f1d1d"
                      : root.bannerType === "warning" ? "#78350f"
                      : root.theme.surface

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    verticalAlignment: Text.AlignVCenter
                    text: root.bannerText
                    color: root.theme.text
                    font.family: root.theme.fontFamily
                    font.pixelSize: root.theme.bodyFontSize
                    elide: Text.ElideRight
                }
            }

            Camerascreen {
                Layout.fillWidth: true
                Layout.fillHeight: true
                theme: root.theme
                cameraController: root.cameraController
            }

            Controls {
                Layout.fillWidth: true
                theme: root.theme
                cameraController: root.cameraController
            }
        }
    }

    Dialog {
        id: errorDialog
        property string messageText: ""
        modal: true
        title: "Camera error"
        standardButtons: Dialog.Ok
        x: (root.width - width) / 2
        y: (root.height - height) / 2

        contentItem: Label {
            text: errorDialog.messageText
            color: root.theme.text
            wrapMode: Text.Wrap
            width: 360
        }
    }

    Popup {
        id: warningPopup
        property string messageText: ""
        width: Math.min(480, root.width - 32)
        height: warningMessage.implicitHeight + 24
        x: (root.width - width) / 2
        y: 12
        modal: false
        focus: false
        closePolicy: Popup.NoAutoClose

        background: Rectangle {
            color: "#78350f"
            border.color: "#f59e0b"
            radius: 6
        }

        contentItem: Label {
            id: warningMessage
            text: warningPopup.messageText
            color: root.theme.text
            wrapMode: Text.Wrap
        }

        Timer {
            id: warningTimer
            interval: 5000
            onTriggered: warningPopup.close()
        }
    }

    Connections {
        target: root.cameraController

        function onErrorOccurred(message) {
            root.bannerText = message
            root.bannerType = "error"
            errorDialog.messageText = message
            errorDialog.open()
        }

        function onWarningOccurred(message) {
            root.bannerText = message
            root.bannerType = "warning"
            warningPopup.messageText = message
            warningPopup.open()
            warningTimer.restart()
        }
    }
}
