#pragma once
#include "../Common.h"

namespace ic {
    class Texture2D {
    public:
        Texture2D();
        void generateTexture(unsigned int width, unsigned int height, unsigned char* data);
        void bind() const;
    private:
        GLuint m_id;
        glm::vec2 m_dim;

        // texture format
        GLuint m_internalFormat; // format of texture object
        GLuint m_imageFormat;    // format of loaded image

        // texture wrapping/filtering configuration
        GLuint m_wrap_S;    // wrapping mode on S axis
        GLuint m_wrap_T;    // wrapping mode on T axis
        GLuint m_filterMin; // filtering mode if texture pixels < screen pixels
        GLuint m_filterMax; // filtering mode if texture pixels > screen pixels
    };
}