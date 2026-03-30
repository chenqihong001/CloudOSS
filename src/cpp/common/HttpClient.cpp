#include "HttpClient.h"
#include <QJsonObject>
#include <QJsonParseError>
#include <qcborvalue.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonparseerror.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qobject.h>
#include <qurl.h>
HttpClient::HttpClient(QObject *parent) : QObject(parent) {}

HttpClient &HttpClient::instance() {
  static HttpClient client;
  client.setBaseUrl("http://127.0.0.1:5566");
  return client;
}

void HttpClient::setBaseUrl(const QString &baseUrl) {
  baseUrl_ = baseUrl;
}

void HttpClient::setAccessToken(const QString &token) {
  accessToken_ = token;
}

QNetworkRequest HttpClient::buildRequest(const QString &path) const {
  QString normalized = path.startsWith("/") ? path : "/" + path;
  QNetworkRequest req(QUrl(baseUrl_ + normalized));
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  if (!accessToken_.isEmpty()) {
    req.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken_.toUtf8());
  }
  return req;
}

void HttpClient::handleReply(QNetworkReply *reply, Callback &&callback) {
  QObject::connect(reply, &QNetworkReply::finished, [reply, callback = std::move(callback)]() mutable {
    QJsonObject result;
    if (reply->error() != QNetworkReply::NoError) {
      result.insert("success", false);
      result.insert("error", reply->errorString());
      callback(result);
      reply->deleteLater();
      return;
    }
    const QByteArray body = reply->readAll();
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
      result.insert("success", false);
      result.insert("error", "invalid json response");
      callback(result);
      reply->deleteLater();
      return;
    }
    callback(doc.object());
    reply->deleteLater();
  });
}

void HttpClient::get(const QString &path, Callback &&callback) {
  QNetworkReply *reply = manager_.get(buildRequest(path));
  handleReply(reply, std::move(callback));
}
void HttpClient::post(const QString &path, const QJsonObject &req, Callback &&callback) {
  QNetworkReply *reply = manager_.post(buildRequest(path), QJsonDocument(req).toJson());
  handleReply(reply, std::move(callback));
}

void HttpClient::put(const QString &path, const QJsonObject &req, Callback &&callback) {
  QNetworkReply *reply = manager_.put(buildRequest(path), QJsonDocument(req).toJson());
  handleReply(reply, std::move(callback));
}

void HttpClient::del(const QString &path, const QJsonObject &req, Callback &&callback) {
  QNetworkRequest request = buildRequest(path);
  QNetworkReply *reply =
      manager_.sendCustomRequest(request, "DELETE", QJsonDocument(req).toJson());
  handleReply(reply, std::move(callback));
}
