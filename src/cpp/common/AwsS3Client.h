#pragma once
#include <QSettings>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/Scheme.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/S3ClientConfiguration.h>
#include <aws/s3/model/Bucket.h>
#include <format>
#include <optional>
#include <qsettings.h>
#include <vector>

class AwsS3Client {
public:
  static AwsS3Client &instance() {
    QSettings settings("Xigua", "CloudOSS");

    std::string currentUserId = "test"; // 测试
    auto accessKeyIds = settings.value(std::format("user:{}:accessKeyId", currentUserId)).toStringList();
    // std::string accessKeyId = accessKeyIds.at(0).toStdString();
    // auto secretKey = settings.value(std::format("user:{}:secretKey", accessKeyId)).toString();
    // static AwsS3Client client{accessKeyId, secretKey.toStdString()};
    static AwsS3Client client{"6BMCL6FRCIR5DRK1GGNQ", "kXd4EIIlrFbt1axcuVxJsorxPintBab4ZCU6yJgI"};
    return client;
  }

  bool createBucket(const std::string &bucketName);

  bool deleteBucket(const std::string &bucketName);

  bool deleteObject(const std::string &objectKey, const std::string &fromBucket);

  bool getObject(const std::string &objectKey, const std::string &fromBucket);

  std::optional<std::vector<Aws::S3::Model::Bucket>> listBuckets();

  bool putObject(const std::string &bucketName, const std::string &fileName);
  bool putObject(const std::string &bucketName, const std::string &fileName, const std::string &objectName);

  std::optional<std::vector<Aws::S3::Model::Object>> listObjects(const std::string &bucketName);

  bool putBucketCors(const std::string &bucketName);

  ~AwsS3Client() {
    Aws::ShutdownAPI(options_);
  }

private:
  AwsS3Client(const std::string &accessKeyId, const std::string &secretKey) : client_(std::move(initializeClient(accessKeyId, secretKey))) {
  }

  Aws::S3::S3Client initializeClient(const std::string &accessKeyId,
                                     const std::string &secretKey) {
    Aws::InitAPI(options_); // 先初始化 SDK

    Aws::Auth::AWSCredentials creds(accessKeyId, secretKey);
    Aws::Client::ClientConfiguration base;
    base.endpointOverride = "192.168.205.49:80";
    base.scheme = Aws::Http::Scheme::HTTP;
    base.verifySSL = false;
    base.checksumConfig.requestChecksumCalculation = Aws::Client::RequestChecksumCalculation::WHEN_REQUIRED;
    Aws::S3::S3ClientConfiguration s3cfg(base);
    s3cfg.useVirtualAddressing = false;
    return Aws::S3::S3Client(creds, nullptr, s3cfg);
  }

  Aws::SDKOptions options_;
  Aws::S3::S3Client client_;
};
