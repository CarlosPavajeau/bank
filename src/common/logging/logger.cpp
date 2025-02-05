#include "logger.h"

std::mutex logger::mutex;

std::string logger::get_current_timestamp()
{
  const auto now = std::chrono::system_clock::now();
  const auto now_time_t = std::chrono::system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_time_t);

  return std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}",
                     now_tm.tm_year + 1900,
                     now_tm.tm_mon + 1,
                     now_tm.tm_mday,
                     now_tm.tm_hour,
                     now_tm.tm_min,
                     now_tm.tm_sec);
}

std::string logger::log_level_to_string(const log_level level)
{
  switch (level) {
    case log_level::debug:
      return "DEBUG";
    case log_level::info:
      return "INFO";
    case log_level::warning:
      return "WARNING";
    case log_level::error:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}
