#include "Shader.h"
#include "GLLayer.h"

using namespace ic;
void Shader::use() {
    glUseProgram(m_id);
}

bool Shader::compile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    m_id = glCreateProgram();
    GLint vShaderId, fShaderId;
    GLLayer::compile(vShaderId, m_id, vertexShaderFile  , GL_VERTEX_SHADER  );
    GLLayer::compile(fShaderId, m_id, fragmentShaderFile, GL_FRAGMENT_SHADER);
    GLLayer::linkProgram(m_id);
    glDeleteShader(vShaderId);
    glDeleteShader(fShaderId);
    return true;
}