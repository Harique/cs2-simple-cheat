#include <Windows.h>
#include <thread>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "math.h"
#pragma once

void DrawBox(Vector3 corners[4], float r, float g, float b, float a,
    unsigned int shaderProgram, unsigned int VAO, unsigned int VBO, unsigned int EBO);