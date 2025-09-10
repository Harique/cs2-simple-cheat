#include <cmath>
#include <ostream>
#pragma once

class Vector2 {
public:
	float x;
	float y;
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Vector2 operator-(Vector2& other) {
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator+(Vector2& other) {
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator/(float& other) {
		return Vector2(x / other, y / other);
	}
	float operator*(Vector2& other) {
		return (x * other.x + y * other.y);
	}
	Vector2 operator*(float& scalar) {
		return Vector2(x * scalar, y * scalar);
	}
	Vector2 normalized(const Vector2& v) {
		float len = sqrtf(v.x * v.x + v.y * v.y);
		if (len > 0.0f) {
			return { v.x / len, v.y / len};
		}
		return { 0.0f, 0.0f}; // zero vector case
	}
	void negate();

};

class Vector3 {
public:
	float x;
	float y;
	float z;
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3 operator-(Vector3& other) {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator+(Vector3& other) {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator/(float& other) {
		return Vector3(x / other, y / other, z / other);
	}
	float operator*(Vector3& other) {
		return (x * other.x + y * other.y + z * other.z);
	}
	Vector3 operator*(float& scalar) {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}
	Vector3 normalized(const Vector3& v) {
		float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		if (len > 0.0f) {
			return { v.x / len, v.y / len, v.z / len };
		}
		return { 0.0f, 0.0f, 0.0f }; // zero vector case
	}
	Vector3 crossp(const Vector3& a, const Vector3& b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
	void negate();

};
class Vector4 {
public:
	float x;
	float y;
	float z;
	float w;
	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
	Vector4(float x, float y, float z,float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Vector4 operator-(Vector4& other) {
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	Vector4 operator+(Vector4& other) {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	Vector4 operator/(float& other) {
		return Vector4(x / other, y / other, z / other, w / other);
	}
	float operator*(Vector4& other) {
		return (x * other.x + y * other.y + z * other.z, w * other.w);
	}
	Vector4 operator*(float& scalar) {
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}
	void negate();

};
