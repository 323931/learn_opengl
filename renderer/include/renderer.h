#pragma once

#include <GL/gl.h>
#include <QOpenGLFunctions_3_3_Core>

struct DrawCommand {
    GLuint vaoId = 0;
    GLuint modelMatrixBufferId = 0;
    GLsizei indexCount = 0;
    GLenum indexType = GL_UNSIGNED_SHORT;
    const void* indexOffset = nullptr;
    GLsizei instanceCount = 1;
};

class Renderer {
public:
    void drawInstanced(QOpenGLFunctions_3_3_Core& gl, const DrawCommand& command) const;
    void drawNormalLineInstanced(QOpenGLFunctions_3_3_Core& gl, const DrawCommand& command) const;
};
