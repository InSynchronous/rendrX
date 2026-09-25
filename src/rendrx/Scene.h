#pragma once

#include "rendrx/Triangle.h"
#include <vector>
#define GLFW_INCLUDE_NONE
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace rendrx {
class Scene {
  private:
    GLFWwindow *window;
    unsigned int vertexShader, fragmentShader, shaderProgram, texture, VBO, VAO;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraRot = glm::vec3(0.0f, 0.0f, -90.0f);

    std::vector<Triangle> triangles;
    /*
       Vertex shader I wrote. takes in a vector3 of a vertex's position, returns
       the same thing.
    */
    const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 vertData;
        layout (location = 1) in vec2 uvData;
    
        out vec2 uvCoord;
    
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(vertData, 1.0);
            uvCoord = uvData;
        }
    )";

    /*
    Fragment shader code, called on each pixel draw thats in the triangle.
    returns texture lookup result
    */

    const char *fragmentShaderSource = R"(
        #version 330 core
    
        in vec2 uvCoord;
        out vec4 FragColor;
    
        uniform sampler2D texture1;
    
        void main() {
            FragColor = texture(texture1, uvCoord);
        }
    )";

    void init();
    void loadTextures();
    void uploadTriangles();

  public:
    void addTriangle(Triangle t);
    bool shouldClose();
    void launch();
    void render();
};
} // namespace rendrx
