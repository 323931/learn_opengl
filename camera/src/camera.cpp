#include "camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 0.0f), 
    viewDirection(0.0f, 0.0f, -1.0f), 
    upDirection(0.0f, 1.0f, 0.0f){
}

Camera::~Camera()
{
}