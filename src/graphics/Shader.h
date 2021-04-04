#pragma once
#include "../Common.h"

namespace ic {
    class Shader {
    public:
        void use();
        bool compile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
        GLint getId() { return m_id; }

        void setFloat    (const char *name, float value                       );
        void setInteger  (const char *name, int value                         );
        void setVector2f (const char *name, float x, float y                  );
        void setVector2f (const char *name, const glm::vec2 &value            );
        void setVector3f (const char *name, float x, float y, float z         );
        void setVector3f (const char *name, const glm::vec3 &value            );
        void setVector4f (const char *name, float x, float y, float z, float w);
        void setVector4f (const char *name, const glm::vec4 &value            );
        void setMatrix3  (const char *name, const glm::mat3 &matrix           );
        void setMatrix4  (const char *name, const glm::mat4 &matrix           );
    private:
        GLuint m_id;
    };
}