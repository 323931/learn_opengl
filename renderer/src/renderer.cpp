#include "renderer.h"

#include <glm/glm.hpp>
#include <iostream>

void Renderer::drawInstanced(QOpenGLFunctions_3_3_Core& gl, const Renderable& renderable) const
{
    if (!validateRenderable(gl, renderable)) {
        return;
    }

    const GpuMesh& mesh = *renderable.mesh;

    gl.glBindVertexArray(mesh.vaoId);
    gl.glBindBuffer(GL_ARRAY_BUFFER, renderable.modelMatrixBufferId);

    for (size_t i = 0; i < 4; ++i) {
        const auto offset = reinterpret_cast<void*>(sizeof(float) * i * 4);
        gl.glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), offset);
        gl.glEnableVertexAttribArray(4 + i);
        gl.glVertexAttribDivisor(4 + i, 1);
    }

    gl.glDrawElementsInstanced(mesh.primitiveMode,
                               mesh.indexCount,
                               mesh.indexType,
                               mesh.indexOffset,
                               renderable.instanceCount);
}

bool Renderer::validateRenderable(QOpenGLFunctions_3_3_Core& gl, const Renderable& renderable) const
{
    if (renderable.mesh == nullptr) {
        std::cerr << "Skip draw: renderable.mesh is null\n";
        return false;
    }

    const GpuMesh& mesh = *renderable.mesh;

    if (mesh.vaoId == 0 || !gl.glIsVertexArray(mesh.vaoId)) {
        std::cerr << "Skip draw: invalid VAO id " << mesh.vaoId << "\n";
        return false;
    }

    if (renderable.modelMatrixBufferId == 0 || !gl.glIsBuffer(renderable.modelMatrixBufferId)) {
        std::cerr << "Skip draw: invalid model matrix buffer id "
                  << renderable.modelMatrixBufferId << "\n";
        return false;
    }

    if (mesh.indexCount <= 0) {
        std::cerr << "Skip draw: indexCount is " << mesh.indexCount << "\n";
        return false;
    }

    if (renderable.instanceCount <= 0) {
        std::cerr << "Skip draw: instanceCount is " << renderable.instanceCount << "\n";
        return false;
    }

    return true;
}
