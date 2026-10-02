#include "Scene.h"
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace rendrx;

Scene::Scene() {
    std::cout << "Initializing" << std::endl;

    if (!glfwInit()) {
        std::cerr << "Failed to init glfw, aborting!" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "rendrX demonstration", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window, aborting!" << std::endl;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
    }
}

void Scene::init() {
    // Compile the shaders at runtime
    // idk bout how ur supposed to do it in prod yet
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();

    if (shaderProgram == 0) {
        std::cerr << "[Shader] ERROR: Failed to create shader program.\n";
        std::exit(-1);
    } else {
        std::cout << "[Shader] Created shader program. ID: " << shaderProgram
                  << '\n';
    }

    glAttachShader(shaderProgram, vertexShader);

    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "[Shader] ERROR: Failed to attach vertex shader. ID: "
                  << vertexShader << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Attached vertex shader. ID: " << vertexShader
                  << '\n';
    }

    glAttachShader(shaderProgram, fragmentShader);

    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "[Shader] ERROR: Failed to attach fragment shader. ID: "
                  << fragmentShader << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Attached fragment shader. ID: " << fragmentShader
                  << '\n';
    }

    std::cout << "[Shader] Linking program " << shaderProgram << "...\n";

    glLinkProgram(shaderProgram);

    GLint success = GL_FALSE;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);

        std::cerr << "[Shader] Program linking FAILED.\n";
        std::cerr << "[Shader] Program ID: " << shaderProgram << '\n';
        std::cerr << "[Shader] Linker log:\n" << infoLog << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Program linked successfully.\n";
        std::cout << "[Shader] Program ID: " << shaderProgram << '\n';
    }

    glUseProgram(shaderProgram);

    glUniform3f(glGetUniformLocation(shaderProgram, "fogColor"), 0.6f, 0.7f,
                0.8f);

    glUniform1f(glGetUniformLocation(shaderProgram, "fogStart"), 100.0f);

    glUniform1f(glGetUniformLocation(shaderProgram, "fogEnd"), 200.0f);

    // init
    modelLocation = glGetUniformLocation(shaderProgram, "model");

    viewLocation = glGetUniformLocation(shaderProgram, "view");

    projectionLocation = glGetUniformLocation(shaderProgram, "projection");

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1, 0.2, 0.3, 1.0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Scene::loadTextures() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data =
        stbi_load("texture.png", &width, &height, &channels, 0);

    if (data) {
        GLenum format = channels == 4 ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glUseProgram(shaderProgram);

    int textureLocation = glGetUniformLocation(shaderProgram, "texture1");

    glUniform1i(textureLocation, 0);

    stbi_image_free(data);
}

void Scene::addWorld(World &world) { this->world = &world; }

void Scene::launch() {
    lastFrame = glfwGetTime();
    this->init();
    this->loadTextures();
}

bool Scene::isChunkVisible(glm::ivec2 chunkPos,
                           const glm::mat4 &viewProjection) {

    constexpr float chunkSize = 16.0f;
    constexpr float chunkHeight = 256.0f;

    float minX = chunkPos.x * chunkSize;
    float minZ = chunkPos.y * chunkSize;

    glm::vec3 min{minX, 0.0f, minZ};

    glm::vec3 max{minX + chunkSize, chunkHeight, minZ + chunkSize};

    // extract frustum planes from projection matrix.
    glm::vec4 planes[6];

    planes[0] = glm::vec4(viewProjection[0][3] + viewProjection[0][0],
                          viewProjection[1][3] + viewProjection[1][0],
                          viewProjection[2][3] + viewProjection[2][0],
                          viewProjection[3][3] + viewProjection[3][0]); // Left

    planes[1] = glm::vec4(viewProjection[0][3] - viewProjection[0][0],
                          viewProjection[1][3] - viewProjection[1][0],
                          viewProjection[2][3] - viewProjection[2][0],
                          viewProjection[3][3] - viewProjection[3][0]); // Right

    planes[2] =
        glm::vec4(viewProjection[0][3] + viewProjection[0][1],
                  viewProjection[1][3] + viewProjection[1][1],
                  viewProjection[2][3] + viewProjection[2][1],
                  viewProjection[3][3] + viewProjection[3][1]); // Bottom

    planes[3] = glm::vec4(viewProjection[0][3] - viewProjection[0][1],
                          viewProjection[1][3] - viewProjection[1][1],
                          viewProjection[2][3] - viewProjection[2][1],
                          viewProjection[3][3] - viewProjection[3][1]); // Top

    planes[4] = glm::vec4(viewProjection[0][3] + viewProjection[0][2],
                          viewProjection[1][3] + viewProjection[1][2],
                          viewProjection[2][3] + viewProjection[2][2],
                          viewProjection[3][3] + viewProjection[3][2]); // Near

    planes[5] = glm::vec4(viewProjection[0][3] - viewProjection[0][2],
                          viewProjection[1][3] - viewProjection[1][2],
                          viewProjection[2][3] - viewProjection[2][2],
                          viewProjection[3][3] - viewProjection[3][2]); // Far

    // Test AABB against each plane.
    for (glm::vec4 plane : planes) {

        glm::vec3 normal{plane.x, plane.y, plane.z};

        // find the AABB vertex furthest in the directio of the plane normal.
        glm::vec3 positive{normal.x >= 0.0f ? max.x : min.x,
                           normal.y >= 0.0f ? max.y : min.y,
                           normal.z >= 0.0f ? max.z : min.z};

        // out of plane = gett banned gg ez
        if (glm::dot(normal, positive) + plane.w < 0.0f)
            return false;
    }

    return true;
}

void Scene::render() {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    fpsTimer += deltaTime;
    frameCount++;

    if (fpsTimer >= 1.0f) {
        fps = frameCount / fpsTimer;

        std::cout << "FPS: " << fps << '\n';

        fpsTimer = 0.0f;
        frameCount = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraRot.z -= 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraRot.z += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraRot.x += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraRot.x -= 1.0f;
    }

    glm::vec3 direction;

    float pitch = glm::radians(cameraRot.x);
    float yaw = glm::radians(cameraRot.z);

    direction.x = cos(yaw) * cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw) * cos(pitch);

    direction = glm::normalize(direction);

    glm::vec3 forward =
        glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
    glm::vec3 right =
        glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    prevCameraPos = cameraPos;
    float speed = 20.0f;
    float movement = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += forward * movement;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= forward * movement;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= right * movement;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += right * movement;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos.y += movement;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPos.y -= movement;
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Matricies
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));

    // Look at the triangle at origin ALWAYS
    glm::mat4 view = glm::lookAt(cameraPos,                  // pos
                                 cameraPos + direction,      // direction
                                 glm::vec3(0.0f, 1.0f, 0.0f) // up
    );

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), // FOV
                                            1280.0f / 800.0f,    // aspect ratio
                                            0.1f,                // near
                                            500.0f               // far
    );

    glm::mat4 viewProjection = projection * view;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // configure matricies
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

    for (auto &[position, chunk] : world->chunks) {
        if (!isChunkVisible(position, viewProjection)) {
            continue;
        }
        chunk->draw();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Scene::shouldClose() { return glfwWindowShouldClose(this->window); }
