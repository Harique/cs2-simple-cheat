#include "vector.h"
#include <iostream>
#include <numbers>
inline void Vector2::negate() {
    x = x * -1;
    y = y * -1;
    return;
}

inline void Vector3::negate() {
    x = x * -1;
    y = y * -1;
    z = z * -1;
    return;
}

inline void Vector4::negate() {
    x = x * -1;
    y = y * -1;
    z = z * -1;
    w = w * -1;
    return;
}
inline float to_degrees(const float radians ) {
    return radians * 180.f / std::numbers::pi_v<float>;
}