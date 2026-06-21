#include "glwindow.h"
#include "renderer.h"
#include "shapedata.h"
#include "vertex.h"

#include <GL/gl.h>
#include <GL/glext.h>
#include <QFile>
#include <cstddef>
#include <QSurfaceFormat>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <qopenglext.h>
#include <shapeGenerator.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QMouseEvent>
#include <QKeyEvent>

using glm::mat4;

namespace {
constexpr GLsizei kSceneCubeCount = 6;
constexpr GLsizei kSceneArrowCount = 3;
constexpr float kFloorY = -1.3f;
}

GLWindow::GLWindow(QWidget* parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    setFocusPolicy(Qt::StrongFocus);
}

void GLWindow::setLightPosition(float x, float y, float z)
{
    lighting_.pointLight.position = glm::vec3(x, y, z);
    update();
}

glm::vec3 GLWindow::lightPosition() const
{
    return lighting_.pointLight.position;
}

void GLWindow::sendDataToOpengl(){
    ShapeData cube = ShapeGenerator::createCube();
    ShapeData arrow =  ShapeGenerator::createArrow();
    ShapeData plane = ShapeGenerator::createPlane(32);
    ShapeData arrowNormalLine = ShapeGenerator::createNormalLine(arrow);
    ShapeData planeNormalLine = ShapeGenerator::createNormalLine(plane);

    cubeIndexCount_ = cube.index_num;
    arrowIndexCount_ = arrow.index_num;
    planeIndexCount_ = plane.index_num;
    arrowNormalLineIndexCount_ = arrowNormalLine.index_num;
    planeNormalLineIndexCount_ = planeNormalLine.index_num;

    const GLushort arrowVertexOffset = cube.vertex_num;
    GLushort planeVertexOffset = cube.vertex_num + arrow.vertex_num;
    GLushort arrowNormalLineVertexOffset = cube.vertex_num + arrow.vertex_num + plane.vertex_num;
    GLushort planeNormalLineVertexOffset = cube.vertex_num + arrow.vertex_num + plane.vertex_num + arrowNormalLine.vertex_num;

    glGenBuffers(1, &totalVboId_);
    glBindBuffer(GL_ARRAY_BUFFER, totalVboId_);
    glBufferData(GL_ARRAY_BUFFER, cube.vertices_size()+arrow.vertices_size()+plane.vertices_size()+arrowNormalLine.vertices_size()+planeNormalLine.vertices_size(), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 
        0, 
        cube.vertices_size(), 
        cube.vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 
        cube.vertices_size(), 
        arrow.vertices_size(), 
        arrow.vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 
        cube.vertices_size()+arrow.vertices_size(), 
        plane.vertices_size(), 
        plane.vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 
        cube.vertices_size()+arrow.vertices_size()+plane.vertices_size(), 
        arrowNormalLine.vertices_size(), 
        arrowNormalLine.vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 
        cube.vertices_size()+arrow.vertices_size()+plane.vertices_size()+arrowNormalLine.vertices_size(), 
        planeNormalLine.vertices_size(), 
        planeNormalLine.vertices);


    std::vector<GLushort> arrowIndices;
    arrowIndices.reserve(arrow.index_num);
    for (int i = 0; i < arrow.index_num; ++i) {
        arrowIndices.push_back(arrow.indices[i] + arrowVertexOffset);
    }
   
    std::vector<GLushort> planeIndices;
    planeIndices.reserve(plane.index_num);
    for(int i = 0;i<plane.index_num;++i){
        planeIndices.push_back(plane.indices[i] + planeVertexOffset);
    }

    std::vector<GLushort> arrowNormalLineIndices;
    arrowNormalLineIndices.reserve(arrowNormalLine.index_num);
    for(int i = 0;i<arrowNormalLine.index_num;++i){
        arrowNormalLineIndices.push_back(arrowNormalLine.indices[i] + arrowNormalLineVertexOffset);
    }

    std::vector<GLushort> planeNormalLineIndices;
    planeNormalLineIndices.reserve(planeNormalLine.index_num);
    for(int i = 0;i<planeNormalLine.index_num;++i){
        planeNormalLineIndices.push_back(planeNormalLine.indices[i] + planeNormalLineVertexOffset);
    }

    glGenBuffers(1, &totalIndexBufferId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, totalIndexBufferId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indices_size()+arrow.indices_size()+plane.indices_size()+arrowNormalLine.indices_size()+planeNormalLine.indices_size(), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 
        0, 
        cube.indices_size(),
        cube.indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 
        cube.indices_size(), 
        arrow.indices_size(), 
        arrowIndices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 
        cube.indices_size()+arrow.indices_size(), 
        plane.indices_size(),
        planeIndices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 
        cube.indices_size()+arrow.indices_size()+plane.indices_size(), 
        arrowNormalLine.indices_size(), 
        arrowNormalLineIndices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 
        cube.indices_size()+arrow.indices_size()+plane.indices_size()+arrowNormalLine.indices_size(), 
        planeNormalLine.indices_size(), 
        planeNormalLineIndices.data());

    glGenVertexArrays(1, &cubeVaoId_);
    glGenVertexArrays(1, &arrowVaoId_);
    glGenVertexArrays(1, &planeVaoId_);

    configVao(cubeVaoId_, totalVboId_, totalIndexBufferId_);
    configVao(arrowVaoId_, totalVboId_, totalIndexBufferId_);
    configVao(planeVaoId_, totalVboId_, totalIndexBufferId_);

    glBindVertexArray(0);

    glGenBuffers(1, &cubeModelMatrixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, cubeModelMatrixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * kSceneCubeCount, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &lightModelMatrixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, lightModelMatrixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &arrowModelMatrixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, arrowModelMatrixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * kSceneArrowCount, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &planeModelMatrixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, planeModelMatrixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);

    cubeMesh_ = GpuMesh{
        cubeVaoId_,
        GL_TRIANGLES,
        static_cast<GLsizei>(cubeIndexCount_),
        GL_UNSIGNED_SHORT,
        nullptr
    };
    arrowMesh_ = GpuMesh{
        arrowVaoId_,
        GL_TRIANGLES,
        arrowIndexCount_,
        GL_UNSIGNED_SHORT,
        reinterpret_cast<void*>(cubeIndexCount_ * sizeof(GLushort))
    };
    planeMesh_ = GpuMesh{
        planeVaoId_,
        GL_TRIANGLES,
        static_cast<GLsizei>(planeIndexCount_),
        GL_UNSIGNED_SHORT,
        reinterpret_cast<void*>((cubeIndexCount_ + arrowIndexCount_) * sizeof(GLushort))
    };
    arrowNormalLineMesh_ = GpuMesh{
        arrowVaoId_,
        GL_LINES,
        static_cast<GLsizei>(arrowNormalLineIndexCount_),
        GL_UNSIGNED_SHORT,
        reinterpret_cast<void*>((cubeIndexCount_ + arrowIndexCount_ + planeIndexCount_) * sizeof(GLushort))
    };
    planeNormalLineMesh_ = GpuMesh{
        planeVaoId_,
        GL_LINES,
        static_cast<GLsizei>(planeNormalLineIndexCount_),
        GL_UNSIGNED_SHORT,
        reinterpret_cast<void*>(
            (cubeIndexCount_ + arrowIndexCount_ + planeIndexCount_ + arrowNormalLineIndexCount_) * sizeof(GLushort)
        )
    };

    lightRenderable_ = Renderable{&cubeMesh_, lightModelMatrixBufferId_, 1};
    cubeRenderable_ = Renderable{&cubeMesh_, cubeModelMatrixBufferId_, kSceneCubeCount};
    arrowRenderable_ = Renderable{&arrowMesh_, arrowModelMatrixBufferId_, kSceneArrowCount};
    planeRenderable_ = Renderable{&planeMesh_, planeModelMatrixBufferId_, 1};
    arrowNormalLineRenderable_ = Renderable{&arrowNormalLineMesh_, arrowModelMatrixBufferId_, kSceneArrowCount};
    planeNormalLineRenderable_ = Renderable{&planeNormalLineMesh_, planeModelMatrixBufferId_, 1};
    initializeRenderItems();

    cube.release();
    arrow.release();
    plane.release();
    arrowNormalLine.release();
    planeNormalLine.release();
}

