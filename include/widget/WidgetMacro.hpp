#pragma once

#include "core/EventMacro.hpp"

#define FUI_WIDGET_PROPERTY(TYPE, NAME, ...)                                                                           \
public:                                                                                                                \
  auto NAME(TYPE const& v)->decltype(this) {                                                                           \
    _##NAME = v;                                                                                                       \
    return this;                                                                                                       \
  }                                                                                                                    \
  TYPE const& NAME() const { return _##NAME; }                                                                         \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME = __VA_ARGS__;

#define FUI_WIDGET_READONLY_PROPERTY(TYPE, NAME, ...)                                                                  \
public:                                                                                                                \
  TYPE const& NAME() const { return _##NAME; }                                                                         \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME = __VA_ARGS__;

#define FUI_WIDGET_WRITEONLY_PROPERTY(TYPE, NAME, ...)                                                                           \
public:                                                                                                                \
  auto NAME(TYPE const& v)->decltype(this) {                                                                           \
    _##NAME = v;                                                                                                       \
    return this;                                                                                                       \
  }                                                                                                                    \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME = __VA_ARGS__;

#define FUI_WIDGET_PROPERTY_ALT(TYPE, NAME)                                                                            \
public:                                                                                                                \
  auto NAME(TYPE const& v)->decltype(this);                                                                            \
  TYPE NAME() const;

#define FUI_WIDGET_PROPERTY_ALT_SETTER(TYPE, NAME, ...)                                                                \
public:                                                                                                                \
  template <typename... Args> auto NAME(Args... args)->decltype(this) {                                 \
    return _##NAME##Setter(std::forward<Args>(args)...);                                                            \
  }                                                                                                                    \
  TYPE const& NAME() const { return _##NAME; }                                                                         \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME = __VA_ARGS__;

#define FUI_WIDGET_EVENT(PROTOTYPE, EVENT) FUI_DEFINE_EVENT(PROTOTYPE, EVENT)
