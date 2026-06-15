#include "glwindow.h"
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
#include <vector>
#include <shapeGenerator.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QMouseEvent>
#include <QKeyEvent>

using glm::mat4;

GLWindow::GLWindow(QWidget* parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    setFocusPolicy(Qt::StrongFocus);
}

bool GLWindow::checkStatus(GLuint objectId,
    void (QOpenGLFunctions_3_3_Core::*getStatusFunc)(GLuint, GLenum, GLint*),
    void (QOpenGLFunctions_3_3_Core::*getInfoLogFunc)(GLuint, GLsizei, GLsizei*, GLchar*),
    GLenum statusType){
        GLint status;
        (this->*getStatusFunc)(objectId, statusType, &status);
        if(status != GL_TRUE){
            GLint logLength;
            (this->*getStatusFunc)(objectId, GL_INFO_LOG_LENGTH, &logLength);
            if(logLength > 0){
                std::vector<char> log(logLength);
                (this->*getInfoLogFunc)(objectId, logLength, nullptr, log.data());
                std::cerr << "Status check failed: " << log.data() << std::endl;
                return false;
            }
        }
        return true;
}

bool GLWindow::checkShaderCompile(GLuint shaderId){
    return checkStatus(shaderId,
                       &QOpenGLFunctions_3_3_Core::glGetShaderiv,
                       &QOpenGLFunctions_3_3_Core::glGetShaderInfoLog,
                       GL_COMPILE_STATUS);
}

bool GLWindow::checkShaderLink(GLuint programId){
    return checkStatus(programId,
                       &QOpenGLFunctions_3_3_Core::glGetProgramiv,
                       &QOpenGLFunctions_3_3_Core::glGetProgramInfoLog,
                       GL_LINK_STATUS);
}

void GLWindow::sendDataToOpengl(){
    ShapeData cube = ShapeGenerator::loadModel("models/teapot.obj",0.03f);
    ShapeData arrow =  ShapeGenerator::createArrow();
    ShapeData plane = ShapeGenerator::createPlane(20);

    cubeIndexCount_ = cube.index_num;
    arrowIndexCount_ = arrow.index_num;
    planeIndexCount_ = plane.index_num;
    const GLushort arrowVertexOffset = cube.vertex_num;
    GLushort planeVertexOffset = cube.vertex_num + arrow.vertex_num;

    glGenBuffers(1, &totalVboId_);
    glBindBuffer(GL_ARRAY_BUFFER, totalVboId_);
    glBufferData(GL_ARRAY_BUFFER, cube.vertices_size()+arrow.vertices_size()+plane.vertices_size(), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cube.vertices_size(), cube.vertices);
    glBufferSubData(GL_ARRAY_BUFFER, cube.vertices_size(), arrow.vertices_size(), arrow.vertices);
    glBufferSubData(GL_ARRAY_BUFFER, cube.vertices_size()+arrow.vertices_size(), plane.vertices_size(), plane.vertices);


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

    glGenBuffers(1, &totalIndexBufferId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, totalIndexBufferId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indices_size()+arrow.indices_size()+plane.indices_size(), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, cube.indices_size(), cube.indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, cube.indices_size(), arrow.indices_size(), arrowIndices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, cube.indices_size()+arrow.indices_size(), plane.indices_size(), planeIndices.data());
    
    glGenVertexArrays(1, &cubeVaoId_);
    glGenVertexArrays(1, &arrowVaoId_);
    glGenVertexArrays(1, &planeVaoId_);

    configVao(cubeVaoId_, totalVboId_, totalIndexBufferId_);
    configVao(arrowVaoId_, totalVboId_, totalIndexBufferId_);
    configVao(planeVaoId_, totalVboId_, totalIndexBufferId_);

    glBindVertexArray(0);

    glGenBuffers(1, &cubeFullTransformMartixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, cubeFullTransformMartixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4)*2, nullptr, GL_DYNAMIC_DRAW);


    glGenBuffers(1, &arrowFullTransformMartixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, arrowFullTransformMartixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &planeFullTransformMartixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, planeFullTransformMartixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);

    cube.release();
    arrow.release();
    plane.release();
}

