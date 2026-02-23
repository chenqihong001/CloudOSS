// DanmuToast.qml
import QtQuick
import QtQuick.Controls

Item {
    id: root

    // 配置属性
    property int displayDuration: 3000
    property int animationDuration: 400
    property int maxWidth: 420
    property int toastHeight: 56

    // 内部状态
    property var messageQueue: []
    property bool isShowing: false

    // 尺寸跟随父级
    anchors.fill: parent

    // ── 类型配置 ──────────────────────────────────────────
    function getAccent(type) {
        switch (type) {
        case "error":
            return "#FF5370";
        case "success":
            return "#4DFFB4";
        case "warning":
            return "#FFB830";
        case "info":
            return "#58A6FF";
        default:
            return "#A0A0B0";
        }
    }

    function getGlow(type) {
        switch (type) {
        case "error":
            return Qt.rgba(1.0, 0.33, 0.44, 0.25);
        case "success":
            return Qt.rgba(0.30, 1.0, 0.71, 0.22);
        case "warning":
            return Qt.rgba(1.0, 0.72, 0.19, 0.25);
        case "info":
            return Qt.rgba(0.35, 0.65, 1.0, 0.25);
        default:
            return Qt.rgba(0.5, 0.5, 0.6, 0.18);
        }
    }

    function getIcon(type) {
        switch (type) {
        case "error":
            return "✕";
        case "success":
            return "✓";
        case "warning":
            return "!";
        case "info":
            return "i";
        default:
            return "·";
        }
    }

    // ── 公共接口 ──────────────────────────────────────────
    function show(message, type) {
        messageQueue.push({
            text: message,
            type: type || "info"
        });
        processQueue();
    }

    function error(msg) {
        show(msg, "error");
    }
    function success(msg) {
        show(msg, "success");
    }
    function warning(msg) {
        show(msg, "warning");
    }
    function info(msg) {
        show(msg, "info");
    }

    // ── 队列处理 ──────────────────────────────────────────
    function processQueue() {
        if (isShowing || messageQueue.length === 0)
            return;
        isShowing = true;
        var msg = messageQueue.shift();

        var accent = getAccent(msg.type);
        var glow = getGlow(msg.type);

        // 更新内容
        messageText.text = msg.text;
        iconLabel.text = getIcon(msg.type);
        accentBar.color = accent;
        iconCircle.color = Qt.rgba(Qt.color(accent).r, Qt.color(accent).g, Qt.color(accent).b, 0.18);
        iconLabel.color = accent;
        glowLayer.color = glow;

        // 重置位置
        toast.opacity = 0;
        toast.y = root.height - root.toastHeight - 80 + 24;

        showAnim.start();
    }

    // ── Toast 容器 ────────────────────────────────────────
    Item {
        id: toast

        width: Math.min(messageText.implicitWidth + 104, root.maxWidth)
        height: root.toastHeight
        anchors.horizontalCenter: parent.horizontalCenter
        y: root.height - root.toastHeight - 80
        opacity: 0
        z: 9999

        // 外发光层
        Rectangle {
            id: glowLayer
            anchors.centerIn: parent
            width: parent.width + 24
            height: parent.height + 16
            radius: 20
            color: Qt.rgba(0.35, 0.65, 1.0, 0.22)
            layer.enabled: true
            layer.effect: null  // 依赖自身半透明模拟光晕
        }

        // 磨砂主体
        Rectangle {
            id: glassBody
            anchors.fill: parent
            radius: 14
            color: Qt.rgba(0.10, 0.10, 0.14, 0.88)
            border.width: 1
            border.color: Qt.rgba(1, 1, 1, 0.09)

            // 内部细微渐变高光
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height * 0.45
                radius: parent.radius
                color: "transparent"
                border.width: 0

                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: Qt.rgba(1, 1, 1, 0.13)
                    radius: 14
                }
            }

            // 左侧彩色指示条
            Rectangle {
                id: accentBar
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
                width: 3
                height: parent.height * 0.55
                radius: 2
                color: "#58A6FF"
            }

            // 图标圆圈
            Rectangle {
                id: iconCircle
                anchors.left: accentBar.right
                anchors.leftMargin: 14
                anchors.verticalCenter: parent.verticalCenter
                width: 28
                height: 28
                radius: 14
                color: Qt.rgba(0.35, 0.65, 1.0, 0.18)

                Text {
                    id: iconLabel
                    anchors.centerIn: parent
                    text: "i"
                    color: "#58A6FF"
                    font.pixelSize: 13
                    font.bold: true
                    font.family: "Helvetica Neue"
                }
            }

            // 消息文字
            Text {
                id: messageText
                anchors.left: iconCircle.right
                anchors.leftMargin: 12
                anchors.right: parent.right
                anchors.rightMargin: 18
                anchors.verticalCenter: parent.verticalCenter
                color: "#E8E8F0"
                font.pixelSize: 13
                font.family: "Helvetica Neue"
                font.weight: Font.Normal
                wrapMode: Text.Wrap
                maximumLineCount: 2
                elide: Text.ElideRight
                lineHeight: 1.35
            }
        }

        // ── 动画序列 ──────────────────────────────────────
        SequentialAnimation {
            id: showAnim

            ParallelAnimation {
                NumberAnimation {
                    target: toast
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 280
                    easing.type: Easing.OutQuart
                }
                NumberAnimation {
                    target: toast
                    property: "y"
                    to: root.height - root.toastHeight - 80
                    duration: 320
                    easing.type: Easing.OutQuart
                }
            }

            PauseAnimation {
                duration: root.displayDuration
            }

            ParallelAnimation {
                NumberAnimation {
                    target: toast
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: root.animationDuration
                    easing.type: Easing.InQuart
                }
                NumberAnimation {
                    target: toast
                    property: "y"
                    to: root.height - root.toastHeight - 80 + 12
                    duration: root.animationDuration
                    easing.type: Easing.InQuart
                }
            }

            onFinished: {
                root.isShowing = false;
                root.processQueue();
            }
        }

        // ── 鼠标交互 ──────────────────────────────────────
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onEntered: {
                showAnim.pause();
                toast.opacity = 1;
                hoverHighlight.opacity = 1;
            }
            onExited: {
                showAnim.resume();
                hoverHighlight.opacity = 0;
            }
            onClicked: {
                showAnim.stop();
                dismissAnim.start();
            }
        }

        // 悬停高亮层
        Rectangle {
            id: hoverHighlight
            anchors.fill: glassBody
            radius: glassBody.radius
            color: Qt.rgba(1, 1, 1, 0.04)
            opacity: 0
            Behavior on opacity {
                NumberAnimation {
                    duration: 150
                }
            }
        }
    }

    // 点击消退动画
    SequentialAnimation {
        id: dismissAnim
        ParallelAnimation {
            NumberAnimation {
                target: toast
                property: "opacity"
                to: 0
                duration: 200
                easing.type: Easing.OutCubic
            }
            NumberAnimation {
                target: toast
                property: "y"
                to: toast.y + 10
                duration: 200
                easing.type: Easing.OutCubic
            }
        }
        onFinished: {
            root.isShowing = false;
            root.processQueue();
        }
    }
}
