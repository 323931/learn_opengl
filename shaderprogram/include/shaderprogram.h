#pragma once

#include <GL/gl.h>
#include <QByteArray>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class ShaderProgram {
public:
    GLuint id() const;

    void use(QOpenGLFunctions_3_3_Core& gl) const;
    void setMat4(QOpenGLFunctions_3_3_Core& gl, const char* name, const glm::mat4& value);
    void setVec3(QOpenGLFunctions_3_3_Core& gl, const char* name, const glm::vec3& value);
    void setFloat(QOpenGLFunctions_3_3_Core& gl, const char* name, float value);
    void setInt(QOpenGLFunctions_3_3_Core& gl, const char* name, int value);

    bool load(QOpenGLFunctions_3_3_Core& gl,
              const QByteArray& vertexShaderSource,
              const QByteArray& fragmentShaderSource);
    void destroy(QOpenGLFunctions_3_3_Core& gl);

private:
    GLint uniformLocation(QOpenGLFunctions_3_3_Core& gl, const char* name);
    bool checkShaderCompile(QOpenGLFunctions_3_3_Core& gl, GLuint shaderId) const;
    bool checkProgramLink(QOpenGLFunctions_3_3_Core& gl, GLuint programId) const;

    GLuint programId_ = 0;
    std::unordered_map<std::string, GLint> uniformLocationCache_;
};
