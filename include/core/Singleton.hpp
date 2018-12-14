#pragma once

#include <cassert>

namespace fui {

template <typename T> class Singleton {
private:
  Singleton(const Singleton<T>&);

  Singleton& operator=(const Singleton<T>&);

  static T* _singleton;

protected:
  Singleton() {
    assert(!_singleton);
    _singleton = static_cast<T*>(this);
  }
  ~Singleton() {
    assert(_singleton);
    _singleton = nullptr;
  }

public:
  static T& instance() {
    assert(_singleton);
    return *_singleton;
  }
};
} // namespace fui