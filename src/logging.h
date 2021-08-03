#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace shepichess {

inline void initLogging() 
{
  auto stderr_logger = spdlog::stderr_color_mt("shepichess_stderr_logger");
  spdlog::set_default_logger(stderr_logger);
}

} // namespace shepichess
