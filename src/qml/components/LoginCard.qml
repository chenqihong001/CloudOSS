import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import CloudOSS

Rectangle {
    id: root
    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 40
        spacing: 30
        Item {
            width: 1
            height: 20
        }
        Text {
            text: "欢迎登录"
            font.pixelSize: 25
            font.bold: true
            color: "#1f2937"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        // 用户名
        TextField {
            id: userField
            width: parent.width
            placeholderText: "请输入用户名..."
            leftPadding: 16
            rightPadding: 16
            topPadding: 12
            bottomPadding: 12
            background: Rectangle {
                radius: 10
                color: userField.focus ? "#f3f4f6" : "#f9fafb"
                border.width: 1
                border.color: userField.focus ? "#6366f1" : "#e5e7eb"
            }
        }
        // 密码
        TextField {
            id: pwdFiled
            width: parent.width
            placeholderText: "请输入密码..."
            echoMode: TextInput.Password
            leftPadding: 16
            rightPadding: 16
            topPadding: 12
            bottomPadding: 12
            background: Rectangle {
                radius: 10
                color: pwdFiled.focus ? "#f3f4f6" : "#f9fafb"
                border.width: 1
                border.color: pwdFiled.focus ? "#6366f1" : "#e5e7eb"
            }
        }
        // 占位
        Item {
            width: 1
            height: 20
        }
        Rectangle {
            width: parent.width
            height: 48
            radius: 10
            color: "#6366f1"
            Text {
                anchors.centerIn: parent
                text: "登录"
                color: "white"
                font.bold: true
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("登录");
                    AuthService.login(userField.text, pwdFiled.text);
                }
            }
        }
        Text {
            text: "忘记密码"
            color: "#6366f1"
            anchors.horizontalCenter: parent.horizontalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
        }
    }
}
