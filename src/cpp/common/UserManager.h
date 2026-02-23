#pragma once
#include <QObject>
#include <QQmlEngine>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
class UserManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  UserManager(QObject *parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE QString email() const { return email_; }
  Q_INVOKABLE QString username() const { return username_; }

  void setEmail(const QString &email) { email_ = email; }
  void setUsername(const QString &username) { username_ = username; }
  void setToken(const QString &token) { token_ = token; }

private:
  QString email_;
  QString username_;
  QString token_;
};