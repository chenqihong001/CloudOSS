
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <aws/s3/S3Client.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("CloudOSS", "Main");

  return app.exec();
}
