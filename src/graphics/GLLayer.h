#pragma once

#include "../Common.h"
#include "Model.h"

namespace ic {
    class GLLayer {
    public:
        enum class ErrorCode {
            SUCCESS,
            ERROR_GLEW,
            ERROR_GLEW_VERSION_2_0,
            ERROR_READING_FILE,
            ERROR_COMPILE,
            ERROR_LINK,
            ERROR_BIND_VARIABLE,
        };

        static ErrorCode initializeGL(
            int& argc,
            char** argv,
            int width,
            int height,
            const char* name,
            void(*displayFunc)(void),
			void(*exitFunc)(void)
        ); 

        template <typename T>
        static void bindBufferObject(GLuint& id, GLint bufferType, size_t arraySize, T* vertices, GLint drawType);
        static void bindBufferObject(GLuint& id, GLint bufferType, size_t arraySize, Model::ModelVertex* vertices, GLint drawType);
        static void bindBufferObject(GLuint& id, GLint bufferType, size_t arraySize, Model::ModelFace* vertices, GLint drawType);

        static bool compile(GLint& shaderId, GLint program, const std::string& filename, GLenum shaderType);
        static bool linkProgram(GLint program);
        static bool bindVariable(GLint program, GLint& id, const std::string& name, const bool isUniform);

        static void bindAttributeAndVertices(GLuint vbo, GLint id, size_t elementsPerVertex, size_t stride, size_t offset);

        static std::string getErrorString(ErrorCode errorCode);

    private:
        static bool checkCompileError(GLint program, const std::string& type);
        static std::string readFile(const std::string& filename);
        
        static const std::string OPENGL_SHADER_VERSION_HEADER;
        static const std::string OPENGL_SHADER_PRECISION_HEADER;
    };
}