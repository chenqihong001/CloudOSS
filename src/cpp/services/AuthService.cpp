#include "AuthService.h"
#include "ConfigManager.h"
#include "HttpClient.h"
#include <QJsonObject>
#include <qdebug.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonparseerror.h>
#include <qlogging.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qqml.h>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

void AuthService::login(const QString &username, const QString &password) {
  auto &client = HttpClient::instance();
  QJsonObject req;
  req["username"] = username;
  req["password"] = password;
  client.post("api/v1/auth/login", req, [this](const QJsonObject &resp) {
    if (!resp["success"].toBool()) {
      qWarning() << "[AuthService] login(): " << resp["errors"];
      emit loginFailed(resp["error"].toString()); // 发送给QML
      return;                                     // 结束
    }
    // 没有错误
    ConfigManager *manager = qobject_cast<ConfigManager *>(
        qmlEngine(this)->singletonInstance<ConfigManager *>("CloudOSS", "ConfigManager"));
    manager->setUsername(resp["data"]["user"]["username"].toString());
    manager->setToken(resp["data"]["token"].toString());
    manager->setEmail(resp["data"]["user"]["email"].toString());
    manager->setUserId(resp["data"]["user"]["id"].toString());
    manager->setTokenExpiresAtSec(resp["data"]["tokenExpiresAtSec"].toInteger());
    emit loginSucceed();
  });
}

void AuthService::registerAccount(const QString &username, const QString &password, const QString &email) {
  auto &client = HttpClient::instance();
  QJsonObject req;
  req["username"] = username;
  req["password"] = password;
  req["email"] = email;
  client.post("api/v1/auth/register", req, [this](const QJsonObject &resp) {
    if (!resp["success"].toBool()) {
      qWarning() << "[AuthService] registerAccount(): " << resp["errors"].toString();
      emit registerFailed(resp["errors"].toString());
      return;
    }
    // 成功
    emit registerSucceed();
  });
}

/*
失败响应
{
  "success":false,
  "errors":"邮箱已被注册"
}

成功响应
{
  "success":true
  "data":{
    "username":"",
    "userId":"",
    "email":""
  }
}
 */

// 刷新Token
void AuthService::refreshToken() {
  auto *manager = qobject_cast<ConfigManager *>(
      qmlEngine(this)->singletonInstance<ConfigManager *>("CloudOSS", "ConfigManager"));
  auto token = manager->token();
  QJsonObject req;
  req["refreshToken"] = token;
  auto &client = HttpClient::instance();
  client.post("api/v1/auth/refresh-token", req, [this, manager](const QJsonObject &resp) {
    // "data":{"token":"","expiresAtSec":}
    if (!resp["success"].toBool()) {
      qWarning() << "[AuthService] refreshToken: " << resp["errors"].toString();
      emit refreshTokenFailed(resp["errors"].toString());
      return;
    }
    //{"success":true,"data":{"token":"","tokenexpiresAtSec":}}
    manager->setToken(resp["data"]["token"].toString());
    manager->setTokenExpiresAtSec(resp["data"]["tokenExpiresAtSec"].toInteger());
    emit refreshTokenSucceed();
  });
}

void AuthService::forgotPassword(const QString &email) {
  QJsonObject req;
  req["email"] = email;
  client.post("/api/v1/auth/send-authcode", req, [this](const QJsonObject &resp) {
    if (!resp["success"].toBool()) {
      qWarning() << "[AuthService] sendForgotPwdAuthCode(): " << resp["errors"].toString();
      // errors: 邮箱不存在
      emit forgotPasswordFailed(resp["errors"].toString());
      return;
    }
    // 验证码发送到邮箱
    emit forgotPasswordSucceed();
  });
}

void AuthService::resetPassword(const QString &email, const QString &authCode, const QString newPwd) {
  // 和注册一样
  QJsonObject req;
  req["email"] = email;
  req["newPassword"] = newPwd;
  req["authCode"] = authCode;
  client.post("api/v1/auth/forgot-password", req, [this](const QJsonObject &resp) {
    if (!resp["success"].toBool()) {
      // 失败
      auto errors = resp["errors"].toString();
      qWarning() << "[AuthService] forgotPassword:" << errors;
      // errors:验证码错误|邮箱不存在
      emit resetPasswordFailed(errors);
      return;
    }
    // 重置密码成功
    emit resetPasswordSucceed(); // 进入登录界面
  });
}