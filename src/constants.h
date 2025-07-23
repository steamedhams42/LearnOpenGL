#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {

const int WIDTH = 1440;
const int HEIGHT = 900;

const float ASPECT_RATIO = 1.0f * WIDTH / HEIGHT;

// z-distance (depth) from frustums
const float NEAR = 0.1f;
const float FAR = 100.0f;

const int nTriangles = 3;
const int nVertices = 3 * nTriangles;

const int nCubes = 10;

};  // namespace constants

#endif