#pragma once

#include "rendrx/World.h"
#define GLFW_INCLUDE_NONE
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rendrx {
class Scene {
  private:
    GLFWwindow *window;
    unsigned int vertexShader, fragmentShader, shaderProgram, texture;

    float deltaTime;
    float lastFrame;

    glm::vec3 prevCameraPos = glm::vec3(0.0f, 120.0f, 3.0f);
    glm::vec3 cameraPos = prevCameraPos;
    glm::vec3 cameraRot = glm::vec3(0.0f, 0.0f, 55.0f);
    float fpsTimer = 0.0f;
    int frameCount = 0;
    float fps = 0.0f;

    World *world = nullptr;

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
    bool isChunkVisible(glm::ivec2 chunkPos, const glm::mat4 &viewProjection);

  public:
    Scene();
    void addWorld(World &world);
    bool shouldClose();
    void launch();
    void render();

    const glm::vec3 &getCameraPos() const { return cameraPos; }
    const glm::vec3 &getPrevCameraPos() const { return prevCameraPos; }
    const glm::vec3 &getCameraRot() const { return cameraRot; }
};
} // namespace rendrx
