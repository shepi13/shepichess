#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace shepichess {

inline void initLogging() 
{
  static auto stderr_logger = spdlog::stderr_color_mt("shepichess_stderr_logger");
  // Use #define SPDLOG_ACTIVE_LEVEL instead of these functions to change default level
  stderr_logger->set_level(spdlog::level::trace);
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_default_logger(stderr_logger);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [thread %t] [%^%l%$] [%s:%#] %v");
}

} // namespace shepichess