void GLWindow::sendAnotherTriangle(){
    //制作一个三角形动态移动的效果，所以一次需要传递三个顶点，第一次定位在左上角
    //一直向右边移动，所以传递的顶点分别是 -1.0，+1.0f，-1.0f，0.0f, -0.9f,1.0f;
    if(triangle_num >= 20){
        return;
    }

    float this_x = -1.0f + triangle_num * per_step;

    float vertices[] = {
        this_x, +1.0f, +0.9f,
        +0.0f, +0.0f, +1.0f,//1

        this_x, +0.0f, +0.9f,
        +0.0f, +0.0f, +1.0f,//2

        this_x + per_step, +1.0f, +0.9f,
        +0.0f, +0.0f, +1.0f,//3
    };

    glBufferSubData(GL_ARRAY_BUFFER, triangle_num * byte_size_per_triangle, byte_size_per_triangle, vertices);
    triangle_num++;
}

void GLWindow::configVao(GLuint vaoId, GLuint vboId, GLuint indexBufferId){
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

    //绑定顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, normal)));

}

void GLWindow::bindModelMatrixToVao(GLuint vaoId, GLuint bufferId){
    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    for(size_t i = 0; i<4;++i){
        glVertexAttribPointer(i+3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*i*4));
        glEnableVertexAttribArray(i+3);
        glVertexAttribDivisor(i+3, 1);
    }
}

