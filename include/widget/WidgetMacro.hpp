#pragma once

#include "core/EventMacro.hpp"

#define FUI_WIDGET_PROPERTY(TYPE, NAME)                                                                                \
public:                                                                                                                \
  auto NAME(const TYPE& v)->decltype(this) {                                                                           \
    _##NAME = v;                                                                                                       \
    return this;                                                                                                       \
  }                                                                                                                    \
  const TYPE& NAME() const { return _##NAME; }                                                                         \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME;

#define FUI_WIDGET_READONLY_PROPERTY(TYPE, NAME)                                                                       \
public:                                                                                                                \
  const TYPE& NAME() const { return _##NAME; }                                                                         \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME;

#define FUI_WIDGET_PROPERTY_ALT(TYPE, NAME)                                                                            \
public:                                                                                                                \
  auto NAME(const TYPE& v)->decltype(this);                                                                            \
  TYPE NAME() const;

#define FUI_WIDGET_PROPERTY_ALT_SETTER(TYPE, NAME)                                                                     \
public:                                                                                                                \
  auto NAME(const TYPE& v)->decltype(this);                                                                            \
  const TYPE& NAME() const { return _##NAME; }                                                                         \
                                                                                                                       \
protected:                                                                                                             \
  TYPE _##NAME;

#define FUI_WIDGET_EVENT(PROTOTYPE, EVENT) FUI_DEFINE_EVENT(PROTOTYPE, EVENT)
