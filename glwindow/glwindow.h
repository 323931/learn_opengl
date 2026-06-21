#pragma once

// #include <GL/gl.h>
#include <GL/gl.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <camera.h>
#include <glm/glm.hpp>
#include <material.h>
#include <renderer.h>
#include <shaderprogram.h>

#include <vector>

class QKeyEvent;
class QMouseEvent;

enum class MaterialType {
    SolidColor,
    Lighting
};

struct RenderItem {
    MaterialType materialType = MaterialType::Lighting;
    const Renderable* renderable = nullptr;
};

class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLWindow(QWidget* parent = nullptr);
    void setLightPosition(float x, float y, float z);
    glm::vec3 lightPosition() const;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void sendDataToOpengl();
    void sendAnotherTriangle();

    ~GLWindow();

private:
    void configVao(GLuint vaoId, GLuint vboId, GLuint indexBufferId);
    void bindModelMatrixToVao(GLuint vaoId, GLuint bufferId);
    void updateModelMatrix();
    void installShaders();
    QString readShaderCode(const QString& path);
    void initializeMaterials();
    void initializeRenderItems();
    void drawRenderItem(const RenderItem& item, const FrameUniforms& frame);

    //cube 
    GLuint cubeVaoId_ = 0;
    GLuint cubeVboId_ = 0;
    GLuint cubeIndexBufferId_ = 0;
    GLuint cubeFullTransformMartixBufferId_ = 0;
    GLuint cubeModelMatrixBufferId_ = 0;
    GLuint lightModelMatrixBufferId_ = 0;
    GLuint cubeIndexCount_ = 0;
    //aroow
    GLuint arrowVaoId_ = 0;
    GLuint arrowVboId_ = 0;
    GLuint arrowIndexBufferId_ = 0;
    GLuint arrowFullTransformMartixBufferId_ = 0;
    GLuint arrowModelMatrixBufferId_ = 0;
    GLsizei arrowIndexCount_ = 0;
    //arrow normal line
    GLuint arrowNormalLineIndexCount_ = 0;
    //plane
    GLuint planeVaoId_ = 0;
    GLuint planeFullTransformMartixBufferId_ = 0;
    GLuint planeModelMatrixBufferId_ = 0;
    GLuint planeIndexCount_ = 0;
    //plane normal line
    GLuint planeNormalLineIndexCount_ = 0;

    //只用一个缓存区来存cube和arrow
    GLuint totalVboId_ = 0;
    GLuint totalIndexBufferId_ = 0;

private:
    const float per_step = 0.1f;
    int triangle_num = 0;
    int vertex_num_per_triangle = 3;
    int float_num_per_vertex = 6;
    int byte_size_per_triangle = vertex_num_per_triangle*float_num_per_vertex*sizeof(float);

private:
    ShaderProgram lightingShader_;
    ShaderProgram solidColorShader_;
    SolidColorMaterial solidColorMaterial_;
    LightingMaterial lightingMaterial_;
    Renderer renderer_;
    GpuMesh cubeMesh_;
    GpuMesh arrowMesh_;
    GpuMesh planeMesh_;
    GpuMesh arrowNormalLineMesh_;
    GpuMesh planeNormalLineMesh_;
    Renderable lightRenderable_;
    Renderable cubeRenderable_;
    Renderable arrowRenderable_;
    Renderable planeRenderable_;
    Renderable arrowNormalLineRenderable_;
    Renderable planeNormalLineRenderable_;
    std::vector<RenderItem> renderItems_;
    Camera camera_;
    SceneLighting lighting_;
};
