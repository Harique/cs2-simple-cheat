#include "vector.h"
#include <cmath>
#include <iostream>


#pragma once

#define DEG2RAD(x) ((x) * 0.01745329251994329576923690768489)
class Matrix4x4 {
public:
    float m[4][4] = { 0 };

    static Matrix4x4 Identity() {
        Matrix4x4 mat;
        for (int i = 0; i < 4; i++)
            mat.m[i][i] = 1.0f;
        return mat;
    }

    Vector4 operator*(const Vector4& v) const {
        Vector4 r;
        r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
        r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
        r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
        r.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
        return r;
    }

    Matrix4x4 operator*(const Matrix4x4& b) const {
        Matrix4x4 result = {};
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col)
                for (int i = 0; i < 4; ++i)
                    result.m[row][col] += m[row][i] * b.m[i][col];
        return result;
    }
};
Vector3 Vec3;
inline Matrix4x4 viewMatrix(Vector3& eye, float& yaw, float& pitch) {
    float yawRad = DEG2RAD(yaw);
    float pitchRad = DEG2RAD(pitch);

    Vector3 fakeUp = { 0.f, 1.f, 0.f };

    Vector3 forward = Vec3.normalized({
        cos(yawRad) * cos(pitchRad),
        sin(pitchRad),
        sin(yawRad) * cos(pitchRad)
    });
    forward.negate();
    Vector3 right = Vec3.normalized(Vec3.crossp(forward, fakeUp)); // since directX gives LH vectors but we need RH for openGL, we flip the order
    Vector3 up = Vec3.normalized(Vec3.crossp(right, forward));

    Matrix4x4 view = Matrix4x4::Identity();
    view.m[0][0] = right.x;   view.m[0][1] = right.y;   view.m[0][2] = right.z;   view.m[0][3] = -(right.x * eye.x + right.y * eye.y + right.z * eye.z);
    view.m[1][0] = up.x;      view.m[1][1] = up.y;      view.m[1][2] = up.z;      view.m[1][3] = -(up.x * eye.x + up.y * eye.y + up.z * eye.z);
    view.m[2][0] = forward.x; view.m[2][1] = forward.y;  view.m[2][2] = forward.z; view.m[2][3] = -(forward.x * eye.x + forward.y * eye.y + forward.z * eye.z);
    return view;

}
inline Matrix4x4 getProjectionMatrix(float& fovDegrees, float& aspect) {
    //FarZ is the furthest range of rendered objects
    //NearZ is the closest thing to the camera, basically 0
    //aspect is aspect ratio with width/height
    //fov is hard coded to be 90 in game
    float farZ = 100.f;
    float nearZ = 0.1f;
    float fovRad = DEG2RAD(fovDegrees);
    float f = 1.0f / tan(fovRad / 2.0f);
    //bunch of math that i dont understand but memorized, ask max for explanation later.
    Matrix4x4 mat = {};
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = -(farZ + nearZ) / (farZ - nearZ);
    mat.m[2][3] = -(2 * farZ * nearZ) / (farZ - nearZ);
    mat.m[3][2] = -1.0f;
    mat.m[3][3] = 0.f;
    return mat;
}
inline bool WorldToScreen(const Vector3& pos,const Matrix4x4& viewProj,int screenWidth, int screenHeight,Vector2& out){

    Vector4 clip = viewProj * Vector4{ pos.x, pos.y, pos.z, 1.0f };

    if (clip.w <= 0.0f) return false; // Behind camera

    float ndcX = clip.x / clip.w;
    float ndcY = clip.y / clip.w;

    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f)
        return false; // Outside screen

    out.x = (screenWidth * 0.5f * ndcX) + (ndcX + screenWidth * 0.5f);
    out.y = (screenHeight * 0.5f * ndcY) + (ndcY + screenHeight * 0.5f);
    return true;
}


