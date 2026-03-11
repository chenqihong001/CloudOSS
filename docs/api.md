# 管理台 API 设计（Drogon）

> 说明：本 API 为管理面能力（账号/权限/统计/日志等）。桶与对象的数据面操作由前端通过 **aws-cpp-sdk** 直连 radosgw 的 S3 兼容接口完成；管理面仅提供必要的配置与业务数据。

## 1. 基本约定
- **Base URL**：`/api/v1`
- **鉴权**：除登录/注册/找回密码外，其它接口需 `Authorization: Bearer <token>`
- **响应结构（建议统一）**
  ```json
  {
    "success": true,
    "data": {},
    "errors": ""
  }
  ```
  - 兼容说明：当前客户端登录失败读取 `error` 字段，后端可临时同时返回 `error` 与 `errors`。

- **分页参数**：`page`, `pageSize` 或 `pageToken`, `limit`
- **时间字段**：统一使用秒级时间戳或 ISO8601（保持一致即可）

## 2. 认证与账号
### 2.1 登录
- **POST** `/auth/login`
- **Request**
  ```json
  {"username":"demo","password":"123456"}
  ```
- **Response**
  ```json
  {
    "success": true,
    "data": {
      "token": "jwt-token",
      "tokenExpiresAtSec": 1700000000,
      "user": {"id":"u1","username":"demo","email":"a@b.com"}
    }
  }
  ```

### 2.2 注册
- **POST** `/auth/register`
- **Request**
  ```json
  {"username":"demo","password":"123456","email":"a@b.com"}
  ```

### 2.3 刷新 Token
- **POST** `/auth/refresh-token`
- **Request**
  ```json
  {"refreshToken":"jwt-token"}
  ```

### 2.4 发送验证码（忘记密码）
- **POST** `/auth/send-authcode`
- **Request**
  ```json
  {"email":"a@b.com"}
  ```

### 2.5 重置密码
- **POST** `/auth/forgot-password`
- **Request**
  ```json
  {"email":"a@b.com","authCode":"123456","newPassword":"newpwd"}
  ```

## 3. 用户与 Access Key
### 3.1 当前用户信息
- **GET** `/users/me`

### 3.2 更新用户信息
- **PUT** `/users/me`

### 3.3 Access Key 列表
- **GET** `/keys`

### 3.4 创建 Access Key
- **POST** `/keys`
- **Response**
  ```json
  {"success":true,"data":{"ak":"AK...","sk":"SK..."}}
  ```

### 3.5 禁用/删除 Access Key
- **POST** `/keys/{keyId}/disable`
- **DELETE** `/keys/{keyId}`

## 4. 桶（Bucket）管理（管理面）
### 4.1 桶列表
- **GET** `/buckets`

### 4.2 创建桶
- **POST** `/buckets`
- **Request**
  ```json
  {"name":"my-bucket","acl":"private","quotaBytes":10737418240}
  ```

### 4.3 桶详情
- **GET** `/buckets/{bucket}`

### 4.4 更新桶配置
- **PUT** `/buckets/{bucket}`
- **Request**
  ```json
  {"acl":"private","versioning":true,"corsRules":[],"lifecycleRules":[]}
  ```

### 4.5 删除桶
- **DELETE** `/buckets/{bucket}`

## 5. 对象（Object）管理（可选管理面）
> 数据面通过 aws-cpp-sdk 直接调用 S3 接口完成；以下接口仅在需要服务端统一控制或记录元数据时启用。

### 5.1 对象列表
- **GET** `/buckets/{bucket}/objects?prefix=&limit=&pageToken=`

### 5.2 对象元数据
- **GET** `/buckets/{bucket}/objects/meta?key=xxx`

### 5.3 预签名上传/下载
- **POST** `/buckets/{bucket}/objects/presign-upload`
- **POST** `/buckets/{bucket}/objects/presign-download`
- **Request**
  ```json
  {"key":"path/file.txt","expiresInSec":900,"contentType":"text/plain"}
  ```
- **Response**
  ```json
  {"success":true,"data":{"url":"https://...","method":"PUT","headers":{}}}
  ```

### 5.4 分片上传（Multipart）
- **POST** `/buckets/{bucket}/objects/initiate-multipart`
- **POST** `/buckets/{bucket}/objects/complete-multipart`
- **POST** `/buckets/{bucket}/objects/abort-multipart`

### 5.5 批量删除
- **DELETE** `/buckets/{bucket}/objects`
- **Request**
  ```json
  {"keys":["a.txt","b.txt"]}
  ```

## 6. 分享链接
### 6.1 生成分享链接
- **POST** `/share-links`
- **Request**
  ```json
  {"bucket":"my-bucket","key":"a.txt","expiresInSec":3600}
  ```

### 6.2 分享链接列表
- **GET** `/share-links`

### 6.3 撤销分享链接
- **DELETE** `/share-links/{id}`

## 7. 用量与统计
### 7.1 概览统计
- **GET** `/usage/summary?from=2026-02-01&to=2026-02-27`

### 7.2 按桶统计
- **GET** `/usage/buckets?bucket=my-bucket&from=&to=`

## 8. 审计与日志
- **GET** `/audit/logs?type=login&from=&to=&page=&pageSize=`

## 9. 错误码建议（可扩展）
- `AUTH_INVALID`：账号或密码错误
- `AUTH_EXPIRED`：Token 过期
- `BUCKET_EXISTS`：桶已存在
- `BUCKET_NOT_FOUND`：桶不存在
- `OBJECT_NOT_FOUND`：对象不存在
- `QUOTA_EXCEEDED`：配额不足
