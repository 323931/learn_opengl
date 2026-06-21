#include "renderer.h"

#include <glm/glm.hpp>

void Renderer::drawInstanced(QOpenGLFunctions_3_3_Core& gl, const Renderable& renderable) const
{
    if (renderable.mesh == nullptr) {
        return;
    }

    const GpuMesh& mesh = *renderable.mesh;

    gl.glBindVertexArray(mesh.vaoId);
    gl.glBindBuffer(GL_ARRAY_BUFFER, renderable.modelMatrixBufferId);

    for (size_t i = 0; i < 4; ++i) {
        const auto offset = reinterpret_cast<void*>(sizeof(float) * i * 4);
        gl.glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), offset);
        gl.glEnableVertexAttribArray(3 + i);
        gl.glVertexAttribDivisor(3 + i, 1);
    }

    gl.glDrawElementsInstanced(mesh.primitiveMode,
                               mesh.indexCount,
                               mesh.indexType,
                               mesh.indexOffset,
                               renderable.instanceCount);
}
