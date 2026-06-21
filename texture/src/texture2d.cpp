#include "texture2d.h"

#include <QImage>
#include <iostream>

bool Texture2D::load(QOpenGLFunctions_3_3_Core& gl, const QString& path)
{
    destroy(gl);

    QImage image(path);
    if (image.isNull()) {
        std::cerr << "Failed to load texture image: " << path.toStdString() << std::endl;
        return false;
    }

    // OpenGL 这里按 RGBA/unsigned byte 上传；mirrored() 是为了匹配常见的 OpenGL UV 原点习惯。
    const QImage textureImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    // textureId_ 是 GPU texture object 的句柄；后续 GL_TEXTURE_2D 操作都会作用到当前绑定对象。
    gl.glGenTextures(1, &textureId_);
    gl.glBindTexture(GL_TEXTURE_2D, textureId_);

    // S/T 分别对应 UV 的 u/v。GL_REPEAT 允许 texCoord 超过 0..1，用来把地面纹理重复铺开。
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 缩小时使用 mipmap 之间的线性插值，减少远处纹理闪烁；放大时使用线性过滤让结果更平滑。
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 上传第 0 层 mipmap。internalFormat 是 GPU 内部格式，format/type 描述 QImage 像素数据格式。
    gl.glTexImage2D(GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    textureImage.width(),
                    textureImage.height(),
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    textureImage.constBits());
    // 根据原图自动生成 1/2、1/4、1/8... 的缩小版本，配合 GL_LINEAR_MIPMAP_LINEAR 使用。
    gl.glGenerateMipmap(GL_TEXTURE_2D);
    // 解绑避免后续误操作继续改到这张纹理；绘制时会在 bind() 里重新绑定。
    gl.glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture2D::bind(QOpenGLFunctions_3_3_Core& gl, GLuint textureUnit) const
{
    gl.glActiveTexture(GL_TEXTURE0 + textureUnit);
    gl.glBindTexture(GL_TEXTURE_2D, textureId_);
}

void Texture2D::destroy(QOpenGLFunctions_3_3_Core& gl)
{
    if (textureId_ != 0) {
        gl.glDeleteTextures(1, &textureId_);
        textureId_ = 0;
    }
}

GLuint Texture2D::id() const
{
    return textureId_;
}
