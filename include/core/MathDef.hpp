#pragma once

namespace fui {

struct Color {
  union {
    struct {
      float r, g, b, a;
    };
    struct {
      float x, y, z, w;
    };
  };
};
struct ConstantColor {
  constexpr static Color Black = {0, 0, 0, 0};
};

template <typename T> struct Vector2 { T x, y; };
typedef Vector2<int> Vector2i;
template <typename T> struct ConstantVector2 {
  constexpr static Vector2<T> Zero = {0, 0};
};
typedef ConstantVector2<int> ConstantVector2i;

template <typename T> constexpr Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T> constexpr Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T> struct Rect { 
  union {
    struct {
      T x, y, w, h;
    };
    struct {
      Vector2<T> position, size;
    };
  };
};
typedef Rect<int> Recti;

} // namespace fui
