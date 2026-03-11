#include "AwsS3Client.h"
#include "Logger.h"
#include <aws/core/utils/memory/stl/AWSAllocator.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/S3Errors.h>
#include <aws/s3/S3ServiceClientModel.h>
#include <aws/s3/model/CORSConfiguration.h>
#include <aws/s3/model/CORSRule.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/DeleteBucketRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/Object.h>
#include <aws/s3/model/PutBucketCorsRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>
#include <ios>
#include <memory>
#include <optional>

// 解决window.h GetObject冲突
#undef GetObject

bool AwsS3Client::createBucket(const std::string &bucketName) {
  Aws::S3::Model::CreateBucketRequest request;
  request.SetBucket(bucketName);
  auto outcome = client_.CreateBucket(request);
  if (!outcome.IsSuccess()) {
    auto err = outcome.GetError();
    LOG_ERROR << "Error: createBucket: " << err.GetExceptionName() << ":" << err.GetMessage() << '\n';
    return false;
  }
  LOG_INFO << "Bucket:" << bucketName << "Create Success";
  return true;
}

bool AwsS3Client::deleteBucket(const std::string &bucketName) {
  Aws::S3::Model::DeleteBucketRequest request;
  request.SetBucket(bucketName);
  Aws::S3::Model::DeleteBucketOutcome outcome = client_.DeleteBucket(request);
  if (!outcome.IsSuccess()) {
    const Aws::S3::S3Error &err = outcome.GetError();
    LOG_ERROR << "Error: deleteBucket: " << err.GetExceptionName() << ":" << err.GetMessage() << '\n';
    return false;
  } else {
    LOG_INFO << "Bucket:" << bucketName << "Delete Success";
  }
  return true;
}

bool AwsS3Client::deleteObject(const std::string &objectKey, const std::string &fromBucket) {
  Aws::S3::Model::DeleteObjectRequest req;
  req.WithKey(objectKey).WithBucket(fromBucket);
  Aws::S3::Model::DeleteObjectOutcome outcome = client_.DeleteObject(req);
  if (!outcome.IsSuccess()) {
    auto err = outcome.GetError();
    LOG_ERROR << "Error: deleteObject: " << err.GetExceptionName() << ":" << err.GetMessage();
    return false;
  }
  LOG_INFO << "Object:" << objectKey << " From " << fromBucket << " Delete Success";
  return true;
}

bool AwsS3Client::getObject(const std::string &objectKey, const std::string &fromBucket) {
  Aws::S3::Model::GetObjectRequest req;
  req.SetBucket(fromBucket);
  req.SetKey(objectKey);
  Aws::S3::Model::GetObjectOutcome outcome = client_.GetObject(req);
  if (!outcome.IsSuccess()) {
    const Aws::S3::S3Error &err = outcome.GetError();
    LOG_ERROR << "Error: getObject: " << err.GetMessage();
    return false;
  }
  LOG_INFO << "getObject:" << objectKey << "Success";
  return true;
}

std::optional<std::vector<Aws::S3::Model::Bucket>> AwsS3Client::listBuckets() {
  auto outcome = client_.ListBuckets();
  if (!outcome.IsSuccess()) {
    LOG_ERROR << "Failed With Error: " << outcome.GetError();
    return std::nullopt;
  }
  LOG_INFO << "listBucket Success";
  return outcome.GetResult().GetBuckets();
  // bucket.GetName()
}

