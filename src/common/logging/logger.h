#ifndef LOGGER_H
#define LOGGER_H

#include <format>
#include <iostream>
#include <mutex>
#include <string>

enum class log_level
{
  debug,
  info,
  warning,
  error
};

class logger
{
public:
  template<typename... Args>
  static void log(const log_level level,
                  const std::string_view format_str,
                  Args&&... args)
  {
    std::lock_guard lock(mutex);

    auto timestamp = get_current_timestamp();
    auto formatted_message = std::vformat(
        format_str, std::make_format_args(std::forward<Args>(args)...));

    auto log_message = std::format("[{}] [{}] {}",
                                   timestamp,
                                   log_level_to_string(level),
                                   formatted_message);

    std::cout << log_message << std::endl;
  }

private:
  static std::mutex mutex;

  static std::string get_current_timestamp();
  static std::string log_level_to_string(log_level level);
};

// Logging macros
#define LOG_DEBUG(format, ...) \
  logger::log(log_level::debug, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) \
  logger::log(log_level::info, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) \
  logger::log(log_level::warning, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) \
  logger::log(log_level::error, format, ##__VA_ARGS__)

#endif  // LOGGER_H
