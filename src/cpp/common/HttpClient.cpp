#pragma once
#include "HttpClient.h"
#include <QJsonDocument>
#include <aws/s3/S3ServiceClientModel.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonparseerror.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qobject.h>
#include <qstringview.h>
#include <qurl.h>

HttpClient &HttpClient::instance() {
  static HttpClient client("http://localhost:5566");
  return client;
}

QNetworkReply *HttpClient::get(const QString &path) {
  return manager_->get(buildRequest(path));
}

QNetworkReply *HttpClient::post(const QString &path, const QJsonObject &json) {
  auto req = buildRequest(path);
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QJsonDocument doc(json);
  return manager_->post(req, doc.toJson());
}

void HttpClient::post(const QString &path, const QJsonObject &req, Callback &&callback) {
  auto reply = post(path, req);
  connect(reply, &QNetworkReply::finished, [callback = std::move(callback), reply]() {
    QJsonObject resp;
    if (reply->error() != QNetworkReply::NoError) {
      handleNetworkError(reply);
      resp["error"] = handleNetworkError(reply);
      callback(resp);
      return;
    }
    // 没有错误
    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
      resp["error"] = parseError.errorString();
      callback(resp);
      return;
    }
    callback(doc.object()); // 返回结果
  });
}

QNetworkReply *HttpClient::put(const QString &path, const QJsonObject &json) {
  auto req = buildRequest(path);
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QJsonDocument doc(json);
  return manager_->put(req, doc.toJson());
}

QNetworkReply *HttpClient::del(const QString &path) {
  return manager_->deleteResource(buildRequest(path));
}

QNetworkRequest HttpClient::buildRequest(const QString &path) {
  return QNetworkRequest{QUrl{host_ + path}};
}

QString HttpClient::handleNetworkError(QNetworkReply *reply) {
  auto error = reply->errorString();
  qWarning() << "[HttpClient] handleNetworkError(): " << error;
  return error;
}