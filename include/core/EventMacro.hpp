#pragma once

#include "Signal.hpp"

#define FUI_DEFINE_EVENT(PROTOTYPE, EVENT)                                                                             \
public:                                                                                                                \
  typedef std::function<PROTOTYPE> EVENT##Handler;                                                                     \
  auto on##EVENT(const EVENT##Handler& handler)->decltype(this) {                                                      \
    _signal##EVENT += handler;                                                                                         \
    return this;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
protected:                                                                                                             \
  Signal<PROTOTYPE> _signal##EVENT;
