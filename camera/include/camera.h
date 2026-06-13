#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
class Camera{
public:
    Camera();
    ~Camera();
public:
    glm::mat4 getWorldToViewMartix() const{
        return glm::lookAt(position, position + viewDirection, upDirection);
    }
private:
    glm::vec3 position;
    glm::vec3 viewDirection;
    const glm::vec3 upDirection;
};