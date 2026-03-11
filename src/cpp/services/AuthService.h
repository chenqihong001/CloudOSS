#pragma once
#include "HttpClient.h"
#include <QObject>
#include <QQmlEngine>
#include <qcontainerfwd.h>
#include <qtmetamacros.h>

class AuthService : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
public:
  AuthService(QObject *parent = nullptr) : QObject(parent), client(HttpClient::instance()) {}

  Q_INVOKABLE void login(const QString &username, const QString &password);
  Q_INVOKABLE void registerAccount(const QString &username, const QString &password, const QString &email);
  Q_INVOKABLE void refreshToken();
  Q_INVOKABLE void resetPassword(const QString &email, const QString &authCode, const QString newPwd);
  Q_INVOKABLE void forgotPassword(const QString &email); // 发送忘记密码验证码

signals:
  void loginSucceed();
  void loginFailed(const QString &error);

  void registerSucceed();
  void registerFailed(const QString &error);

  void refreshTokenSucceed();
  void refreshTokenFailed(const QString &error);

  void forgotPasswordSucceed();
  void forgotPasswordFailed(const QString &error);

  void resetPasswordFailed(const QString &error);
  void resetPasswordSucceed();

private:
  HttpClient &client;
};