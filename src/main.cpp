#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

#include "constants.h"

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

  // (1) Vertex shader
  unsigned int vertexShader;
  vertexShaderSetup(vertexShader);
  glAttachShader(shaderProgram, vertexShader);

  // (2) Fragment shader
  unsigned int fragmentShader;
  fragmentShaderSetup(fragmentShader);
  glAttachShader(shaderProgram, fragmentShader);

  // (3) Link shaders
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

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

int main(void) {
  windowSetup();

  // Build shader program
  unsigned int shaderProgram;
  shaderSetup(shaderProgram);

  // Vertex buffer object
  unsigned int VBO;
  // Vertex array object
  unsigned int VAO;
  float vertices[constants::nVertices] = {-.5, -.5, 0, .5, -.5, 0, 0, .5, 0};
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // Bind the Vertex Array Object first
  glBindVertexArray(VAO);
  // Bind and set vertex buffer(s)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Configure vertex attributes(s).
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  // Main rendering loop.
  while (!glfwWindowShouldClose(window)) {
    // User input listener
    processInput(window);

    // Rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
