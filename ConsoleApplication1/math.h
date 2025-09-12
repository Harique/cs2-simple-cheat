#include <cmath>
#include <iostream>
#include <ostream>
#define DEG2RAD(x) ((x) * 0.01745329251994329576923690768489)
#pragma once

struct Vector2 {
	float x;
	float y;
	Vector2();
	Vector2(float x, float y);
	Vector2 operator-(const Vector2& other);
	Vector2 operator+(const Vector2& other);
	Vector2 operator/(const float& other);
	float operator*(const Vector2& other);
	Vector2 operator*(const float& scalar);
	Vector2 normalized(const Vector2& v);
	void negate();

};

struct Vector3 {
	float x;
	float y;
	float z;
	Vector3();
	Vector3(float x, float y, float z);
	Vector3 operator-(const Vector3& other);
	Vector3 operator+(const Vector3& other);
	Vector3 operator/(const float& other);
	float operator*(const Vector3& other);
	inline Vector3 operator*(const float& scalar);
	Vector3 normalized(const Vector3& v);
	Vector3 crossp(const Vector3& a, const Vector3& b);
	void negate();

};
struct Vector4 {
	float x;
	float y;
	float z;
	float w;
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4 operator-(const Vector4& other);
	Vector4 operator+(const Vector4& other);
	Vector4 operator/(const float& other);
	float operator*(const Vector4& other);
	Vector4 operator*(const float& scalar);
	void negate();

};


struct Matrix4x4 {
    float m[4][4] = { 0 };

	Vector4 operator*(const Vector4& v) const;
	Matrix4x4 operator*(const Matrix4x4& b) const;
	Matrix4x4(float* pointerToMemoryMatrix);
	Matrix4x4();
	Matrix4x4(float value);

};
    
bool WorldToScreen(const Vector4& pos, const Matrix4x4& viewProj, Vector2& out);

std::ostream& operator<<(std::ostream& os, const Vector2& v);
std::ostream& operator<<(std::ostream& os, const Vector3& v);
std::ostream& operator<<(std::ostream& os, const Vector4& v);
std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix);