#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>  // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>

class Shader {
 public:
  // constructor reads and builds the shader
  Shader(const char* vertex_shader_file_path,
         const char* fragment_shader_file_path);
  // Use/activate the shader
  void use();
  // Delete shader
  void Delete();
  // Utility uniform functions
  void set_bool(const std::string& name, bool value) const;
  void set_int(const std::string& name, int value) const;
  void set_float(const std::string& name, float value) const;
  void set_mat4(const std::string& name, const glm::mat4& mat) const;

  unsigned int id() { return shader_program_; }

 private:
  enum ShaderType { VERTEX = 0, FRAGMENT = 1, PROGRAM = 2 };
  void compileShader(unsigned int& shader,
                     const char* source,
                     Shader::ShaderType type);
  void printShaderLogIfError(unsigned int shader,
                             Shader::ShaderType type) const;
  // Shader program ID
  unsigned int shader_program_;
};

#endif