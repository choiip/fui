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

// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
inline int isBlack(Color col) { return (col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f) ? 1 : 0; }

} // namespace fui
