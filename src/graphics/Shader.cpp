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

void Shader::setFloat   (const char *name, float val                         ) { glUniform1f       (glGetUniformLocation(m_id, name), val                             ); }
void Shader::setInteger (const char *name, int val                           ) { glUniform1i       (glGetUniformLocation(m_id, name), val                             ); }
void Shader::setVector2f(const char *name, float x, float y                  ) { glUniform2f       (glGetUniformLocation(m_id, name), x, y                            ); }
void Shader::setVector2f(const char *name, const glm::vec2 &val              ) { glUniform2f       (glGetUniformLocation(m_id, name), val.x, val.y                    ); }
void Shader::setVector3f(const char *name, float x, float y, float z         ) { glUniform3f       (glGetUniformLocation(m_id, name), x, y, z                         ); }
void Shader::setVector3f(const char *name, const glm::vec3 &val              ) { glUniform3f       (glGetUniformLocation(m_id, name), val.x, val.y, val.z             ); }
void Shader::setVector4f(const char *name, float x, float y, float z, float w) { glUniform4f       (glGetUniformLocation(m_id, name), x, y, z, w                      ); }
void Shader::setVector4f(const char *name, const glm::vec4 &val              ) { glUniform4f       (glGetUniformLocation(m_id, name), val.x, val.y, val.z, val.w      ); }
void Shader::setMatrix3 (const char *name, const glm::mat3 &matrix           ) { glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, false, glm::value_ptr(matrix)); }
void Shader::setMatrix4 (const char *name, const glm::mat4 &matrix           ) { glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, false, glm::value_ptr(matrix)); }