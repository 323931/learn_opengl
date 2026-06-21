#pragma once

#include <GL/gl.h>
#include <QString>
#include <QOpenGLFunctions_3_3_Core>

class Texture2D {
public:
    bool load(QOpenGLFunctions_3_3_Core& gl, const QString& path);
    void bind(QOpenGLFunctions_3_3_Core& gl, GLuint textureUnit) const;
    void destroy(QOpenGLFunctions_3_3_Core& gl);

    GLuint id() const;

private:
    GLuint textureId_ = 0;
};
