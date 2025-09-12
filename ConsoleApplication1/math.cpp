#include <cmath>
#include <iostream>
#include <ostream>
#include "math.h"
#define DEG2RAD(x) ((x) * 0.01745329251994329576923690768489)
#pragma once

// Vector 2 Definitions
Vector2::Vector2() : x(0.f), y(0.f) {}
Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}
Vector2 Vector2::operator-(const Vector2& other) {
	return Vector2(x - other.x, y - other.y);
}
Vector2 Vector2::operator+(const Vector2& other) {
	return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::operator/(const float& other) {
	return Vector2(x / other, y / other);
}
float Vector2::operator*(const Vector2& other) {
	return (x * other.x + y * other.y);
}
Vector2 Vector2::operator*(const float& scalar) {
	return Vector2(x * scalar, y * scalar);
}
Vector2 Vector2::normalized(const Vector2& v) {
	float len = sqrtf(v.x * v.x + v.y * v.y);
	if (len > 0.0f) {
		return { v.x / len, v.y / len };
	}
	return { 0.0f, 0.0f }; // zero vector case
}
void Vector2::negate() {
	x = x * -1;
	y = y * -1;
}

/////////////////////////////////////


// Vector3 Definitions
Vector3::Vector3() : x(0.f), y(0.f), z(0.f) {}
Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3 Vector3::operator-(const Vector3& other) {
	return Vector3(x - other.x, y - other.y, z - other.z);
}
Vector3 Vector3::operator+(const Vector3& other) {
	return Vector3(x + other.x, y + other.y, z + other.z);
}
Vector3 Vector3::operator/(const float& other) {
	return Vector3(x / other, y / other, z / other);
}
float Vector3::operator*(const Vector3& other) {
	return (x * other.x + y * other.y + z * other.z);
}
inline Vector3 Vector3::operator*(const float& scalar) {
	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3 Vector3::normalized(const Vector3& v) {
	float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	if (len > 0.0f) {
		return { v.x / len, v.y / len, v.z / len };
	}
	return { 0.0f, 0.0f, 0.0f }; // zero vector case
}
Vector3 Vector3::crossp(const Vector3& a, const Vector3& b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}
void Vector3::negate() {
	x = x * -1;
	y = y * -1;
	z = z * -1;
}
/////////////////////////////////////



// Vector 4 definitions
Vector4::Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
Vector4::Vector4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vector4 Vector4::operator-(const Vector4& other) {
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}
Vector4 Vector4::operator+(const Vector4& other) {
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}
Vector4 Vector4::operator/(const float& other) {
	return Vector4(x / other, y / other, z / other, w / other);
}
float Vector4::operator*(const Vector4& other) {
	return (x * other.x + y * other.y + z * other.z, w * other.w);
}
Vector4 Vector4::operator*(const float& scalar) {
	return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}
void Vector4::negate() {
	x = x * -1;
	y = y * -1;
	z = z * -1;
	w = w * -1;
}
/////////////////////////////////////


// Matrix function definitions
Vector4 Matrix4x4::operator*(const Vector4& v) const {
	Vector4 r;
	r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return r;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& b) const {
	Matrix4x4 result(0.0f); // Has to start off as zero matrix
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			for (int i = 0; i < 4; ++i)
				result.m[row][col] += m[row][i] * b.m[i][col];
	return result;
}

Matrix4x4::Matrix4x4(float* pointerToMemoryMatrix) {
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			this->m[row][col] = *(pointerToMemoryMatrix + row * 4 + col);
}


Matrix4x4::Matrix4x4() {
	for (int i = 0; i < 4; i++)
		m[i][i] = 1.0f;
}

Matrix4x4::Matrix4x4(float value) { // zero if value == 0
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = value;
}
/////////////////////////////////////


bool WorldToScreen(const Vector4& pos, const Matrix4x4& viewProj, Vector2& out) {
	Vector4 clip = viewProj * pos;

	if (clip.w <= 0.001f) return false; // Behind the camera
	// U gotta check wether its smaller then 0 because everything that is infront of u means z > 0, and because w = -z 
	// that turns around the expression to everything that is infront of u has a negative z value so z < 0  and therefore w < 0
	out.x = (float)(clip.x / clip.w);
	out.y = (float)(clip.y / clip.w);

	return true;
}


// For printing all of this stuff
std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "Vec2(" << v.x << ", " << v.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector4& v) {
    os << "Vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}
std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix) {
    os << "[" << matrix.m[0][0] << "][" << matrix.m[0][1] << "][" << matrix.m[0][2] << "][" << matrix.m[0][3] << "]" << std::endl
        << "[" << matrix.m[1][0] << "][" << matrix.m[1][1] << "][" << matrix.m[1][2] << "][" << matrix.m[1][3] << "]" << std::endl
        << "[" << matrix.m[2][0] << "][" << matrix.m[2][1] << "][" << matrix.m[2][2] << "][" << matrix.m[2][3] << "]" << std::endl
        << "[" << matrix.m[3][0] << "][" << matrix.m[3][1] << "][" << matrix.m[3][2] << "][" << matrix.m[3][3] << "]";
    return os;
}