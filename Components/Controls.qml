pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var cameraController
    implicitHeight: controlsColumn.implicitHeight

    ColumnLayout {
        id: controlsColumn
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Repeater {
                model: ["Play", "Pause", "Resume", "Rewind", "Forward"]

                Button {
                    id: controlButton
                    required property var modelData
                    Layout.fillWidth: true
                    text: modelData
                    onClicked: {
                        switch (modelData) {
                        case "Play": root.cameraController.play(); break
                        case "Pause": root.cameraController.pause(); break
                        case "Resume": root.cameraController.resume(); break
                        case "Rewind": root.cameraController.rewind(5); break
                        case "Forward": root.cameraController.forward(5); break
                        }
                    }

                    background: Rectangle {
                        radius: 5
                        color: controlButton.down ? root.theme.accentPressed : root.theme.surfaceRaised
                        border.color: root.theme.border
                    }

                    contentItem: Text {
                        text: controlButton.text
                        color: root.theme.text
                        font.family: root.theme.fontFamily
                        font.pixelSize: root.theme.bodyFontSize
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Label {
                text: "Resolution"
                color: root.theme.text
                font.family: root.theme.fontFamily
            }

            ComboBox {
                id: resolutionCombo
                model: ["640 x 480", "1280 x 720", "1920 x 1080"]
                currentIndex: 1
                onActivated: function(index) {
                    const parts = currentText.split("x")
                    root.cameraController.setResolution(Number(parts[0].trim()), Number(parts[1].trim()))
                }

                contentItem: Text {
                    leftPadding: 8
                    text: resolutionCombo.displayText
                    color: root.theme.text
                    font.family: root.theme.fontFamily
                    font.pixelSize: root.theme.bodyFontSize
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 150
                    implicitHeight: 34
                    color: root.theme.surfaceRaised
                    border.color: root.theme.border
                    radius: 5
                }
            }

            Label {
                Layout.fillWidth: true
                text: "Current resolution: " + root.currentResolution
                color: root.theme.mutedText
                font.family: root.theme.fontFamily
                font.pixelSize: root.theme.bodyFontSize
            }
        }
    }

    property string currentResolution: "1280 x 720"

    Connections {
        target: root.cameraController

        function onResolutionChanged(width, height) {
            root.currentResolution = width + " x " + height
            const index = resolutionCombo.find(root.currentResolution)
            if (index >= 0 && index !== resolutionCombo.currentIndex) {
                resolutionCombo.currentIndex = index
            }
        }
    }
}
