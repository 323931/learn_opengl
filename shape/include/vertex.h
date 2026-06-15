#pragma once
#include <GL/gl.h>
#include <cstring>
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;

    Vertex() : position(0.0f), color(0.0f),normal(0.0f,1.0f,0.0f) {}
    Vertex(const glm::vec3& pos, const glm::vec3& col) : position(pos), color(col), normal(0.0f,1.0f,0.0f) {}
    Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec3& norm) : position(pos), color(col), normal(norm) {}
};
