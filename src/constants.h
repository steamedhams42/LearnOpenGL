#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {

const char* VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const int nTriangles = 3;
const int nVertices = 3 * nTriangles;

};  // namespace constants

#endif