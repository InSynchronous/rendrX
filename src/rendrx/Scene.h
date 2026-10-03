#pragma once

#include "rendrx/World.h"
#define GLFW_INCLUDE_NONE
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rendrx {
struct Hit {
    bool hit = false;

    glm::ivec3 block{0};
    glm::ivec3 normal{0};

    float distance = 0.0f;
};

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

class Scene {
  private:
    GLFWwindow *window;
    unsigned int geometryVertexShader, geometryFragmentShader,
        lightingFragmentShader, lightingVertexShader, shadowVertexShader,
        shadowShader, lightingShader, geometryShader, texture, fullscreenVAO;
    // more shaders ahghghgh refactoring sucks
    unsigned int crosshairVertexShader;
    unsigned int crosshairFragmentShader;
    unsigned int crosshairShader;
    GLint screenSizeLoc;
    GLint modelLocation;
    GLint viewLocation;
    GLint projectionLocation;
    GLint shadowLightSpaceLocation;
    GLint shadowModelLocation;
    GLint shadowMapUniform;
    GLint lightSpaceMatrixUniform;

    GLint groundLightLoc;
    GLint skyLightLoc;
    GLint sunDirectionLoc;
    GLint sunLightLoc;

    // Buffers
    GLuint gBufferLoc;
    GLuint gPositionLoc;
    GLuint gNormalLoc;
    GLuint gDepthLoc;
    GLuint gAlbedoLoc;

    // shadow buffer
    GLuint shadowMap;
    GLuint shadowFBO;

    float deltaTime;
    float lastFrame;

    glm::vec3 prevCameraPos = glm::vec3(0.0f, 120.0f, 3.0f);
    glm::vec3 cameraPos = prevCameraPos;
    glm::vec3 cameraRot = glm::vec3(0.0f, 0.0f, 55.0f);
    float fpsTimer = 0.0f;
    int frameCount = 0;
    float fps = 0.0f;

    glm::vec3 sunDirection;
    glm::vec3 sunPos;

    World *world = nullptr;

    /*
       Vertex shader I wrote. takes in a vector3 of a vertex's position, returns
       the same thing.
    */
    const char *geometryVertexShaderSource = R"(
        #version 330 core
    
        layout (location = 0) in vec3 vertData;
        layout (location = 1) in vec2 uvData;
        layout (location = 2) in vec3 normalData;
    
        out vec2 uvCoord;
        out vec3 fragPosition;
        out vec3 normalCoord;
    
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
    
        void main()
        {
            vec4 worldPos = model * vec4(vertData, 1.0);
    
            fragPosition = worldPos.xyz;
            uvCoord = uvData;
            normalCoord = normalData;
    
            gl_Position = projection * view * worldPos;
        }
    )";

    const char *lightingVertexShaderSource = R"(
        #version 330 core
    
        out vec2 uv;
    
        void main()
        {
            vec2 positions[3] = vec2[](
                vec2(-1.0, -1.0),
                vec2( 3.0, -1.0),
                vec2(-1.0,  3.0)
            );
    
            vec2 position = positions[gl_VertexID];
    
            uv = position * 0.5 + 0.5;
    
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )";

    const char *shadowVertexShaderSource = R"(
        #version 330 core

        layout (location = 0) in vec3 vertData;
        
        uniform mat4 model;
        uniform mat4 lightSpaceMatrix;
        
        
        void main()
        {
            gl_Position =
                lightSpaceMatrix *
                model *
                vec4(vertData, 1.0);
        }
        
    )";

    /*
    Fragment shader code, called on each pixel draw thats in the triangle.
    returns texture lookup result
    */

    const char *geometryFragmentShaderSource = R"(
        #version 330 core
    
        in vec2 uvCoord;
        in vec3 fragPosition;
        in vec3 normalCoord;
    
        layout (location = 0) out vec3 gPosition;
        layout (location = 1) out vec3 gNormal;
        layout (location = 2) out vec4 gAlbedo;
    
        uniform sampler2D texture1;
    
        void main()
        {
            gPosition = fragPosition;
            gNormal = normalize(normalCoord);
            gAlbedo = texture(texture1, uvCoord);
        }
    )";

    const char *lightingFragmentShaderSource = R"(
        #version 330 core
    
        in vec2 uv;
    
        out vec4 FragColor;
    
        uniform sampler2D gPosition;
        uniform sampler2D gNormal;
        uniform sampler2D gAlbedo;
    
        uniform vec3 skyLight;
        uniform vec3 groundLight;
    
        uniform vec3 sunDirection;
        uniform vec3 sunLight;
    
        uniform float fogStart;
        uniform float fogEnd;
        uniform vec3 fogColor;
        uniform vec3 cameraPosition;
    
        uniform sampler2D shadowMap;
        uniform mat4 lightSpaceMatrix;

        float sampleShadow(vec3 shadowCoords)
        {
            float bias = 0.0008;
            float texelSize = 1.0 / 1024.0;
        
            float shadow = 0.0;
        
            for (int y = -1; y <= 1; ++y)
            {
                for (int x = -1; x <= 1; ++x)
                {
                    vec2 offset =
                        vec2(x, y) * texelSize;
        
                    float shadowDepth =
                        texture(
                            shadowMap,
                            shadowCoords.xy + offset
                        ).r;
        
                    shadow +=
                        shadowCoords.z - bias > shadowDepth
                            ? 1.0
                            : 0.0;
                }
            }
        
            return shadow / 9.0;
        }
         
        void main()
        {
            vec3 position = texture(gPosition, uv).rgb;
            vec3 normal = normalize(texture(gNormal, uv).rgb);
            vec4 albedoSample = texture(gAlbedo, uv);
            if (albedoSample.a == 0.0) {
                // Background pixel, do not light
                FragColor = vec4(0.1, 0.2, 0.3, 1.0);
                return;
            }
            vec3 albedo = albedoSample.rgb;

    
            vec4 lightSpacePosition =
                lightSpaceMatrix * vec4(position, 1.0);
    
            vec3 shadowCoords =
                lightSpacePosition.xyz /
                lightSpacePosition.w;
    
            shadowCoords =
                shadowCoords * 0.5 + 0.5;
    
            float shadow = 0.0;

            if (
                shadowCoords.x >= 0.0 &&
                shadowCoords.x <= 1.0 &&
                shadowCoords.y >= 0.0 &&
                shadowCoords.y <= 1.0 &&
                shadowCoords.z >= 0.0 &&
                shadowCoords.z <= 1.0
            )
            {
                shadow = sampleShadow(shadowCoords);
            }
    
            float hemisphere = normal.y * 0.5 + 0.5;
    
            float sunAmount = max(
                dot(normal, normalize(sunDirection)),
                0.0
            );
    
            vec3 ambient = mix(
                groundLight,
                skyLight,
                hemisphere
            );
    
            vec3 lighting =
                ambient +
                sunAmount * sunLight * (1.0 - shadow);
    
            float distanceToCamera =
                length(position - cameraPosition);
    
            float fogFactor = clamp(
                (distanceToCamera - fogStart) / (fogEnd - fogStart),
                0.0,
                1.0
            );
    
            vec3 final = mix(
                albedo * lighting,
                fogColor,
                fogFactor
            );
    
            FragColor = vec4(
                final,
                1.0
            );
        }
        
    )";

    const char *crosshairVertexShaderSource = R"(
        #version 330 core
    
        uniform vec2 screenSize;
    
        const vec2 positions[4] = vec2[](
            vec2(-10.0, 0.0),
            vec2( 10.0, 0.0),
            vec2(0.0, -10.0),
            vec2(0.0,  10.0)
        );
    
        void main()
        {
            vec2 pixel = positions[gl_VertexID];
    
            vec2 center = screenSize * 0.5;
    
            pixel += center;
    
            vec2 ndc = pixel / screenSize * 2.0 - 1.0;
    
            gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0);
        }
    )";

    const char *crosshairFragmentShaderSource = R"(
        #version 330 core
    
        out vec4 FragColor;
    
        void main()
        {
            FragColor = vec4(1.0, 1.0, 1.0, 0.5);
        }
    )";

    void init();
    void loadTextures();
    bool isChunkVisible(glm::ivec2 chunkPos, const glm::mat4 &viewProjection);

    Hit raycast(Ray &r, float maxDistance);
    Block selectedBlock = Block::BRICK;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
    bool rightMouseWasDown = false;
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
        Scene *scene = static_cast<Scene *>(glfwGetWindowUserPointer(window));

        static double lastX = xpos;
        static double lastY = ypos;

        double dx = xpos - lastX;
        double dy = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f;

        scene->cameraRot.z += dx * sensitivity;
        scene->cameraRot.x -= dy * sensitivity;

        // Prevent looking completely upside down
        scene->cameraRot.x = glm::clamp(scene->cameraRot.x, -89.0f, 89.0f);
    }

  public:
    Scene();
    void addWorld(World &world);
    bool shouldClose();
    void launch();
    void render();

    int frameBufferLength, windowLength = 1280;
    int frameBufferHeight, windowHeight = 720;

    const glm::vec3 &getCameraPos() const { return cameraPos; }
    const glm::vec3 &getPrevCameraPos() const { return prevCameraPos; }
    const glm::vec3 &getCameraRot() const { return cameraRot; }
};
} // namespace rendrx
