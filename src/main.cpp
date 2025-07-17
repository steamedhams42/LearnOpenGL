#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

#include "constants.h"

GLFWwindow* window;
// Vertex buffer object
unsigned int VBO;

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

void wasShaderSuccessful(unsigned int& shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    assert(success);
  }
}

void vertexShaderSetup(unsigned int& vertexShader) {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &shader_source::VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertexShader);
  wasShaderSuccessful(vertexShader);
}

void fragmentShaderSetup(unsigned int& fragmentShader) {
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &shader_source::FRAGMENT_SHADER_SOURCE,
                 NULL);
  glCompileShader(fragmentShader);
  wasShaderSuccessful(fragmentShader);
}

void shaderSetup(unsigned int& shaderProgram) {
  shaderProgram = glCreateProgram();

  unsigned int vertexShader;
  vertexShaderSetup(vertexShader);
  glAttachShader(shaderProgram, vertexShader);

  unsigned int fragmentShader;
  fragmentShaderSetup(fragmentShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    assert(success);
  }

  glUseProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

int main(void) {
  windowSetup();

  // Vertex buffer
  float vertices[constants::nVertices] = {-.5, -.5, 0, .5, -.5, 0, 0, .5, 0};
  vertexBufferSetup(VBO, vertices, constants::nVertices);

  // Shaders
  unsigned int shaderProgram;
  shaderSetup(shaderProgram);

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
