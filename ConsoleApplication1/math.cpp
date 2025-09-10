#include "vector.h"

void Vector2::negate() {
    x = x * -1;
    y = y * -1;
    return;
}

void Vector3::negate() {
    x = x * -1;
    y = y * -1;
    z = z * -1;
    return;
}

void Vector4::negate() {
    x = x * -1;
    y = y * -1;
    z = z * -1;
    w = w * -1;
    return;
}