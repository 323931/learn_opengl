#include "camera.h"
#include <glm/geometric.hpp>
#include <iostream>
Camera::Camera()
    : position(-7.76914f, 1.59395f, -2.79f), 
    viewDirection(0.98f, -0.16f, 0.0288f), 
    upDirection(0.0f, 1.0f, 0.0f){
}

Camera::~Camera()
{
}

void Camera::mousePositionUpdated(const glm::vec2& newMousePosition){
    glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

    if(isFirstMouse){
        oldMousePosition = newMousePosition;
        isFirstMouse = false;
        return;
    }

    if(mouseDelta.x >= 10.0f){
        oldMousePosition = newMousePosition;
        return;
    }
    
    //相机左右转动视角,绕y轴旋转
    float angleAroundY = -mouseDelta.x * sensitivity;
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 
    angleAroundY,
     glm::vec3(0.0f,1.0f,0.0f));

    viewDirection = glm::vec3(rotate *glm::vec4(viewDirection,0.0f));
    
    //相机上下转动视角,绕相机当前的right方向旋转。也就是pitch，抬头低头
    float angleAroundRight = -mouseDelta.y * sensitivity;

    //用“看的方向”和“头顶方向”叉乘，算出相机的右方向。
    //叉乘 cross(a, b) 的作用是：
    // 给两个方向 a 和 b，算出一个同时垂直于它们的新方向。
    glm::vec3 rightDirection = glm::normalize(glm::cross(viewDirection, upDirection));

    glm::mat4 pitch = glm::rotate(glm::mat4(1.0f),
        angleAroundRight, 
        rightDirection);
 
    viewDirection = glm::vec3(pitch * glm::vec4(viewDirection,0.0f));
    viewDirection = glm::normalize(viewDirection);

    oldMousePosition = newMousePosition;
}

void Camera::moveForward(float amount){
    position += viewDirection * amount;
}

void Camera::moveRight(float amount){
    glm::vec3 rightDirection = glm::normalize(glm::cross(viewDirection,upDirection));
    position += rightDirection * amount;
}

void Camera::moveUp(float amount){
    position += upDirection * amount;
}

void Camera::printCameraInfo(){
    std::cout << "position: " << position.x << " " << position.y << " " << position.z << std::endl;
    std::cout << "viewDirection: " << viewDirection.x << " " << viewDirection.y << " " << viewDirection.z << std::endl;
    std::cout << "upDirection: " << upDirection.x << " " << upDirection.y << " " << upDirection.z << std::endl;
    glm::vec3 rightDirection = getRightDirection();
    std::cout << "rightDirection: " << rightDirection.x << " " << rightDirection.y << " " << rightDirection.z << std::endl;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getViewDirection() const{
    return viewDirection;
}

glm::vec3 Camera::getUpDirection() const{
    return upDirection;
}

glm::vec3 Camera::getRightDirection() const{
    return glm::normalize(glm::cross(viewDirection,upDirection));
}