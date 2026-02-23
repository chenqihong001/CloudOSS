pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CloudOSS
import "components"

Window {
    id: window
    visible: true
    width: 1000
    height: 600
    title: "CloudOSS"
    StackView {
        id: stackView
        initialItem: loginPage
        anchors.fill: parent
    }

    Component {
        id: loginPage
        LoginPage {
            onSwitchToRegister: {
                stackView.push(registerPage, StackView.ReplaceTransition);
            }
        }
    }
    Component {
        id: registerPage
        RegisterPage {

            onSwitchToDashboard: {
                stackView.replace(dashboardPage, StackView.ReplaceTransition);
            }
            onSwitchToLogin: {
                stackView.replace(loginPage);
            }
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
        target: AuthService
        function onLoginFailed(error) {
            console.log("登录失败", error);
            ToastService.info("网络错误");
        }
    }
    Connections {
        target: ToastService
        function onShowRequested(message, type) {
            toast.show(message, type);
        }
    }
}
