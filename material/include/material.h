#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <shaderprogram.h>
#include <texture2d.h>

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

class Material {
public:
    ShaderProgram* shader = nullptr;
    virtual bool use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const = 0;
    virtual ~Material() = default;
};


class SolidColorMaterial : public Material {
public:
    ~SolidColorMaterial() override = default;
    bool use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const override;
};

class LightingMaterial : public Material {
public:
    const SceneLighting* lighting = nullptr;
    float roughness = 0.25f;
    bool debugSpecularOnly = false;
    // 决定表面基础颜色。
    const Texture2D* diffuseTexture = nullptr;
    bool useDiffuseTexture = false;
    // 决定每个位置的表面粗糙程度：黑色更光滑，白色更粗糙。
    const Texture2D* roughnessTexture = nullptr;
    bool useRoughnessTexture = false;
    ~LightingMaterial() override = default;
    bool use(QOpenGLFunctions_3_3_Core& gl, const FrameUniforms& frame) const override;
};