void GLWindow::mouseMoveEvent(QMouseEvent *event){
    glm::vec2 mousePosition = glm::vec2(event->x(), event->y());
    camera_.mousePositionUpdated(mousePosition);
    update();
}

void GLWindow::updateModelMatrix(){
    mat4 cubeModelMatrices[] = {
        glm::translate(mat4(1.0f),glm::vec3(0.0f,kFloorY + 0.35f,-4.6f))
            * glm::scale(mat4(1.0f), glm::vec3(1.35f,0.35f,1.35f)),
        glm::translate(mat4(1.0f),glm::vec3(-3.2f,kFloorY + 1.4f,-5.6f))
            * glm::rotate(mat4(1.0f),glm::radians(12.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.45f,1.4f,0.45f)),
        glm::translate(mat4(1.0f),glm::vec3(3.2f,kFloorY + 1.1f,-5.2f))
            * glm::rotate(mat4(1.0f),glm::radians(-16.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.45f,1.1f,0.45f)),
        glm::translate(mat4(1.0f),glm::vec3(-1.4f,kFloorY + 0.5f,-7.1f))
            * glm::rotate(mat4(1.0f),glm::radians(20.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.9f,0.5f,0.5f)),
        glm::translate(mat4(1.0f),glm::vec3(1.8f,kFloorY + 0.45f,-7.3f))
            * glm::rotate(mat4(1.0f),glm::radians(-25.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.75f,0.45f,0.75f)),
        glm::translate(mat4(1.0f),glm::vec3(-4.0f,kFloorY + 0.25f,-2.5f))
            * glm::rotate(mat4(1.0f),glm::radians(35.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.4f,0.25f,1.2f)),
    };
    glBindBuffer(GL_ARRAY_BUFFER, cubeModelMatrixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeModelMatrices), cubeModelMatrices);

    mat4 lightModelMatrix =
        glm::translate(mat4(1.0f), lighting_.pointLight.position)
        * glm::scale(mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f));
    glBindBuffer(GL_ARRAY_BUFFER, lightModelMatrixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4), &lightModelMatrix);

    mat4 arrowModelMatrices[] = {
        glm::translate(mat4(1.0f),glm::vec3(0.0f,kFloorY + 1.25f,-4.6f))
            * glm::rotate(mat4(1.0f),glm::radians(25.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::rotate(mat4(1.0f),glm::radians(35.0f),glm::vec3(1.0f,0.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(1.55f)),
        glm::translate(mat4(1.0f),glm::vec3(-3.6f,kFloorY + 0.32f,-3.6f))
            * glm::rotate(mat4(1.0f),glm::radians(-25.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.85f)),
        glm::translate(mat4(1.0f),glm::vec3(3.4f,kFloorY + 0.32f,-4.0f))
            * glm::rotate(mat4(1.0f),glm::radians(205.0f),glm::vec3(0.0f,1.0f,0.0f))
            * glm::scale(mat4(1.0f), glm::vec3(0.75f)),
    };

    glBindBuffer(GL_ARRAY_BUFFER, arrowModelMatrixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arrowModelMatrices), arrowModelMatrices);


    mat4 planeModelMatrices[] = {
        glm::translate(mat4(1.0f),glm::vec3(-1.0f,kFloorY,-4.0f)),
    };

    glBindBuffer(GL_ARRAY_BUFFER, planeModelMatrixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4), planeModelMatrices);
}

void GLWindow::keyPressEvent(QKeyEvent *event){
    float amount = 0.1f;
    switch (event->key()) {
        case Qt::Key_W:
            camera_.moveForward(amount);
            break;
        case Qt::Key_S:
            camera_.moveForward(-amount);
            break;
        case Qt::Key_A:
            camera_.moveRight(-amount);
            break;
        case Qt::Key_D:
            camera_.moveRight(amount);
            break;
        case Qt::Key_Q:
            camera_.moveUp(amount);
            break;
        case Qt::Key_E:
            camera_.moveUp(-amount);
            break;
        case Qt::Key_P:
            camera_.printCameraInfo();
            break;
        default:
            break;
    };
    update();
}

void GLWindow::initializeGL(){
    // OpenGL 函数 只能在 initializeGL() 之后 调用。构造函数里 GPU 还没准备好，一调 glEnable 就可能 段错误
    initializeOpenGLFunctions();

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //不渲染背面--鞋带算法
    glEnable(GL_CULL_FACE);

    installShaders();
    sendDataToOpengl();

    if (lightingShader_.id() == 0 || solidColorShader_.id() == 0) {
        std::cerr << "Shader program not ready — check compile/link messages above\n";
    } else {
        std::cerr << "Shader program OK, id=" << lightingShader_.id() << "\n";
    }
    
    updateModelMatrix();
    
    update(); // 触发首帧 paintGL（静态立方体也要重绘一次）
}

