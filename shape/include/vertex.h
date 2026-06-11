#pragma once
#include <GL/gl.h>
#include <cstring>
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;

    Vertex() : position(0.0f), color(0.0f) {}
    Vertex(const glm::vec3& pos, const glm::vec3& col) : position(pos), color(col) {}
};
