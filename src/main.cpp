#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

// Please note that I use an automated clang formatter. This is why my code
// looks so "neat". I did not manually do things like space aligns, 2d array
// formatting, fancy indents etc. This isn't ai is the point im trying to make
// here

/*
Vertex shader I wrote. takes in a vector3 of a vertex's position, returns the
same thing.
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
Fragment shader code, called on each pixel draw thats in the triangle. returns
solid color
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

int main() {
    std::cout << "Initializing" << std::endl;

    if (!glfwInit()) {
        std::cerr << "Failed to init glfw, aborting!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(1280, 720, "rendrX demonstration", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window, aborting!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Compile the shaders at runtime
    // idk bout how ur supposed to do it in prod yet
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // make the program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Load textures
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // horizontal/u
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // vertical/v
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    // tri mesh
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
                        1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.5f, 1.0f};
    // upload triangle to gpu
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // 1 triangle/unique VAO
    glGenBuffers(1, &VBO);      // 1 buffer? still unsure

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // select our vbo

    // Upload vertecies to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, // layout location = 0 as seen in frag shader
                          3, // 3 components per vertex (Xyz)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          5 * sizeof(float), // vertex size in vbo
                          (void *)0          // no offset (0)
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, // layout location = 1 as seen in frag shader
                          2, // 2 components per vertex (Xyz)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          5 * sizeof(float),          // vertex size in vbo
                          (void *)(3 * sizeof(float)) // skip xyz offset (3)
    );
    glEnableVertexAttribArray(1);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    float pitch = 0.0f;
    float yaw = -90.0f;

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos.x += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos.x -= 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos.z += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos.z -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cameraPos.y += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cameraPos.y -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            yaw -= 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            yaw += 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            pitch += 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            pitch -= 1.0f;
        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw));

        // Matricies
        glm::mat4 model =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));

        // Look at the triangle at origin ALWAYS
        glm::mat4 view = glm::lookAt(cameraPos,                  // pos
                                     cameraPos + direction,      // direction
                                     glm::vec3(0.0f, 1.0f, 0.0f) // up
        );

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), // FOV
                                                800.0f / 600.0f, // aspect ratio
                                                0.1f,            // near
                                                100.0f           // far
        );

        glClearColor(0.1, 0.2, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // configure matricies
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                           GL_FALSE, &model[0][0]);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1,
                           GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                           GL_FALSE, &projection[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
