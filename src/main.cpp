#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <array>
#include <cassert>
#include <cmath>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>

#include "constants.h"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWwindow* WINDOW;
// Camera starting position
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse
bool first_mouse = true;
float last_x = constants::WIDTH / 2.f;
float last_y = constants::HEIGHT / 2.f;
float yaw = -90.0;
float pitch = 0;
float fov = 45.0;

// keyboard
float delta_time = 0.0f;
float last_time = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double x, double y) {
  if (first_mouse) {
    last_x = x;
    last_y = y;
    first_mouse = false;
  }

  float x_offset = x - last_x;
  float y_offset = last_y - y;
  last_x = x;
  last_y = y;

  float sensitivity = 0.1f;
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  yaw += x_offset;
  pitch += y_offset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  fov -= (float)yoffset;
  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;
}

float random_real(float x = 1) {
  return x * std::rand() / RAND_MAX;
}

void windowSetup() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  WINDOW = glfwCreateWindow(constants::WIDTH, constants::HEIGHT, "LearnOpenGL",
                            NULL, NULL);
  if (WINDOW == NULL) {
    std::cout << "Failed to create GLFW WINDOW" << std::endl;
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(WINDOW);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return exit(-1);
  }

  glViewport(0, 0, constants::WIDTH, constants::HEIGHT);

  glfwSetFramebufferSizeCallback(WINDOW, framebufferSizeCallback);
  glfwSetCursorPosCallback(WINDOW, mouseCallback);
  glfwSetScrollCallback(WINDOW, scrollCallback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void processInput(float delta_time) {
  if (glfwGetKey(WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(WINDOW, true);
  const float cameraSpeed = 2.5f * delta_time;
  if (glfwGetKey(WINDOW, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(WINDOW, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(WINDOW, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(WINDOW, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void renderTexture() {
  const char* vertex_shader_fp = "src/shaders/vertex/vertex.vs";
  const char* fragment_shader_fp = "src/shaders/fragment/texture.frag";
  Shader texture_shader(vertex_shader_fp, fragment_shader_fp);

  std::srand(std::time(NULL));

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  // world space positions of our cubes
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load and create a texture
  // -------------------------
  unsigned int texture;
  // texture 1
  // ---------
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true);  // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char* data =
      stbi_load("src/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  texture_shader.use();
  texture_shader.set_int("texture", 0);

  // note: currently we set the projection matrix each frame, but since the
  // projection matrix rarely changes it's often best practice to set it
  // outside the main loop only once.
  std::array<std::array<float, 4>, constants::nCubes> cube_axes = {};
  for (int i = 0; i < constants::nCubes; i++) {
    cube_axes[i] = {random_real(), random_real(), random_real(),
                    random_real(360)};
    // std::cout << cube_axes[i][0] << cube_axes[i][1] << cube_axes[i][2]
    //           << cube_axes[i][3] << std::endl;
  }

  // Main rendering loop
  while (!glfwWindowShouldClose(WINDOW)) {
    // User input listener
    float current_time = glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;
    processInput(delta_time);

    // Clear background and buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Activate shader
    texture_shader.use();

    // Perspective projection. 3D -> 2D
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), constants::ASPECT_RATIO,
                                  constants::NEAR, constants::FAR);
    texture_shader.set_mat4("projection", projection);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Create camera position and rotation transformations
    unsigned int viewLoc = glGetUniformLocation(texture_shader.id(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    texture_shader.set_mat4("view", view);

    // Retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(texture_shader.id(), "model");

    // render box(es)
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < constants::nCubes; i++) {
      // calculate the model matrix for each object and pass it to shader before
      // drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(
          model, (float)glfwGetTime() * glm::radians(cube_axes[i][3]),
          glm::vec3(cube_axes[i][0], cube_axes[i][1], cube_axes[i][2]));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      texture_shader.set_mat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Buffer swap
    glfwPollEvents();
    glfwSwapBuffers(WINDOW);
  }
}

int main(void) {
  windowSetup();
  glEnable(GL_DEPTH_TEST);

  int i = 1;
  switch (i) {
    case 0:
      break;
    case 1:
      renderTexture();
      break;
  }

  glfwTerminate();
  return 0;
}