bool AwsS3Client::putObject(const std::string &bucketName, const std::string &fileName) {
  Aws::S3::Model::PutObjectRequest req;
  req.SetBucket(bucketName);
  req.SetKey(fileName);
  std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::FStream>("S3UploadFileStream", fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!*inputData) {
    LOG_ERROR << "Error unable to read file " << fileName;
    return false;
  }
  inputData->seekg(0, std::ios_base::end);
  const auto fileSize = inputData->tellg();
  if (fileSize < 0) {
    LOG_ERROR << "Error unable to get file size " << fileName;
    return false;
  }
  inputData->seekg(0, std::ios_base::beg);
  req.SetContentLength(static_cast<long long>(fileSize));
  req.SetBody(inputData);
  Aws::S3::Model::PutObjectOutcome outcome = client_.PutObject(req);
  if (!outcome.IsSuccess()) {
    const auto &err = outcome.GetError();
    LOG_ERROR << "Error: putObject: " << err.GetExceptionName() << ":" << err.GetMessage();
    return false;
  }
  LOG_INFO << "putObject:" << fileName << "Success";
  return true;
}

bool AwsS3Client::putObject(const std::string &bucketName, const std::string &fileName, const std::string &objectName) {

  Aws::S3::Model::PutObjectRequest req;
  req.SetBucket(bucketName);
  req.SetKey(objectName);
  std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::FStream>("S3UploadFileStream", fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!*inputData) {
    LOG_ERROR << "Error unable to read file " << fileName;
    return false;
  }
  inputData->seekg(0, std::ios_base::end);
  const auto fileSize = inputData->tellg();
  if (fileSize < 0) {
    LOG_ERROR << "Error unable to get file size " << fileName;
    return false;
  }
  inputData->seekg(0, std::ios_base::beg);
  req.SetContentLength(static_cast<long long>(fileSize));
  req.SetBody(inputData);
  Aws::S3::Model::PutObjectOutcome outcome = client_.PutObject(req);
  if (!outcome.IsSuccess()) {
    const auto &err = outcome.GetError();
    LOG_ERROR << "Error: putObject: " << err.GetExceptionName() << ":" << err.GetMessage();
    return false;
  }
  LOG_INFO << "putObject:" << fileName << "Success";
  return true;
}

std::optional<std::vector<Aws::S3::Model::Object>> AwsS3Client::listObjects(const std::string &bucketName) {

  Aws::S3::Model::ListObjectsV2Request req;
  req.WithBucket(bucketName);
  Aws::String continuationToken; // 分页，每次最多返回1000个对象
  Aws::Vector<Aws::S3::Model::Object> allObjects;
  do {
    if (!continuationToken.empty()) {
      req.SetContinuationToken(continuationToken);
    }
    auto outcome = client_.ListObjectsV2(req);
    if (!outcome.IsSuccess()) {
      LOG_ERROR << "Error: listObjects: " << outcome.GetError().GetMessage();
      return std::nullopt;
    } else {
      auto objects = outcome.GetResult().GetContents();
      allObjects.insert(allObjects.end(), objects.begin(), objects.end());
      continuationToken = outcome.GetResult().GetNextContinuationToken();
    }

  } while (!continuationToken.empty());
  LOG_INFO << allObjects.size() << " objects found";
  return allObjects;
}

bool AwsS3Client::putBucketCors(const std::string &bucketName) {
  Aws::S3::Model::PutBucketCorsRequest req;
  req.SetBucket(bucketName);
  Aws::S3::Model::CORSConfiguration corsConfig;
  Aws::S3::Model::CORSRule rule;
  rule.AddAllowedMethods("GET");
  rule.AddAllowedMethods("DELETE");
  rule.AddAllowedMethods("POST");
  rule.AddAllowedMethods("PUT");
  rule.SetMaxAgeSeconds(3000); // 接下来的3000s内，相同的跨域请求不需要再发OPTIONS预检请求了
  rule.AddAllowedOrigins("*"); // 允许所有域名访问（测试用）
  corsConfig.AddCORSRules(rule);
  req.SetCORSConfiguration(corsConfig);
  auto outcome = client_.PutBucketCors(req);
  if (!outcome.IsSuccess()) {
    LOG_INFO << "Error: putBucketCors:" << outcome.GetError().GetMessage();
    return false;
  }
  LOG_INFO << "putBucketCors Success";
  return true;
}
