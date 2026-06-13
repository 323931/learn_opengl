#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
class Camera{
public:
    Camera();
    ~Camera();
public:
    glm::mat4 getWorldToViewMartix() const{
        return glm::lookAt(position, position + viewDirection, upDirection);
    }
public:
    //鼠标移动视角相关
    void mousePositionUpdated(const glm::vec2& newMousePosition);
    //键盘移动相机位置
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
private:
    glm::vec3 position;
    glm::vec3 viewDirection;
    const glm::vec3 upDirection;
    glm::vec2 oldMousePosition;
    float sensitivity = 0.002f;
    bool isFirstMouse = true;
};
