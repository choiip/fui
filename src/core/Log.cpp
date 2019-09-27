#include "core/Log.hpp"
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <memory>
#include "nanovg/nanovg.h"

/* FOREGROUND */
#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

namespace fui {

template <Logger::Level LOGLEVEL> static int logFunction(const char* fmt, ...) {
  std::unique_ptr<char[]> formattedHeapMem;
  char formattedStackMem[128];
  char* formatted = &formattedStackMem[0];
  int finalWritten = -1, n = sizeof(formattedStackMem);
  va_list ap;
  for (;;) {
    va_start(ap, fmt);
    finalWritten = vsnprintf(formatted, n, fmt, ap);
    va_end(ap);
    if (finalWritten < 0 || finalWritten >= n) {
      n += abs(finalWritten - n + 1);
      formattedHeapMem.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
      formatted = &formattedHeapMem[0];
    } else {
      break;
    }
  }
  fui::Logger::instance().write(LOGLEVEL, std::string(formatted, finalWritten));

  return finalWritten;
}

Logger::Logger() {
  _handler = ([](Level level, const std::string& message) {
    switch (level) {
    case Level::ERR: std::cerr << KRED << message << RST << '\n'; break;
    case Level::WARN: std::cout << KYEL << message << RST << '\n'; break;
    case Level::DBG: std::cout << KBLU << message << RST << '\n'; break;
    default: std::cout << message << '\n'; break;
    }
  });
  nvgErrorPrint = logFunction<Level::ERR>;
  nvgDebugPrint = logFunction<Level::DBG>;
}

Logger::~Logger() {
  nvgDebugPrint = printf;
  nvgErrorPrint = printf;
}

void Logger::write(Level level, const std::string& message) { (*_handler)(level, message); }

void Logger::setHandle(Logger::Handler handler) {
  if (handler != nullptr) { _handler = handler; }
}

} // namespace fui
