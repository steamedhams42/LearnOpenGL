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
  // Element buffer object
  unsigned int EBO;
  float vertices[] = {
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first Triangle
      1, 2, 3   // second Triangle
  };
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element
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
    glUseProgram(shaderProgram);
    // Seeing as we only have a single VAO there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
