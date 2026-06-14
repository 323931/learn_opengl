#include <cstdlib>
#include <glm/glm.hpp>
glm::vec3 randomColor(){
    float r = rand() / (float)RAND_MAX;
    float g = rand() / (float)RAND_MAX;
    float b = rand() / (float)RAND_MAX;
    return glm::vec3(r, g, b);
}