void GLWindow::sendCubeToOpengl(){
    //send cube to opengl
    ShapeData shape =  ShapeGenerator::createCube();
    cubeIndexCount_ = shape.index_num;
    //在gpu上申请一个 vao，vertex array object ，句柄放到vaoid。
    glGenVertexArrays(1, &cubeVaoId_);
    //激活这个vao，之后对vbo，顶点属性的操作都会写进这个vao
    glBindVertexArray(cubeVaoId_);

    //在gpu申请一个vbo，vertex buffer object，句柄在vboid
    glGenBuffers(1, &cubeVboId_);
    //把刚创建的vbo 绑定要 GL_ARRAY_BUFFER这个绑定点，OpenGL 很多操作都针对「当前绑定的 buffer」，所以先 bind 再 glBufferData。
    glBindBuffer(GL_ARRAY_BUFFER, cubeVboId_);
    //分配空间
    glBufferData(GL_ARRAY_BUFFER, shape.vertices_size(), shape.vertices, GL_STATIC_DRAW);

    //打开顶点属性 0（我们 shader 里的 position）。
    //关掉的话，即使 VBO 有数据，管线也不会读这个属性。
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, color)));

    glGenBuffers(1, &cubeIndexBufferId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices_size(), shape.indices, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeFullTransformMartixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, cubeFullTransformMartixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4)*2, nullptr, GL_DYNAMIC_DRAW);

    // glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*0));
    // glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*4));
    // glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*8));
    // glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*12));

    // glEnableVertexAttribArray(2);
    // glEnableVertexAttribArray(3);
    // glEnableVertexAttribArray(4);
    // glEnableVertexAttribArray(5);
    // glVertexAttribDivisor(2, 1);
    // glVertexAttribDivisor(3, 1);
    // glVertexAttribDivisor(4, 1);
    // glVertexAttribDivisor(5, 1);

    for(size_t i = 0; i<4;++i){
        glVertexAttribPointer(i+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*i*4));
        glEnableVertexAttribArray(i+2);
        glVertexAttribDivisor(i+2, 1);
    }

    glBindVertexArray(0);

    std::cerr << "Cube uploaded: " << shape.vertex_num << " vertices, "
              << shape.index_num << " indices"
              << " v2=(" << shape.vertices[2].position.x << ","
              << shape.vertices[2].position.y << ","
              << shape.vertices[2].position.z << ")"
              << " v21=(" << shape.vertices[21].position.x << ","
              << shape.vertices[21].position.y << ","
              << shape.vertices[21].position.z << ")\n";

    shape.release();
}

void GLWindow::sendArrowToOpengl(){
    //send arrow to opengl
    ShapeData shape = ShapeGenerator::createArrow();
    arrowIndexCount_ = shape.index_num;

    glGenVertexArrays(1, &arrowVaoId_);
    glBindVertexArray(arrowVaoId_);

    glGenBuffers(1, &arrowVboId_);
    glBindBuffer(GL_ARRAY_BUFFER, arrowVboId_);
    glBufferData(GL_ARRAY_BUFFER, shape.vertices_size(), shape.vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
    reinterpret_cast<void*>(offsetof(Vertex,color)));


    glGenBuffers(1, &arrowIndexBufferId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrowIndexBufferId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices_size(), shape.indices, GL_STATIC_DRAW);

    glGenBuffers(1, &arrowFullTransformMartixBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, arrowFullTransformMartixBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4), nullptr, GL_DYNAMIC_DRAW);
    
    
    for(size_t i = 0; i<4;++i){
        glVertexAttribPointer(i+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*i*4));
        glEnableVertexAttribArray(i+2);
        glVertexAttribDivisor(i+2, 1);
    }

    glBindVertexArray(0);


    std::cerr << "Arrow uploaded: " << shape.vertex_num << " vertices, "
            << shape.index_num << " indices"
            << " v2=(" << shape.vertices[2].position.x << ","
            << shape.vertices[2].position.y << ","
            << shape.vertices[2].position.z << ")"
            << " v21=(" << shape.vertices[21].position.x << ","
            << shape.vertices[21].position.y << ","
            << shape.vertices[21].position.z << ")\n";

    shape.release();
    
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

}

void GLWindow::bindFullTransformMartixToVao(GLuint vaoId, GLuint bufferId){
    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    for(size_t i = 0; i<4;++i){
        glVertexAttribPointer(i+2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float)*i*4));
        glEnableVertexAttribArray(i+2);
        glVertexAttribDivisor(i+2, 1);
    }
}

void GLWindow::mouseMoveEvent(QMouseEvent *event){
    glm::vec2 mousePosition = glm::vec2(event->x(), event->y());
    camera_.mousePositionUpdated(mousePosition);
    update();
}

