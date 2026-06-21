#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <shaderprogram.h>

struct PointLight {
    glm::vec3 position = glm::vec3(0.0f, 4.5f, -3.5f);
    glm::vec3 color = glm::vec3(1.0f, 0.92f, 0.78f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

struct SceneLighting {
    glm::vec3 ambientLight = glm::vec3(0.08f, 0.07f, 0.06f);
    PointLight pointLight;
};

struct FrameUniforms {
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::vec3 viewPositionWorld = glm::vec3(0.0f);
};

struct SolidColorMaterial {
    ShaderProgram* shader = nullptr;

    void use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const;
};

struct LightingMaterial {
    ShaderProgram* shader = nullptr;
    const SceneLighting* lighting = nullptr;

    void use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const;
};