void GLWindow::resizeGL(int /*w*/, int /*h*/)
{
    // Viewport is set in paintGL(), matching the video workflow.
}

void GLWindow::paintGL()
{
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const FrameUniforms frame{
        camera_.getWorldToViewMartix(),
        glm::perspective(glm::radians(90.0f), static_cast<float>(width()) / height(), 0.1f, 20.0f),
        camera_.getPosition()
    };
    updateModelMatrix();

    for (const RenderItem& item : renderItems_) {
        drawRenderItem(item, frame);
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after draw: 0x" << std::hex << err << std::dec << "\n";
    }
}

void GLWindow::useSolidColorMaterial(const FrameUniforms& frame)
{
    solidColorShader_.use(*this);
    solidColorShader_.setMat4(*this, "viewMatrix", frame.viewMatrix);
    solidColorShader_.setMat4(*this, "projectionMatrix", frame.projectionMatrix);
}

void GLWindow::useLightingMaterial(const FrameUniforms& frame)
{
    lightingShader_.use(*this);
    lightingShader_.setVec3(*this, "ambientLight", lighting_.ambientLight);
    lightingShader_.setVec3(*this, "lightPosition", lighting_.pointLight.position);
    lightingShader_.setVec3(*this, "lightColor", lighting_.pointLight.color);
    lightingShader_.setFloat(*this, "lightConstant", lighting_.pointLight.constant);
    lightingShader_.setFloat(*this, "lightLinear", lighting_.pointLight.linear);
    lightingShader_.setFloat(*this, "lightQuadratic", lighting_.pointLight.quadratic);
    lightingShader_.setVec3(*this, "viewPositionWorld", frame.viewPositionWorld);
    lightingShader_.setMat4(*this, "viewMatrix", frame.viewMatrix);
    lightingShader_.setMat4(*this, "projectionMatrix", frame.projectionMatrix);
}

void GLWindow::initializeRenderItems()
{
    renderItems_ = {
        RenderItem{MaterialType::SolidColor, &lightRenderable_},
        RenderItem{MaterialType::Lighting, &cubeRenderable_},
        RenderItem{MaterialType::Lighting, &arrowRenderable_},
        RenderItem{MaterialType::Lighting, &arrowNormalLineRenderable_},
        RenderItem{MaterialType::Lighting, &planeRenderable_},
        RenderItem{MaterialType::Lighting, &planeNormalLineRenderable_},
    };
}

void GLWindow::drawRenderItem(const RenderItem& item, const FrameUniforms& frame)
{
    if (item.renderable == nullptr) {
        return;
    }

    switch (item.materialType) {
        case MaterialType::SolidColor:
            useSolidColorMaterial(frame);
            break;
        case MaterialType::Lighting:
            useLightingMaterial(frame);
            break;
    }

    renderer_.drawInstanced(*this, *item.renderable);
}

QString GLWindow::readShaderCode(const QString& path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return {};
    }
    return file.readAll();
}

void GLWindow::installShaders()
{
    const QByteArray vertexSource = readShaderCode(":/shaders/vertexshader.vert").toUtf8();
    const QByteArray fragmentSource = readShaderCode(":/shaders/fragmentshader.frag").toUtf8();
    const QByteArray throughColorFragmentSource = readShaderCode(":/shaders/throughColorFragmentshader.frag").toUtf8();

    if (!lightingShader_.load(*this, vertexSource, fragmentSource)) {
        std::cerr << "Failed to load lighting shader program\n";
    }

    if (!solidColorShader_.load(*this, vertexSource, throughColorFragmentSource)) {
        std::cerr << "Failed to load solid color shader program\n";
    }

    solidColorShader_.use(*this);
}

GLWindow::~GLWindow(){
    makeCurrent();

    lightingShader_.destroy(*this);
    solidColorShader_.destroy(*this);

    GLuint vaos[] = { cubeVaoId_, arrowVaoId_, planeVaoId_ };
    glDeleteVertexArrays(3, vaos);

    GLuint buffers[] = {
        cubeVboId_,
        cubeIndexBufferId_,
        cubeFullTransformMartixBufferId_,
        arrowVboId_,
        arrowIndexBufferId_,
        arrowFullTransformMartixBufferId_,
        planeFullTransformMartixBufferId_,
        totalVboId_,
        totalIndexBufferId_,
        cubeModelMatrixBufferId_,
        lightModelMatrixBufferId_,
        arrowModelMatrixBufferId_,
        planeModelMatrixBufferId_
    };
    glDeleteBuffers(sizeof(buffers) / sizeof(buffers[0]), buffers);

    doneCurrent();
}