void GLWindow::updateFullTransformMartix(){
    mat4 projectMartix = glm::perspective(glm::radians(90.0f),(float)width()/height(),0.1f,20.0f);
    mat4 worldToViewMartix = camera_.getWorldToViewMartix();

    mat4 fullTransformMartices[] = {
        projectMartix * worldToViewMartix * glm::translate(mat4(1.0f),glm::vec3(-2.8f,0.0f,-6.5f))*glm::rotate(mat4(1.0f),glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f)),
        projectMartix * worldToViewMartix * glm::translate(mat4(1.0f),glm::vec3(2.8f,0.0f,-6.75f))*glm::rotate(mat4(1.0f),glm::radians(0.0f),glm::vec3(-1.0f,1.0f,0.0f)),
    };
    glBindBuffer(GL_ARRAY_BUFFER, cubeFullTransformMartixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4)*2, fullTransformMartices);

    mat4 arrowFullTransformMartices[] = {
        projectMartix * worldToViewMartix * glm::translate(mat4(1.0f),glm::vec3(-1.0f,-1.0f,-5.5f))*glm::rotate(mat4(1.0f),glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f)) *glm::scale(mat4(1.0f), glm::vec3(0.5f)),
    };

    glBindBuffer(GL_ARRAY_BUFFER, arrowFullTransformMartixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4), arrowFullTransformMartices);


    mat4 planeFullTransformMartices[] = {
        projectMartix * worldToViewMartix * glm::translate(mat4(1.0f),glm::vec3(-1.0f,-1.3f,-3.0f))*glm::rotate(mat4(1.0f),glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f)),
    };

    glBindBuffer(GL_ARRAY_BUFFER, planeFullTransformMartixBufferId_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mat4), planeFullTransformMartices);

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

    if (programId_ == 0) {
        std::cerr << "Shader program not ready — check compile/link messages above\n";
    } else {
        std::cerr << "Shader program OK, id=" << programId_ << "\n";
    }
    
    updateFullTransformMartix();
    
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


    GLint uniformAmbientLightLocation =  glGetUniformLocation(programId_, "ambientLight");
    glm::vec3 ambientLight = glm::vec3(0.7f, 0.5f, 0.4f);
    glUniform3fv(uniformAmbientLightLocation,1, &ambientLight[0]);

    glUseProgram(programId_);
    glBindVertexArray(cubeVaoId_);
    bindFullTransformMartixToVao(cubeVaoId_, cubeFullTransformMartixBufferId_);

    //注意 矩阵作用的顺序是 ： 旋转后，再平移，最后投影
    //所以创建顺序可以这样写，避免多做一些不必要的乘法运算
    // GLint fullTransformMartixLocation = glGetUniformLocation(programId_, "fullTransformMartix");
    // if (fullTransformMartixLocation < 0) {
    //     std::cerr << "fullTransformMartix uniform not found — run cmake --build after editing shader\n";
    //     return;
    // }

    // mat4 fullTransformMartix;
    // mat4 projectionMartix = glm::perspective(glm::radians(60.0f), (float)width() / height(), 0.1f, 10.0f);

    // //first cude
    // mat4 projectTransformMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(-1.0f, 0.0f, -3.0f));
    // mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(36.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // fullTransformMartix = projectionMartix * projectTransformMatrix * rotateMatrix;

    // glUniformMatrix4fv(fullTransformMartixLocation, 1, GL_FALSE, &fullTransformMartix[0][0]);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
    

    // //second cude    
    // projectTransformMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(1.0f, 0.0f, -3.75f));
    // rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(126.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // fullTransformMartix = projectionMartix * projectTransformMatrix * rotateMatrix;

    // glUniformMatrix4fv(fullTransformMartixLocation, 1, GL_FALSE, &fullTransformMartix[0][0]);

    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

    updateFullTransformMartix();
    glDrawElementsInstanced(GL_TRIANGLES, cubeIndexCount_, GL_UNSIGNED_SHORT, nullptr, 2);

    glBindVertexArray(arrowVaoId_);
    bindFullTransformMartixToVao(arrowVaoId_, arrowFullTransformMartixBufferId_);
    glDrawElementsInstanced(GL_TRIANGLES, arrowIndexCount_, GL_UNSIGNED_SHORT, (void*)(cubeIndexCount_*sizeof(GLushort)), 1);
    
    glBindVertexArray(planeVaoId_);
    bindFullTransformMartixToVao(planeVaoId_, planeFullTransformMartixBufferId_);
    glDrawElementsInstanced(GL_TRIANGLES, planeIndexCount_, GL_UNSIGNED_SHORT, (void*)(cubeIndexCount_*sizeof(GLushort)+arrowIndexCount_*sizeof(GLushort)), 1);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after draw: 0x" << std::hex << err << std::dec << "\n";
    }    
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
    const GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    const QByteArray vertexSource = readShaderCode(":/shaders/vertexshader.vert").toUtf8();
    const QByteArray fragmentSource = readShaderCode(":/shaders/fragmentshader.frag").toUtf8();

    if (vertexSource.isEmpty() || fragmentSource.isEmpty()) {
        std::cerr << "Failed to load shader files from Qt resources\n";
        return;
    }

    const char* vertexPtr = vertexSource.constData();
    const char* fragmentPtr = fragmentSource.constData();
    glShaderSource(vertexShaderId, 1, &vertexPtr, nullptr);
    glShaderSource(fragmentShaderId, 1, &fragmentPtr, nullptr);

    glCompileShader(vertexShaderId);
    glCompileShader(fragmentShaderId);

    if(!checkShaderCompile(vertexShaderId) || !checkShaderCompile(fragmentShaderId)){
        std::cerr << "Failed to compile shader\n";
        return;
    }

    programId_ = glCreateProgram();
    glAttachShader(programId_, vertexShaderId);
    glAttachShader(programId_, fragmentShaderId);
    glLinkProgram(programId_);

    if(!checkShaderLink(programId_)){
        std::cerr << "Failed to link shader\n";
        exit(EXIT_FAILURE);
    }

    glUseProgram(programId_);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

GLWindow::~GLWindow(){
    glDeleteProgram(programId_);
    glDeleteVertexArrays(1, &cubeVaoId_);
    glDeleteBuffers(1, &cubeVboId_);
    glDeleteBuffers(1, &cubeIndexBufferId_);
    glDeleteBuffers(1, &cubeFullTransformMartixBufferId_);
}
