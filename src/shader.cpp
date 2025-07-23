#include "shader.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertex_shader_file_path,
               const char* fragment_shader_file_path) {
  // 1. Extract shaders
  std::string vertex_shader_source;
  std::string fragment_shader_source;
  std::ifstream vertex_shader_file;
  std::ifstream fragment_shader_file;

  // ensure ifstream objects can throw exceptions:
  vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_shader_file.exceptions(std::ifstream::failbit |
                                  std::ifstream::badbit);

  try {
    // Open shader files
    vertex_shader_file.open(vertex_shader_file_path);
    fragment_shader_file.open(fragment_shader_file_path);

    // Pass files to std::stringstream
    std::stringstream vertex_shader_stream, fragment_shader_stream;
    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();

    // Close file handlers
    vertex_shader_file.close();
    fragment_shader_file.close();

    // Convert std::stringstream to std::string
    vertex_shader_source = vertex_shader_stream.str();
    fragment_shader_source = fragment_shader_stream.str();

  } catch (std::ifstream::failure& e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
    assert(false);
  }

  const char* vertex_shader_source_ = vertex_shader_source.c_str();
  const char* fragment_shader_source_ = fragment_shader_source.c_str();

  // 2. Compile shaders
  unsigned int vertex_shader, fragment_shader;
  compileShader(vertex_shader, vertex_shader_source_, ShaderType::VERTEX);
  compileShader(fragment_shader, fragment_shader_source_, ShaderType::FRAGMENT);

  // 3. Attach and link shader program
  shader_program_ = glCreateProgram();
  glAttachShader(shader_program_, vertex_shader);
  glAttachShader(shader_program_, fragment_shader);
  glLinkProgram(shader_program_);

  char infoLog[512];
  int success;
  glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::LINKING_ERROR\n"
              << infoLog << std::endl;
    assert(success);
  }

  // 4. Delete shaders after linking
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::compileShader(unsigned int& shader,
                           const char* source,
                           Shader::ShaderType type) {
  switch (type) {
    case VERTEX:
      shader = glCreateShader(GL_VERTEX_SHADER);
      break;
    case FRAGMENT:
      shader = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    default:
      assert(false);
      break;
  }
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
}

void Shader::printShaderLogIfError(unsigned int shader,
                                   Shader::ShaderType type) const {
  int success;
  char info_log[1024];
  switch (type) {
    case VERTEX:
    case FRAGMENT: {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << info_log
                  << std::endl;
        assert(success);
      };
      break;
    }
    case PROGRAM:
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, info_log);
        std::cout
            << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
            << info_log
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
      break;
  }
}

void Shader::use() {
  glUseProgram(shader_program_);
}

void Shader::Delete() {
  glDeleteProgram(shader_program_);
}

void Shader::set_bool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(shader_program_, name.c_str()), value);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(shader_program_, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}