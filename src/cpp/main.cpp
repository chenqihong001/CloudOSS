#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <qcoreapplication.h>
#include <qguiapplication.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qqmlapplicationengine.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/icons/app.svg"));
  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app, [] {
        QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.loadFromModule("CloudOSS", "Main");
  return app.exec();
}
