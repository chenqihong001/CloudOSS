#include "AuthService.h"
#include "HttpClient.h"
#include "UserManager.h"
#include <QJsonObject>
#include <qdebug.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonparseerror.h>
#include <qlogging.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qqml.h>

void AuthService::login(const QString &username, const QString &password) {
  auto &client = HttpClient::instance();
  QJsonObject req;
  req["username"] = username;
  req["password"] = password;
  client.post("api/v1/auth/login", req, [this](const QJsonObject &resp) {
    if (resp.contains("error") && !resp["error"].toString().isEmpty()) {
      qWarning() << "[AuthService] login(): " << resp["error"];
      emit loginFailed(resp["error"].toString()); // 发送给QML
      return;                                     // 结束
    }

    // 没有错误
    UserManager *userManager = qobject_cast<UserManager *>(
        qmlEngine(this)->singletonInstance<UserManager *>("CloudOSS", "UserManager"));
    userManager->setUsername(resp["username"].toString());
    userManager->setToken(resp["token"].toString());
    userManager->setEmail(resp["email"].toString());
    emit loginSucceed();
  });
}