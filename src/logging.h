#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

inline void initLogging()
{
  auto stderr_logger = spdlog::stderr_color_mt("shepichess_stderr_logger");
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_default_logger(stderr_logger);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [thread %t] [%^%l%$] [%s:%#] %v");
  SPDLOG_DEBUG("Created logger with level: {}", SPDLOG_ACTIVE_LEVEL);
}
