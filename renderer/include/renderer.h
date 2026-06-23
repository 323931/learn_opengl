#pragma once

#include <GL/gl.h>
#include <QOpenGLFunctions_3_3_Core>

struct GpuMesh {
    GLuint vaoId = 0;
    GLenum primitiveMode = GL_TRIANGLES;
    GLsizei indexCount = 0;
    GLenum indexType = GL_UNSIGNED_SHORT;
    const void* indexOffset = nullptr;
};

struct Renderable {
    const GpuMesh* mesh = nullptr;
    GLuint modelMatrixBufferId = 0;
    GLsizei instanceCount = 1;
};

class Renderer {
public:
    void drawInstanced(QOpenGLFunctions_3_3_Core& gl, const Renderable& renderable) const;

private:
    bool validateRenderable(QOpenGLFunctions_3_3_Core& gl, const Renderable& renderable) const;
};
