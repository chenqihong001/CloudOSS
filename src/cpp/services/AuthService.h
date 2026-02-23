#pragma once
#include <QObject>
#include <QQmlEngine>
#include <qcontainerfwd.h>
#include <qtmetamacros.h>

class AuthService : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
public:
  AuthService(QObject *parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE void login(const QString &username, const QString &password);

signals:
  void loginSucceed();
  void loginFailed(const QString &error);

private:
};