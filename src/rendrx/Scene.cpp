#include "Scene.h"
#include "rendrx/Triangle.h"
#include <iostream>
#include <vector>

using namespace rendrx;

void Scene::addTriangle(Triangle t) { triangles.push_back(t); }

void Scene::init() {
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

    // Compile the shaders at runtime
    // idk bout how ur supposed to do it in prod yet
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // make the program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // setup buffers
    glGenVertexArrays(1, &VAO); // 1 triangle/unique VAO
    glGenBuffers(1, &VBO);      // 1 buffer? still unsure

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // select our vbo
}

void Scene::loadTextures() {
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
}

void Scene::uploadTriangles() {
    // Flatten the array
    std::vector<float> vertices;
    for (const Triangle &tri : triangles) {
        vertices.insert(vertices.end(),
                        {tri.p1.x, tri.p1.y, tri.p1.z, tri.u1.x, tri.u1.y,

                         tri.p2.x, tri.p2.y, tri.p2.z, tri.u2.x, tri.u2.y,

                         tri.p3.x, tri.p3.y, tri.p3.z, tri.u3.x, tri.u3.y});
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // select our vbo

    // Upload vertecies to the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, // layout location = 0 as seen in frag shader
                          3, // 3 components per vertex (Xyz)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          5 * sizeof(float), // vertex size in vbo
                          (void *)0          // no offset (0)
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, // layout location = 1 as seen in frag shader
                          2, // 2 components per vertex (uv)
                          GL_FLOAT, // is a float
                          GL_FALSE, // don't round?? idk i mean its a float lol
                          5 * sizeof(float),          // vertex size in vbo
                          (void *)(3 * sizeof(float)) // skip xyz offset (3)
    );

    glEnableVertexAttribArray(1);
}

void Scene::launch() {
    this->init();
    this->loadTextures();
    this->uploadTriangles();
}

void Scene::render() {
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

    // Matricies
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));

    // Look at the triangle at origin ALWAYS
    glm::mat4 view = glm::lookAt(cameraPos,                  // pos
                                 cameraPos + direction,      // direction
                                 glm::vec3(0.0f, 1.0f, 0.0f) // up
    );

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), // FOV
                                            800.0f / 600.0f,     // aspect ratio
                                            0.1f,                // near
                                            100.0f               // far
    );

    glClearColor(0.1, 0.2, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // configure matricies
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                       GL_FALSE, &model[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                       &view[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                       GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Scene::shouldClose() { return glfwWindowShouldClose(this->window); }
