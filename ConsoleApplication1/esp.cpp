#include <Windows.h>
#include <thread>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include "vector.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "esp.h"
inline void DrawBox(float x, float y, float w, float h, float r, float g, float b, float a,
    unsigned int shaderProgram, unsigned int VAO, unsigned int VBO, unsigned int EBO,
    int screenWidth, int screenHeight)
{
    // Convert pixels -> normalized device coordinates (-1 to 1 range)
    float ndcLeft = (x / screenWidth) * 2.0f - 1.0f;
    float ndcRight = ((x + w) / screenWidth) * 2.0f - 1.0f;
    float ndcTop = 1.0f - (y / screenHeight) * 2.0f;
    float ndcBottom = 1.0f - ((y + h) / screenHeight) * 2.0f;

    float vertices[] = {
        ndcLeft,  ndcTop,    0.0f,  // top-left
        ndcRight, ndcTop,    0.0f,  // top-right
        ndcRight, ndcBottom, 0.0f,  // bottom-right
        ndcLeft,  ndcBottom, 0.0f   // bottom-left
    };

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    // Update buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    // Use the shader
    glUseProgram(shaderProgram);

    // (Optional) If your fragment shader has a uniform for color
    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform4f(colorLoc, r, g, b, a);

    // Draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
