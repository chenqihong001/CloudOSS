
// #include <QGuiApplication>
// #include <QQmlApplicationEngine>
// #include <QQmlContext>

// int main(int argc, char *argv[]) {
//   QGuiApplication app(argc, argv);
//   QQmlApplicationEngine engine;
//   QObject::connect(
//       &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
//       []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
//   engine.loadFromModule("CloudOSS", "Main");

//   return app.exec();
// }
#include "AwsS3Client.h"
#include "common/Logger.h"
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/Scheme.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/S3ClientConfiguration.h>

int main() {

  OSS::Logger::init();

  // {
  //   Aws::Auth::AWSCredentials creds("6BMCL6FRCIR5DRK1GGNQ", "kXd4EIIlrFbt1axcuVxJsorxPintBab4ZCU6yJgI");
  //   Aws::Client::ClientConfiguration base;
  //   base.endpointOverride = "192.168.205.49:80";
  //   base.scheme = Aws::Http::Scheme::HTTP;
  //   base.verifySSL = false;
  //   Aws::S3::S3ClientConfiguration s3cfg(base);
  //   s3cfg.useVirtualAddressing = false;
  //   Aws::S3::S3Client s3(creds, nullptr, s3cfg);
  // }
  // Aws::ShutdownAPI(options);

  auto &client = AwsS3Client::instance();
  bool fg;
  client.putObject("test-bucket", "D:/Projects/CppDemo/VscodeCppDemo/env.ps1", "env.txt");
  auto ret = client.listBuckets();
  if (ret != std::nullopt) {
    for (const auto &bucket : ret.value()) {
      LOG_INFO << bucket.GetName();
    }
  }

  auto rett = client.listObjects("test-bucket");
  if (rett != std::nullopt) {
    for (const auto &obj : rett.value()) {
      LOG_INFO << "Object:" << obj.GetKey();
    }
  }

  return 0;
}

// 设置用户配额
// radosgw-admin quota set --quota-scope=user --uid=<用户名> --max-objects=<最大对象数> --max-size=<最大字节数>
// 设置存储桶配额
// radosgw-admin quota set --uid=<用户名> --quota-scope=bucket --bucket=<存储桶名> --max-objects=<最大对象数> --max-size=<最大字节数>