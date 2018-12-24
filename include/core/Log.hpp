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
  void log(Level level, const std::string& message);
  void setHandle(Handler handler);

private:
  Handler _handler;
};

template<Logger::Level LOGLEVEL>
class LogStream : public std::ostringstream {
public:
  LogStream(Logger& logger) : _logger(logger) {}
  ~LogStream() { _logger.log(LOGLEVEL, std::ostringstream::str()); }
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

#define LOGE LogStream<Logger::Level::ERROR>(Logger::instance())()
#define LOGW LogStream<Logger::Level::WARN>(Logger::instance())()
#define LOGD LogStream<Logger::Level::DEBUG>(Logger::instance())()
