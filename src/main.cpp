#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
////////////// line here for linter glad.h must be first
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

#include "constants.h"
#include "shaders.h"

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

void vertexShaderSetup(unsigned int& vertex_shader) {
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader::VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertex_shader);
  wasShaderSuccessful(vertex_shader);
}

void fragmentShaderSetup(unsigned int& fragment_shader,
                         const char* shader_source) {
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &shader_source, NULL);
  glCompileShader(fragment_shader);
  wasShaderSuccessful(fragment_shader);
}

void shaderSetup(unsigned int& shader_program, const char* shader_source) {
  shader_program = glCreateProgram();

  // (1) Vertex shader
  unsigned int vertex_shader;
  vertexShaderSetup(vertex_shader);
  glAttachShader(shader_program, vertex_shader);

  // (2) Fragment shader
  unsigned int fragment_shader;
  fragmentShaderSetup(fragment_shader, shader_source);
  glAttachShader(shader_program, fragment_shader);

  // (3) Link shaders
  glLinkProgram(shader_program);
  int success;
  char infoLog[512];
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    assert(success);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

int main(void) {
  windowSetup();

  // Build shader program
  unsigned int orange_shader_program, yellow_shader_program;
  shaderSetup(orange_shader_program,
              fragment_shader::ORANGE_FRAGMENT_SHADER_SOURCE);
  shaderSetup(yellow_shader_program,
              fragment_shader::YELLOW_FRAGMENT_SHADER_SOURCE);

  float triangle1[] = {
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, 0.5f,  0.0f   // top left
  };
  float triangle2[] = {
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left
  };
  unsigned int VBO[2], VAO[2];
  // Vertex buffer object
  glGenBuffers(2, VBO);
  // Vertex array object
  glGenVertexArrays(2, VAO);

  // (1) Bind the Vertex Array Object first
  glBindVertexArray(VAO[0]);

  // (2a) Bind and copy vertices in vertex buffer object.
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

  // (3) Configure vertex attributes(s).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Triangle 2
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
  // (3) Configure vertex attributes(s).
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

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
    glUseProgram(orange_shader_program);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw using yellow shader
    glUseProgram(yellow_shader_program);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO[0]);
  glDeleteBuffers(1, &VBO[0]);
  glDeleteVertexArrays(1, &VAO[1]);
  glDeleteBuffers(1, &VBO[1]);
  glDeleteProgram(orange_shader_program);
  glDeleteProgram(yellow_shader_program);

  glfwTerminate();
  return 0;
}
