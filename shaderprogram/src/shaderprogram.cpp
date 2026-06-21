#include "shaderprogram.h"

void ShaderProgram::setProgramId(GLuint programId)
{
    programId_ = programId;
    uniformLocationCache_.clear();
}

GLuint ShaderProgram::id() const
{
    return programId_;
}

void ShaderProgram::use(QOpenGLFunctions_3_3_Core& gl) const
{
    gl.glUseProgram(programId_);
}

void ShaderProgram::setMat4(QOpenGLFunctions_3_3_Core& gl, const char* name, const glm::mat4& value)
{
    const GLint location = uniformLocation(gl, name);
    if (location >= 0) {
        gl.glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

void ShaderProgram::setVec3(QOpenGLFunctions_3_3_Core& gl, const char* name, const glm::vec3& value)
{
    const GLint location = uniformLocation(gl, name);
    if (location >= 0) {
        gl.glUniform3fv(location, 1, &value[0]);
    }
}

void ShaderProgram::setFloat(QOpenGLFunctions_3_3_Core& gl, const char* name, float value)
{
    const GLint location = uniformLocation(gl, name);
    if (location >= 0) {
        gl.glUniform1f(location, value);
    }
}

GLint ShaderProgram::uniformLocation(QOpenGLFunctions_3_3_Core& gl, const char* name)
{
    const std::string uniformName(name);
    const auto it = uniformLocationCache_.find(uniformName);
    if (it != uniformLocationCache_.end()) {
        return it->second;
    }

    const GLint location = gl.glGetUniformLocation(programId_, name);
    uniformLocationCache_.insert({uniformName, location});
    return location;
}
