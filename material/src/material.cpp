#include "material.h"

bool SolidColorMaterial::use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const
{
    if (shader == nullptr) {
        return false;
    }

    shader->use(gl);
    shader->setMat4(gl, "viewMatrix", frame.viewMatrix);
    shader->setMat4(gl, "projectionMatrix", frame.projectionMatrix);
    return true;
}

bool LightingMaterial::use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const
{
    if (shader == nullptr || lighting == nullptr) {
        return false;
    }

    shader->use(gl);
    shader->setVec3(gl, "ambientLight", lighting->ambientLight);
    shader->setVec3(gl, "lightPosition", lighting->pointLight.position);
    shader->setVec3(gl, "lightColor", lighting->pointLight.color);
    shader->setFloat(gl, "lightConstant", lighting->pointLight.constant);
    shader->setFloat(gl, "lightLinear", lighting->pointLight.linear);
    shader->setFloat(gl, "lightQuadratic", lighting->pointLight.quadratic);
    shader->setFloat(gl, "roughnessValue", roughness);
    shader->setInt(gl, "debugSpecularOnly", debugSpecularOnly ? 1 : 0);
    shader->setVec3(gl, "viewPositionWorld", frame.viewPositionWorld);
    shader->setMat4(gl, "viewMatrix", frame.viewMatrix);
    shader->setMat4(gl, "projectionMatrix", frame.projectionMatrix);

    const bool hasDiffuseTexture = useDiffuseTexture && diffuseTexture != nullptr && diffuseTexture->id() != 0;
    shader->setInt(gl, "useDiffuseTexture", hasDiffuseTexture ? 1 : 0);
    if (hasDiffuseTexture) {
        diffuseTexture->bind(gl, 0);
        shader->setInt(gl, "diffuseTexture", 0);
    }
    const bool hasRoughnessTexture =
        useRoughnessTexture && roughnessTexture != nullptr && roughnessTexture->id() != 0;
    shader->setInt(gl, "useRoughnessTexture", hasRoughnessTexture ? 1 : 0);
    if (hasRoughnessTexture) {
        roughnessTexture->bind(gl, 1);
        shader->setInt(gl, "roughnessTexture", 1);
    }

    return true;
}
