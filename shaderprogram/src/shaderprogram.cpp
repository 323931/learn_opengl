#include "shaderprogram.h"

#include <iostream>
#include <vector>

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

bool ShaderProgram::load(QOpenGLFunctions_3_3_Core& gl,
                         const QByteArray& vertexShaderSource,
                         const QByteArray& fragmentShaderSource)
{
    destroy(gl);

    if (vertexShaderSource.isEmpty() || fragmentShaderSource.isEmpty()) {
        std::cerr << "Failed to load shader program: shader source is empty" << std::endl;
        return false;
    }

    const GLuint vertexShaderId = gl.glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentShaderId = gl.glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSource = vertexShaderSource.constData();
    const char* fragmentSource = fragmentShaderSource.constData();
    gl.glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
    gl.glShaderSource(fragmentShaderId, 1, &fragmentSource, nullptr);

    gl.glCompileShader(vertexShaderId);
    gl.glCompileShader(fragmentShaderId);

    if (!checkShaderCompile(gl, vertexShaderId) || !checkShaderCompile(gl, fragmentShaderId)) {
        gl.glDeleteShader(vertexShaderId);
        gl.glDeleteShader(fragmentShaderId);
        return false;
    }

    const GLuint newProgramId = gl.glCreateProgram();
    gl.glAttachShader(newProgramId, vertexShaderId);
    gl.glAttachShader(newProgramId, fragmentShaderId);
    gl.glLinkProgram(newProgramId);

    gl.glDeleteShader(vertexShaderId);
    gl.glDeleteShader(fragmentShaderId);

    if (!checkProgramLink(gl, newProgramId)) {
        gl.glDeleteProgram(newProgramId);
        return false;
    }

    programId_ = newProgramId;
    uniformLocationCache_.clear();
    return true;
}

void ShaderProgram::destroy(QOpenGLFunctions_3_3_Core& gl)
{
    if (programId_ != 0) {
        gl.glDeleteProgram(programId_);
        programId_ = 0;
        uniformLocationCache_.clear();
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

bool ShaderProgram::checkShaderCompile(QOpenGLFunctions_3_3_Core& gl, GLuint shaderId) const
{
    GLint status = GL_FALSE;
    gl.glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) {
        return true;
    }

    GLint logLength = 0;
    gl.glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> log(logLength);
        gl.glGetShaderInfoLog(shaderId, logLength, nullptr, log.data());
        std::cerr << "Shader compile failed: " << log.data() << std::endl;
    } else {
        std::cerr << "Shader compile failed with no info log" << std::endl;
    }

    return false;
}

bool ShaderProgram::checkProgramLink(QOpenGLFunctions_3_3_Core& gl, GLuint programId) const
{
    GLint status = GL_FALSE;
    gl.glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == GL_TRUE) {
        return true;
    }

    GLint logLength = 0;
    gl.glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> log(logLength);
        gl.glGetProgramInfoLog(programId, logLength, nullptr, log.data());
        std::cerr << "Shader program link failed: " << log.data() << std::endl;
    } else {
        std::cerr << "Shader program link failed with no info log" << std::endl;
    }

    return false;
}
