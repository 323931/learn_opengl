#include "camera.h"
#include <glm/geometric.hpp>

Camera::Camera()
    : position(0.0f, 0.0f, 0.0f), 
    viewDirection(0.0f, 0.0f, -1.0f), 
    upDirection(0.0f, 1.0f, 0.0f){
}

Camera::~Camera()
{
}

void Camera::mousePositionUpdated(const glm::vec2& newMousePosition){
    glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

    if(mouseDelta.x >= 10.0f){
        oldMousePosition = newMousePosition;
        return;
    }


    if(isFirstMouse){
        isFirstMouse = false;
        return;
    }
    
    //相机左右转动视角,绕y轴旋转
    float angleAroundY = -mouseDelta.x * sensitivity;
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 
    angleAroundY,
     glm::vec3(0.0f,1.0f,0.0f));

    viewDirection = glm::vec3(rotate *glm::vec4(viewDirection,0.0f));
    viewDirection = glm::normalize(viewDirection);
    
    //相机上下转动视角,绕相机当前的right方向旋转。也就是pitch，抬头低头
    float angleAroundRight = -mouseDelta.y * sensitivity;

    //用“看的方向”和“头顶方向”叉乘，算出相机的右方向。
    //叉乘 cross(a, b) 的作用是：
    // 给两个方向 a 和 b，算出一个同时垂直于它们的新方向。
    glm::vec3 rightDirection = glm::cross(viewDirection, upDirection);

    glm::mat4 pitch = glm::rotate(glm::mat4(1.0f),
        angleAroundRight, 
        rightDirection);
 
    viewDirection = glm::vec3(pitch * glm::vec4(viewDirection,0.0f));
    viewDirection = glm::normalize(viewDirection);

    oldMousePosition = newMousePosition;
}