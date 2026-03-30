#pragma once

#include "HttpClient.h"
#include <QObject>
#include <QQmlEngine>
#include <qqmlintegration.h>

class AuthService : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
public:
  explicit AuthService(QObject *parent = nullptr);

signals:
  HttpClient &client_ = HttpClient::instance();
};