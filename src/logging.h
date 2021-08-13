#pragma once

#include <mutex>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace shepichess {

using LogLevel = spdlog::level::level_enum;
constexpr LogLevel kDefaultLogLevel = static_cast<LogLevel>(SPDLOG_ACTIVE_LEVEL);

inline void initLogging()
{
  static std::once_flag init_flag;
  std::call_once(init_flag, []() {
    static auto stderr_logger = spdlog::stderr_color_mt("shepichess_stderr_logger");
    stderr_logger->set_level(kDefaultLogLevel);
    spdlog::set_level(kDefaultLogLevel);
    spdlog::set_default_logger(stderr_logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [thread %t] [%^%l%$] [%s:%#] %v");
  });
}

} // namespace shepichess