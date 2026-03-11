#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>


namespace OSS {

class Logger {
public:
  // 初始化日志系统（只需调用一次）
  static void init(spdlog::level::level_enum level = spdlog::level::debug) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Windows下解决中文乱码
#endif
    // 创建彩色控制台 logger
    auto console = spdlog::stdout_color_mt("console");

    // 设置日志格式：[时间] [级别] 消息
    console->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    // 设置日志级别
    console->set_level(level);

    // 设置为默认 logger
    spdlog::set_default_logger(console);
  }
};

// 流式日志辅助类
class LogStream {
public:
  LogStream(spdlog::level::level_enum level, const char *file, int line)
      : m_level(level), m_file(file), m_line(line) {}

  ~LogStream() {
    // 析构时输出日志
    std::string msg = m_stream.str();
    switch (m_level) {
    case spdlog::level::trace:
      SPDLOG_TRACE("{} ({}:{})", msg, m_file, m_line);
      break;
    case spdlog::level::debug:
      SPDLOG_DEBUG("{} ({}:{})", msg, m_file, m_line);
      break;
    case spdlog::level::info:
      SPDLOG_INFO("{} ({}:{})", msg, m_file, m_line);
      break;
    case spdlog::level::warn:
      SPDLOG_WARN("{} ({}:{})", msg, m_file, m_line);
      break;
    case spdlog::level::err:
      SPDLOG_ERROR("{} ({}:{})", msg, m_file, m_line);
      break;
    case spdlog::level::critical:
      SPDLOG_CRITICAL("{} ({}:{})", msg, m_file, m_line);
      break;
    default:
      break;
    }
  }

  // 重载 << 操作符，支持所有类型
  template <typename T>
  LogStream &operator<<(const T &value) {
    m_stream << value;
    return *this;
  }

  // 支持 std::endl 等流操作符
  LogStream &operator<<(std::ostream &(*os)(std::ostream &)) {
    m_stream << os;
    return *this;
  }

private:
  std::ostringstream m_stream;
  spdlog::level::level_enum m_level;
  const char *m_file;
  int m_line;
};

// 定义流式日志宏
#define LOG_TRACE OSS::LogStream(spdlog::level::trace, __FILE__, __LINE__)
#define LOG_DEBUG OSS::LogStream(spdlog::level::debug, __FILE__, __LINE__)
#define LOG_INFO OSS::LogStream(spdlog::level::info, __FILE__, __LINE__)
#define LOG_WARN OSS::LogStream(spdlog::level::warn, __FILE__, __LINE__)
#define LOG_ERROR OSS::LogStream(spdlog::level::err, __FILE__, __LINE__)
#define LOG_CRITICAL OSS::LogStream(spdlog::level::critical, __FILE__, __LINE__)

} // namespace OSS