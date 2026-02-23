import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic
import CloudOSS

Rectangle {
    id: root
    // color: "green"
    signal switchToRegister

    RowLayout {
        anchors.fill: parent
        // Left
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "pink"
        }
        LoginCard {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
