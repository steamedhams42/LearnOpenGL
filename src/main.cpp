#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <cassert>
#include <cmath>
#include <iostream>

#include "constants.h"
#include "shader.h"

GLFWwindow* window;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void windowSetup() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void renderTriangle() {
  const char* vertex_shader_fp = "src/shaders/vertex/vertex.vs";
  const char* rainbow_fragment_shader_fp = "src/shaders/fragment/rainbow.frag";

  // Shader shader(vertex_shader_file_path, magenta_fragment_shader_fp);
  Shader rainbow_shader(vertex_shader_fp, rainbow_fragment_shader_fp);

  float triangle1[] = {
      // positions         // colors (R, G, B)
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // top
  };
  unsigned int VBO, VAO;
  // Vertex buffer object
  glGenBuffers(1, &VBO);
  // Vertex array object
  glGenVertexArrays(1, &VAO);

  // (1) Bind the Vertex Array Object first
  glBindVertexArray(VAO);

  // (2a) Bind and copy vertices in vertex buffer object.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

  // (3) Configure vertex attributes(s).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // (4) Configure triangle color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind.
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Remember: do NOT unbind the EBO while a VAO is active as the bound element
  // buffer object IS stored in the VAO; keep the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Main rendering loop.
  while (!glfwWindowShouldClose(window)) {
    // User input listener
    processInput(window);

    // Rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    rainbow_shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  rainbow_shader.Delete();
}

void renderTexture() {
  while (!glfwWindowShouldClose(window)) {
    // User input listener
    processInput(window);

    // Rendering code here

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
      renderTriangle();
      break;
    case 1:
      renderTexture();
      break;
  }

  glfwTerminate();
  return 0;
}
