import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: "pink"
    signal switchToLogin
    signal switchToDashboard
    Button {
        text: "switch to 登录"
        onClicked: {
            root.switchToLogin();
        }
    }

    Button {
        y: 20
        text: "switch to Dash"
        onClicked: {
            root.switchToDashboard();
        }
    }
}
