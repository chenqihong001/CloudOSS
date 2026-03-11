#pragma once
#include <QObject>
#include <QQmlEngine>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qsettings.h>
#include <qtmetamacros.h>
class ConfigManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
public:
  ConfigManager(QObject *parent = nullptr) : QObject(parent) {}

  // 从QSettings加载一些基本数据
  Q_INVOKABLE void loadConfig();

  Q_INVOKABLE QString email() const { return email_; }
  Q_INVOKABLE QString username() const { return username_; }
  Q_INVOKABLE QString userId() const { return userId_; }
  QString token() const { return token_; }

  void setEmail(const QString &email) { email_ = email; }
  void setUsername(const QString &username) { username_ = username; }
  void setToken(const QString &token) { token_ = token; }
  void setTokenExpiresAtSec(qint64 tokenExpiresAtSec) { tokenExpiresAtSec_ = tokenExpiresAtSec; }
  void setUserId(const QString &userId) { userId_ = userId; }

  QStringList accessKeyIds() const {
    auto key = std::format("user:{}:accessKeyId", userId_.toStdString());
    return settings_.value(key).toStringList();
  }

  QString accessKeyId() {
    auto key = std::format("user:{}:accessKeyId", userId_.toStdString());
    return settings_.value(key).toStringList().at(0);
  }

  void addAccessKeyId(const QString &accessKeyId) {
    accessKeyId_ = accessKeyId; // 保存到内存
    QSettings settings("Xigua", "CloudOSS");
    auto key = std::format("user:{}:accessKeyId", userId_.toStdString());
    auto list = settings.value(key).toStringList();
    list.append(accessKeyId);
    settings.setValue(key, list);
  }

  void removeAccessKeyId(const QString &accessKeyId) {
    QSettings settings("Xigua", "CloudOSS");
    auto key = std::format("user:{}:accessKeyId", userId_.toStdString());
    auto list = settings.value(key).toStringList();
    list.removeAll(key);
    settings.setValue(key, list);
  }

  // 根据accessKey 获取对应的secretKey
  QString getSecretKey(const QString &accessKeyId) {
    return settings_.value(accessKeyId).toString();
  }

private:
  QString email_;
  QString username_;
  QString token_;
  qint64 tokenExpiresAtSec_; // Token过期时间
  QString userId_ = "test";
  QSettings settings_{"Xigua", "CloudOSS"};
  QString accessKeyId_ = "6BMCL6FRCIR5DRK1GGNQ";
  QString secretKey_ = "kXd4EIIlrFbt1axcuVxJsorxPintBab4ZCU6yJgI";
};