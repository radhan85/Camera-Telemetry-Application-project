import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property var theme
    property var telemetry
    color: theme.surface
    radius: 8
    border.color: theme.border

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 16

        Label {
            text: "Telemetry Data"
            color: root.theme.text
            font.family: root.theme.fontFamily
            font.pixelSize: root.theme.headingFontSize
            font.bold: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: root.theme.border
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 12
            rowSpacing: 18

            Label { text: "Temperature"; color: root.theme.mutedText; font.family: root.theme.fontFamily }
            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                text: root.telemetry ? Number(root.telemetry.temperature).toFixed(1) + " °C" : "--"
                color: root.theme.text
                font.family: root.theme.fontFamily
            }

            Label { text: "Speed"; color: root.theme.mutedText; font.family: root.theme.fontFamily }
            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                text: root.telemetry ? root.telemetry.speed + " km/h" : "--"
                color: root.theme.text
                font.family: root.theme.fontFamily
            }

            Label { text: "GPS Latitude"; color: root.theme.mutedText; font.family: root.theme.fontFamily }
            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                text: root.telemetry ? Number(root.telemetry.latitude).toFixed(5) : "--"
                color: root.theme.text
                font.family: root.theme.fontFamily
            }

            Label { text: "GPS Longitude"; color: root.theme.mutedText; font.family: root.theme.fontFamily }
            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                text: root.telemetry ? Number(root.telemetry.longitude).toFixed(5) : "--"
                color: root.theme.text
                font.family: root.theme.fontFamily
            }

            Label { text: "Motor RPM"; color: root.theme.mutedText; font.family: root.theme.fontFamily }
            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                text: root.telemetry ? root.telemetry.motorRpm + " rpm" : "--"
                color: root.theme.text
                font.family: root.theme.fontFamily
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: root.theme.border
        }

        Label {
            text: "Last update: " + (root.telemetry ? root.telemetry.lastUpdate : "--")
            color: root.theme.mutedText
            font.family: root.theme.fontFamily
            font.pixelSize: root.theme.bodyFontSize
        }
    }
}
