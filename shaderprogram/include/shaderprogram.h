#pragma once

#include <GL/gl.h>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class ShaderProgram {
public:
    void setProgramId(GLuint programId);
    GLuint id() const;

    void use(QOpenGLFunctions_3_3_Core& gl) const;
    void setMat4(QOpenGLFunctions_3_3_Core& gl, const char* name, const glm::mat4& value);
    void setVec3(QOpenGLFunctions_3_3_Core& gl, const char* name, const glm::vec3& value);
    void setFloat(QOpenGLFunctions_3_3_Core& gl, const char* name, float value);

private:
    GLint uniformLocation(QOpenGLFunctions_3_3_Core& gl, const char* name);

    GLuint programId_ = 0;
    std::unordered_map<std::string, GLint> uniformLocationCache_;
};
