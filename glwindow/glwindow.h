#pragma once

// #include <GL/gl.h>
#include <GL/gl.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <camera.h>
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
    void sendDataToOpengl();
public:
    void sendAnotherTriangle();
    ~GLWindow();
private:
    void installShaders();
    QString readShaderCode(const QString& path);

    GLuint programId_ = 0;
    GLuint vaoId_ = 0;
    GLuint vboId_ = 0;
    GLuint indexBufferId_ = 0;
    GLuint offsetBufferId_ = 0;
    GLuint fullTransformMartixBufferId_ = 0;
private:
    const float per_step = 0.1f;
    int triangle_num = 0;
    int vertex_num_per_triangle = 3;
    int float_num_per_vertex = 6;
    int byte_size_per_triangle = vertex_num_per_triangle*float_num_per_vertex*sizeof(float);

private:
    Camera camera_;
};
