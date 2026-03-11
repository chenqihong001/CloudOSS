#pragma once
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <functional>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlogging.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qobject.h>
#include <qoverload.h>

class HttpClient : public QObject {
public:
  using Callback = std::function<void(const QJsonObject &)>;
  static HttpClient &instance();

  HttpClient(const QString &host, QObject *parent = nullptr) : QObject(parent), host_(host) {
    manager_ = new QNetworkAccessManager(this);
  }
  static QString handleNetworkError(QNetworkReply *reply); // 处理错误

  QNetworkReply *get(const QString &path);

  QNetworkReply *post(const QString &path, const QJsonObject &json);
  void post(const QString &path, const QJsonObject &req, Callback &&callback);

  QNetworkReply *put(const QString &path, const QJsonObject &json);
  QNetworkReply *del(const QString &path);

private:
  QNetworkRequest buildRequest(const QString &path);

  QString host_;
  QString accessToken_;
  QNetworkAccessManager *manager_;
};