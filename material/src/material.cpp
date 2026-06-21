#include "material.h"

void SolidColorMaterial::use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const
{
    if (shader == nullptr) {
        return;
    }

    shader->use(gl);
    shader->setMat4(gl, "viewMatrix", frame.viewMatrix);
    shader->setMat4(gl, "projectionMatrix", frame.projectionMatrix);
}

void LightingMaterial::use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const
{
    if (shader == nullptr || lighting == nullptr) {
        return;
    }

    shader->use(gl);
    shader->setVec3(gl, "ambientLight", lighting->ambientLight);
    shader->setVec3(gl, "lightPosition", lighting->pointLight.position);
    shader->setVec3(gl, "lightColor", lighting->pointLight.color);
    shader->setFloat(gl, "lightConstant", lighting->pointLight.constant);
    shader->setFloat(gl, "lightLinear", lighting->pointLight.linear);
    shader->setFloat(gl, "lightQuadratic", lighting->pointLight.quadratic);
    shader->setVec3(gl, "viewPositionWorld", frame.viewPositionWorld);
    shader->setMat4(gl, "viewMatrix", frame.viewMatrix);
    shader->setMat4(gl, "projectionMatrix", frame.projectionMatrix);
}
