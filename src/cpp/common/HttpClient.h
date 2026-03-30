#pragma once
#include <QJsonObject>
#include <functional>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qtmetamacros.h>
class HttpClient : public QObject {
  Q_OBJECT
public:
  using Callback = std::function<void(const QJsonObject &)>;

  static HttpClient &instance();
  void setBaseUrl(const QString &baseUrl);
  void setAccessToken(const QString &token);

  void get(const QString &path, Callback &&callback);
  void post(const QString &path, const QJsonObject &req, Callback &&callback);
  void put(const QString &path, const QJsonObject &req, Callback &&callback);
  void del(const QString &path, const QJsonObject &req, Callback &&callback);

private:
  explicit HttpClient(QObject *parent = nullptr);
  QNetworkRequest buildRequest(const QString &path) const;
  void handleReply(QNetworkReply *reply, Callback &&callback);

private:
  QString baseUrl_{"http://127.0.0.1:5566"};
  QString accessToken_;
  QNetworkAccessManager manager_;
};