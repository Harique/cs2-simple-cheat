#include "esp.h"

void DrawBox(Vector3 corners[4], float r, float g, float b, float a,
    unsigned int shaderProgram, unsigned int VAO, unsigned int VBO, unsigned int EBO)
{
    unsigned int indices[] = { 0, 1, 2, 1, 2, 3 };

    // Update buffers
    glDisable(GL_DEPTH_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3) * 4, reinterpret_cast<void*>(corners));

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
