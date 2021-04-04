#pragma once
#include "../Common.h"

namespace ic {
    class Shader {
    public:
        void use();
        bool compile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
        GLint getId() { return m_id; }
    private:
        GLuint m_id;
    };
}