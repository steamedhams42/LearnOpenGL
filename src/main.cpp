#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <iostream>

#include "constants.h"

GLFWwindow* window;
// Vertex buffer object
unsigned int VBO;
// Vertex shader
unsigned int vertexShader;

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

void vertexBufferSetup(unsigned int& VBO,
                       float vertices[],
                       const int nVertices) {
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  std::cerr << "Size of vertices in bytes:" << nVertices * sizeof(vertices[0])
            << std::endl;
  glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(vertices[0]), vertices,
               GL_STATIC_DRAW);
}

void vertexShaderSetup() {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &constants::VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
}

int main(void) {
  windowSetup();

  // Vertex buffer
  float vertices[constants::nVertices] = {-.5, -.5, 0, .5, -.5, 0, 0, .5, 0};
  vertexBufferSetup(VBO, vertices, constants::nVertices);

  // Vertex shader
  vertexShaderSetup();

  // Main rendering loop.
  while (!glfwWindowShouldClose(window)) {
    // User input listener
    processInput(window);

    // Rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
