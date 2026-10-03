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
    geometryVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(geometryVertexShader, 1, &geometryVertexShaderSource, NULL);
    glCompileShader(geometryVertexShader);

    geometryFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(geometryFragmentShader, 1, &geometryFragmentShaderSource,
                   NULL);
    glCompileShader(geometryFragmentShader);

    geometryShader = glCreateProgram();

    if (geometryShader == 0) {
        std::cerr << "[Shader] ERROR: Failed to create shader program.\n";
        std::exit(-1);
    } else {
        std::cout << "[Shader] Created shader program. ID: " << geometryShader
                  << '\n';
    }

    glAttachShader(geometryShader, geometryVertexShader);

    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "[Shader] ERROR: Failed to attach vertex shader. ID: "
                  << geometryVertexShader << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Attached vertex shader. ID: "
                  << geometryVertexShader << '\n';
    }

    glAttachShader(geometryShader, geometryFragmentShader);

    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "[Shader] ERROR: Failed to attach fragment shader. ID: "
                  << geometryFragmentShader << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Attached fragment shader. ID: "
                  << geometryFragmentShader << '\n';
    }

    std::cout << "[Shader] Linking program " << geometryShader << "...\n";

    glLinkProgram(geometryShader);

    GLint success = GL_FALSE;
    glGetProgramiv(geometryShader, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(geometryShader, sizeof(infoLog), nullptr, infoLog);

        std::cerr << "[Shader] Program linking FAILED.\n";
        std::cerr << "[Shader] Program ID: " << geometryShader << '\n';
        std::cerr << "[Shader] Linker log:\n" << infoLog << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Program linked successfully.\n";
        std::cout << "[Shader] Program ID: " << geometryShader << '\n';
    }

    // same shi again, copy paste
    lightingVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(lightingVertexShader, 1, &lightingVertexShaderSource, NULL);
    glCompileShader(lightingVertexShader);

    lightingFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(lightingFragmentShader, 1, &lightingFragmentShaderSource,
                   NULL);
    glCompileShader(lightingFragmentShader);

    lightingShader = glCreateProgram();

    if (lightingShader == 0) {
        std::cerr << "[Shader] ERROR: Failed to create shader program.\n";
        std::exit(-1);
    } else {
        std::cout << "[Shader] Created shader program. ID: " << lightingShader
                  << '\n';
    }

    glAttachShader(lightingShader, lightingVertexShader);

    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "[Shader] ERROR: Failed to attach vertex shader. ID: "
                  << lightingVertexShader << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Attached vertex shader. ID: "
                  << lightingVertexShader << '\n';
    }

    glAttachShader(lightingShader, lightingFragmentShader);

    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "[Shader] ERROR: Failed to attach fragment shader. ID: "
                  << lightingFragmentShader << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Attached fragment shader. ID: "
                  << lightingFragmentShader << '\n';
    }

    std::cout << "[Shader] Linking program " << lightingShader << "...\n";

    glLinkProgram(lightingShader);

    success = GL_FALSE;
    glGetProgramiv(lightingShader, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(lightingShader, sizeof(infoLog), nullptr, infoLog);

        std::cerr << "[Shader] Program linking FAILED.\n";
        std::cerr << "[Shader] Program ID: " << lightingShader << '\n';
        std::cerr << "[Shader] Linker log:\n" << infoLog << '\n';
        std::exit(-1);

    } else {
        std::cout << "[Shader] Program linked successfully.\n";
        std::cout << "[Shader] Program ID: " << lightingShader << '\n';
    }

    shadowMapUniform = glGetUniformLocation(lightingShader, "shadowMap");

    lightSpaceMatrixUniform =
        glGetUniformLocation(lightingShader, "lightSpaceMatrix");

    // again
    shadowVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shadowVertexShader, 1, &shadowVertexShaderSource, NULL);
    glCompileShader(shadowVertexShader);
    shadowShader = glCreateProgram();
    if (shadowShader == 0) {
        std::cerr
            << "[Shader] ERROR: Failed to create shadow shader program.\n";
        std::exit(-1);
    } else {
        std::cout << "[Shader] Created shadow shader program. ID: "
                  << shadowShader << '\n';
    }
    glAttachShader(shadowShader, shadowVertexShader);
    if (glGetError() != GL_NO_ERROR) {
        std::cerr
            << "[Shader] ERROR: Failed to attach shadow vertex shader. ID: "
            << shadowVertexShader << '\n';
        std::exit(-1);
    } else {
        std::cout << "[Shader] Attached shadow vertex shader. ID: "
                  << shadowVertexShader << '\n';
    }
    std::cout << "[Shader] Linking shadow program " << shadowShader << "...\n";
    glLinkProgram(shadowShader);
    success = GL_FALSE;

    glGetProgramiv(shadowShader, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(shadowShader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "[Shader] Shadow program linking FAILED.\n";
        std::cerr << "[Shader] Program ID: " << shadowShader << '\n';
        std::cerr << "[Shader] Linker log:\n" << infoLog << '\n';
        std::exit(-1);
    } else {
        std::cout << "[Shader] Shadow program linked successfully.\n";
        std::cout << "[Shader] Program ID: " << shadowShader << '\n';
    }

    shadowModelLocation = glGetUniformLocation(shadowShader, "model");

    glUseProgram(lightingShader);

    // Lighting
    skyLightLoc = glGetUniformLocation(lightingShader, "skyLight");
    sunDirectionLoc = glGetUniformLocation(lightingShader, "sunDirection");
    sunLightLoc = glGetUniformLocation(lightingShader, "sunLight");
    groundLightLoc = glGetUniformLocation(lightingShader, "groundLight");

    sunDirection = glm::normalize(glm::vec3(0.5f, 0.6f, 0.35f));

    glm::vec3 sunLight(0.45f, 0.42f, 0.36f);

    glUniform3f(glGetUniformLocation(lightingShader, "sunLight"), sunLight.x,
                sunLight.y, sunLight.z);

    glUniform3f(skyLightLoc, 0.65f, 0.68f, 0.72f);

    glUniform3f(sunDirectionLoc, sunDirection.x, sunDirection.y,
                sunDirection.z);

    glUniform3f(groundLightLoc, 0.20f, 0.23f, 0.27f);

    // fog
    glm::vec3 fogColor(0.55f, 0.65f, 0.75f);

    glUniform3f(glGetUniformLocation(lightingShader, "fogColor"), fogColor.x,
                fogColor.y, fogColor.z);

    glUniform1f(glGetUniformLocation(lightingShader, "fogStart"), 150.0f);

    glUniform1f(glGetUniformLocation(lightingShader, "fogEnd"), 500.0f);

    glUniform3f(glGetUniformLocation(lightingShader, "cameraPosition"),
                cameraPos.x, cameraPos.y, cameraPos.z);

    // projection
    modelLocation = glGetUniformLocation(geometryShader, "model");

    viewLocation = glGetUniformLocation(geometryShader, "view");

    projectionLocation = glGetUniformLocation(geometryShader, "projection");

    // shadow
    shadowLightSpaceLocation =
        glGetUniformLocation(shadowShader, "lightSpaceMatrix");

    // Differed rendering
    glGenVertexArrays(1, &fullscreenVAO);
    glGenFramebuffers(1, &gBufferLoc);
    glBindFramebuffer(GL_FRAMEBUFFER, gBufferLoc);

    glGenTextures(1, &gPositionLoc);
    glBindTexture(GL_TEXTURE_2D, gPositionLoc);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 720, 0, GL_RGB, GL_FLOAT,
                 nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           gPositionLoc, 0);

    glGenTextures(1, &gNormalLoc);
    glBindTexture(GL_TEXTURE_2D, gNormalLoc);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 720, 0, GL_RGB, GL_FLOAT,
                 nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           gNormalLoc, 0);

    glGenTextures(1, &gDepthLoc);
    glBindTexture(GL_TEXTURE_2D, gDepthLoc);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1280, 720, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glGenTextures(1, &gAlbedoLoc);
    glBindTexture(GL_TEXTURE_2D, gAlbedoLoc);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1280, 720, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           gAlbedoLoc, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           gDepthLoc, 0);

    // Sun depth buffer
    glGenFramebuffers(1, &shadowFBO);
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1024, 1024, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "SHADOW FBO ERROR: 0x" << std::hex << status << std::dec
                  << '\n';
    } else {
        std::cout << "Shadow FBO COMPLETE\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, gBufferLoc);

    // Generate once in initalization
    GLuint attachments[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
    };

    glDrawBuffers(3, attachments);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "[GBuffer] ERROR: Framebuffer is not complete!\n";
        std::exit(-1);
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Init stuff
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

    glUseProgram(geometryShader);

    int textureLocation = glGetUniformLocation(geometryShader, "texture1");

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
                                            1280.0f / 720.0f,    // aspect ratio
                                            0.1f,                // near
                                            500.0f               // far
    );

    glm::mat4 viewProjection = projection * view;

    // pass 0
    glm::vec3 lightPos = cameraPos + sunDirection * 200.0f;

    glm::mat4 lightView =
        glm::lookAt(lightPos, cameraPos, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 lightProjection =
        glm::ortho(-120.0f, 120.0f, -120.0f, 120.0f, 1.0f, 400.0f);
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    glViewport(0, 0, 1024, 1024);

    // IMPORTANT
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowShader);

    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &model[0][0]);

    glUniformMatrix4fv(shadowLightSpaceLocation, 1, GL_FALSE,
                       &lightSpaceMatrix[0][0]);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.1f, 4.0f);

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowShader);

    for (auto &[position, chunk] : world->chunks) {
        if (!isChunkVisible(position, lightSpaceMatrix))
            continue;
        chunk->draw();
    }

    glDisable(GL_POLYGON_OFFSET_FILL);

    glCullFace(GL_BACK);
    glViewport(0, 0, 1280, 720);

    glBindFramebuffer(GL_FRAMEBUFFER, gBufferLoc);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(geometryShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

    for (auto &[position, chunk] : world->chunks) {
        if (!isChunkVisible(position, viewProjection))
            continue;
        chunk->draw();
    }

    // PASS #2, fancy shaders

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    glUseProgram(lightingShader); // now use fancy lighting shaders

    GLint gPositionUniform = glGetUniformLocation(lightingShader, "gPosition");

    GLint gNormalUniform = glGetUniformLocation(lightingShader, "gNormal");

    GLint gAlbedoUniform = glGetUniformLocation(lightingShader, "gAlbedo");
    GLint cameraPositionLoc =
        glGetUniformLocation(lightingShader, "cameraPosition");

    glUniform3f(cameraPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPositionLoc);
    glUniform1i(gPositionUniform, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormalLoc);
    glUniform1i(gNormalUniform, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoLoc);
    glUniform1i(gAlbedoUniform, 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glUniform1i(shadowMapUniform, 3);

    glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_FALSE,
                       &lightSpaceMatrix[0][0]);

    glBindVertexArray(fullscreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Scene::shouldClose() { return glfwWindowShouldClose(this->window); }
