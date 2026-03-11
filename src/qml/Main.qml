pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CloudOSS
import "components"

Window {
    id: window

    visible: true
    width: 1280
    height: 820
    minimumWidth: 1080
    minimumHeight: 720
    title: "CloudOSS 控制台"
    color: "#f6f8fa"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginPage
        clip: true
    }

    Component {
        id: loginPage

        LoginPage {
            onSwitchToRegister: stackView.push(registerPage, StackView.PushTransition)
            onSwitchToDashboard: stackView.replace(dashboardPage, StackView.ReplaceTransition)
        }
    }

    Component {
        id: registerPage

        RegisterPage {
            onSwitchToLogin: stackView.pop(StackView.PopTransition)
            onSwitchToDashboard: stackView.replace(dashboardPage, StackView.ReplaceTransition)
        }
    }

    Component {
        id: dashboardPage

        DashboardPage {}
    }

    DanmuToast {
        id: toast
    }

    Connections {
        target: ToastService

        function onShowRequested(message, type) {
            toast.show(message, type);
        }
    }
}



