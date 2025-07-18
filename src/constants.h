#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace shader_source {
const char* VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
};  // namespace shader_source

namespace constants {

const int nTriangles = 3;
const int nVertices = 3 * nTriangles;

};  // namespace constants

#endif