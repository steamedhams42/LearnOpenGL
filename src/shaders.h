#ifndef SHADERS_H
#define SHADERS_H

namespace vertex_shader {

const char* VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor; // specify a color output to the fragment shader\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";

};  // namespace vertex_shader

namespace fragment_shader {

const char* RAINBOW_FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"

    "in vec3 vertexColor;\n"

    "void main()\n"
    "{\n"
    "    FragColor = vec4(vertexColor, 1.);\n"
    "}\0";

const char* ORANGE_FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char* YELLOW_FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.f, 1.f, 0f, 1.0f);\n"
    "}\0";

};  // namespace fragment_shader

#endif