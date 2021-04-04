#include "Texture2D.h"

using namespace ic;

Texture2D::Texture2D() :
    m_dim(0, 0),
    m_internalFormat(GL_RGB),
    m_imageFormat(GL_RGB),
    m_wrap_S(GL_REPEAT),
    m_wrap_T(GL_REPEAT),
    m_filterMin(GL_NEAREST),
    m_filterMax(GL_LINEAR)
{
    glGenTextures(1, &m_id);
}

void Texture2D::generateTexture(unsigned int width, unsigned int height, unsigned char* data) {
    m_dim = glm::vec2(width, height);

    // create texture
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_imageFormat, GL_UNSIGNED_BYTE, data);

    // set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_id);
}