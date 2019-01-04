#pragma once

#include <sstream>
#include "core/Singleton.hpp"

namespace fui {

class Logger : public Singleton<Logger> {
public:
  enum class Level {
    ERROR,
    WARN,
    DEBUG,
  };

  typedef void (* Handler)(Level level, const std::string& message);

  Logger();
  ~Logger();
  void write(Level level, const std::string& message);
  void setHandle(Handler handler);

private:
  Handler _handler;
};

template<Logger::Level LOGLEVEL>
class LogStream : public std::ostringstream {
public:
  explicit LogStream(Logger& logger) : _logger(logger) {}
  ~LogStream() { _logger.write(LOGLEVEL, std::ostringstream::str()); }
  template <typename T>
  LogStream& operator<<(const T& v) {
    (*(std::ostringstream*)this) << v;
    return *this;
  }
  LogStream& operator()() {
    return *this;
  }
private:
  Logger& _logger;
};

}

#define LOGE fui::LogStream<fui::Logger::Level::ERROR>(fui::Logger::instance())()
#define LOGW fui::LogStream<fui::Logger::Level::WARN>(fui::Logger::instance())()
#define LOGD fui::LogStream<fui::Logger::Level::DEBUG>(fui::Logger::instance())()
