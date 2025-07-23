#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <cassert>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "constants.h"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLFWwindow* window;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void windowSetup() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(constants::WIDTH, constants::HEIGHT, "LearnOpenGL",
                            NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return exit(-1);
  }

  glViewport(0, 0, constants::WIDTH, constants::HEIGHT);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void renderTexture() {
  const char* vertex_shader_fp = "src/shaders/vertex/vertex.vs";
  const char* fragment_shader_fp = "src/shaders/fragment/texture.frag";
  Shader texture_shader(vertex_shader_fp, fragment_shader_fp);

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };
  unsigned int VBO, VAO, EBO;
  // Vertex buffer object
  glGenBuffers(1, &VBO);
  // Vertex array object
  glGenVertexArrays(1, &VAO);
  // Vertex buffer object
  glGenBuffers(1, &EBO);

  // (1) Bind the Vertex Array Object first
  glBindVertexArray(VAO);

  // (2a) Bind and copy vertices in vertex buffer object.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // (2b) Bind vertex buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // (3) Configure object position coordinates(s).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // (4) Configure object color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // (5) Configure texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Bind texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Texture wrapping options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load
  int texture_width, texture_height, nChannels;
  unsigned char* texture_data = stbi_load("src/container.jpg", &texture_width,
                                          &texture_height, &nChannels, 0);
  if (texture_data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Texture data failed to load." << std::endl;
    assert(false);
  }
  stbi_image_free(texture_data);

  texture_shader.use();
  texture_shader.set_int("texture", 0);

  // note: currently we set the projection matrix each frame, but since the
  // projection matrix rarely changes it's often best practice to set it
  // outside the main loop only once.
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), constants::ASPECT_RATIO,
                                0.1f, 100.0f);
  texture_shader.set_mat4("projection", projection);

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // User input listener
    processInput(window);

    // Clear background
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    texture_shader.use();

    // Create matrix transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    model =
        glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // Extract uniform matrix locations from shader
    unsigned int modelLoc = glGetUniformLocation(texture_shader.id(), "model");
    unsigned int viewLoc = glGetUniformLocation(texture_shader.id(), "view");
    // Pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    // Rendering code here
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Buffer swap
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}

int main(void) {
  windowSetup();

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
