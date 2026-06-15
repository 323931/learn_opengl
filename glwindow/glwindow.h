#pragma once

// #include <GL/gl.h>
#include <GL/gl.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <camera.h>

class QKeyEvent;
class QMouseEvent;

class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLWindow(QWidget* parent = nullptr);
    bool checkStatus(GLuint objectId,
                     void (QOpenGLFunctions_3_3_Core::*getStatusFunc)(GLuint, GLenum, GLint*),
                     void (QOpenGLFunctions_3_3_Core::*getInfoLogFunc)(GLuint, GLsizei, GLsizei*, GLchar*),
                     GLenum statusType);
    bool checkShaderCompile(GLuint shaderId);
    bool checkShaderLink(GLuint programId);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void sendDataToOpengl();
    void sendAnotherTriangle();
    void sendCubeToOpengl();
    void sendArrowToOpengl();

    ~GLWindow();

private:
    void configVao(GLuint vaoId, GLuint vboId, GLuint indexBufferId);
    void bindFullTransformMartixToVao(GLuint vaoId, GLuint bufferId);
    void updateFullTransformMartix();
    void installShaders();
    QString readShaderCode(const QString& path);

    GLuint programId_ = 0;
    //cube 
    GLuint cubeVaoId_ = 0;
    GLuint cubeVboId_ = 0;
    GLuint cubeIndexBufferId_ = 0;
    GLuint cubeFullTransformMartixBufferId_ = 0;
    GLuint cubeIndexCount_ = 0;
    //aroow
    GLuint arrowVaoId_ = 0;
    GLuint arrowVboId_ = 0;
    GLuint arrowIndexBufferId_ = 0;
    GLuint arrowFullTransformMartixBufferId_ = 0;
    GLsizei arrowIndexCount_ = 0;
    //plane
    GLuint planeVaoId_ = 0;
    GLuint planeFullTransformMartixBufferId_ = 0;
    GLuint planeIndexCount_ = 0;

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
    Camera camera_;
};
