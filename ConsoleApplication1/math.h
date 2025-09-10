#include "vector.h"
#include <cmath>
#include <iostream>


#pragma once

#define DEG2RAD(x) ((x) * 0.01745329251994329576923690768489)

struct Matrix4x4 {
    float matrix[16];
};

Vector3 Vec3;
 
inline bool WorldToScreen(const Vector3& pos, float viewProj[], int screenWidth, int screenHeight, Vector2& out) {

    float clipw = (viewProj[12] * pos.x) + (viewProj[13] * pos.y) + (viewProj[14] * pos.z) + viewProj[15];

    if (clipw > 0.001f) {
        
        float clipx = (viewProj[0] * pos.x) + (viewProj[1] * pos.y) + (viewProj[2] * pos.z) + viewProj[3];
        float clipy = (viewProj[4] * pos.x) + (viewProj[5] * pos.y) + (viewProj[6] * pos.z) + viewProj[7];



        out.x = (screenWidth / 2) + (screenWidth / 2) * screenWidth / clipw;
        out.y = (screenHeight / 2) - (screenHeight / 2) * screenHeight / clipw;

        
        return true;
    }

    return false;

}


