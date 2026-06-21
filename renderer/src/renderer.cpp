#include "renderer.h"

#include <GL/glext.h>
#include <glm/glm.hpp>

void Renderer::drawInstanced(QOpenGLFunctions_3_3_Core& gl, const DrawCommand& command) const
{
    gl.glBindVertexArray(command.vaoId);
    gl.glBindBuffer(GL_ARRAY_BUFFER, command.modelMatrixBufferId);

    for (size_t i = 0; i < 4; ++i) {
        const auto offset = reinterpret_cast<void*>(sizeof(float) * i * 4);
        gl.glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), offset);
        gl.glEnableVertexAttribArray(3 + i);
        gl.glVertexAttribDivisor(3 + i, 1);
    }

    gl.glDrawElementsInstanced(GL_TRIANGLES,
                               command.indexCount,
                               command.indexType,
                               command.indexOffset,
                               command.instanceCount);
}

void Renderer::drawNormalLineInstanced(QOpenGLFunctions_3_3_Core &gl, const DrawCommand &command) const {
    gl.glBindVertexArray(command.vaoId);
    gl.glBindBuffer(GL_ARRAY_BUFFER, command.modelMatrixBufferId);

    for (size_t i = 0; i < 4; ++i) {
        const auto offset = reinterpret_cast<void*>(sizeof(float) * i * 4);
        gl.glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), offset);
        gl.glEnableVertexAttribArray(3 + i);
        gl.glVertexAttribDivisor(3 + i, 1);
    }

    gl.glDrawElementsInstanced(GL_LINES,
        command.indexCount,
        command.indexType,
        command.indexOffset,
        command.instanceCount);
}
