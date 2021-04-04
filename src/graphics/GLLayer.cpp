#include "GLLayer.h"

namespace ic {
#ifdef GL_ES_VERSION_2_0
    const std::string GLLayer::OPENGL_SHADER_VERSION_HEADER = "#version 100\n"; // OpenGL ES 2.0
#else
    const std::string GLLayer::OPENGL_SHADER_VERSION_HEADER = "#version 120\n"; // OpenGL 2.1
#endif
    const std::string GLLayer::OPENGL_SHADER_PRECISION_HEADER = readFile("src/graphics/shaders/precisionHeader.glsl") + "\n";

    GLLayer::ErrorCode GLLayer::initializeGL(
        int& argc,
        char** argv,
        int width,
        int height,
        const char* name,
        void(*displayFunc)(void),
		void(*exitFunc)(void)
    ) {
        // Initialize GLUT
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
        glutInitWindowSize(width, height);
        glutInitWindowPosition(-1, -1);
        glutCreateWindow(name);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        glutDisplayFunc(displayFunc);
		glutCloseFunc(exitFunc);

        // Initialize GLEW
        GLenum glew_status = glewInit();
        if (glew_status != GLEW_OK) {
            std::cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << std::endl;
            return ErrorCode::ERROR_GLEW;
        }

        // Enforce a minimum OpenGL version of 2.0
        if (!GLEW_VERSION_2_0) {
            return ErrorCode::ERROR_GLEW_VERSION_2_0;
        }

        return ErrorCode::SUCCESS;
    }

    template <typename T>
    void GLLayer::bindBufferObject(GLuint& id, GLint bufferType, size_t arraySize, T* vertices, GLint drawType) {
        glGenBuffers(1, &id);
        glBindBuffer(bufferType, id);
        glBufferData(bufferType, arraySize, vertices, drawType);
    }

    void GLLayer::bindBufferObject(GLuint& id, GLint bufferType, size_t arraySize, Model::ModelVertex* vertices, GLint drawType) {
        bindBufferObject<GLfloat>(id, bufferType, arraySize, reinterpret_cast<GLfloat*>(vertices), drawType);
    }

    void GLLayer::bindBufferObject(GLuint& id, GLint bufferType, size_t arraySize, Model::ModelFace* vertices, GLint drawType) {
        bindBufferObject<GLushort>(id, bufferType, arraySize, reinterpret_cast<GLushort*>(vertices), drawType);
    }

    bool GLLayer::compileAndLink(GLint program, const std::string& filename, GLenum shaderType) {
        std::string source = readFile(filename);
        if (source.length() == 0) {
            Logger::log("Failed to read shader source: " + filename);
            return false;
        }

        // Create shader
        const GLchar* sources[] = {
            OPENGL_SHADER_VERSION_HEADER.c_str(),
            OPENGL_SHADER_PRECISION_HEADER.c_str(),
            source.c_str(),
        };

        GLuint shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 3, sources, NULL);
        glCompileShader(shaderId);
        GLint successfullyCompiled = GL_FALSE;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &successfullyCompiled);
        if (successfullyCompiled == GL_FALSE) {
			GLchar errorInfo[1024];
			glGetShaderInfoLog(shaderId, sizeof(errorInfo), NULL, errorInfo);
            Logger::log("Failed to compile source \'" + filename + "\': ");
			Logger::log(errorInfo);
            glDeleteShader(shaderId);
            return false;
        }

        // Link shader to program
        glAttachShader(program, shaderId);

        Logger::log("Successfully compiled and linked source: " + filename);
        return true;
    }

    bool GLLayer::linkProgram(GLint program) {
        GLint link_ok = GL_FALSE;
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
        if (link_ok == GL_FALSE) {
            Logger::log("Failed to link program");
            return false;
        }

        Logger::log("Successfully linked program");
        return true;
    }

    bool GLLayer::bindVariable(GLint program, GLint& id, const std::string& name, const bool isUniform) {
        if (isUniform) {
            id = glGetUniformLocation(program, name.c_str());
        } else {
            id = glGetAttribLocation(program, name.c_str());
        }

        if (id == -1) {
            Logger::log("Could not bind variable " + name);
            return false;
        }

        Logger::log("Successfully bound variable " + name);
        return true;
    }

    void GLLayer::bindAttributeAndVertices(GLuint vbo, GLint id, size_t elementsPerVertex, size_t stride, size_t offset) {
        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(id);

        // Describe VBO format to OpenGL
        glVertexAttribPointer(
            id,                // attribute id
            elementsPerVertex, // number of elements per vertex, here (x,y)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            stride,            // no extra data between each position
            (GLvoid*) offset   // offset of first element
        );
    }

    // Return an error message for an error code
    std::string GLLayer::getErrorString(ErrorCode errorCode) {
        switch (errorCode) {
            case ErrorCode::SUCCESS:                return "No error";
            case ErrorCode::ERROR_GLEW:             return "Failed to initialize GLEW";
            case ErrorCode::ERROR_GLEW_VERSION_2_0: return "Graphics card does not support OpenGL 2.0";
            case ErrorCode::ERROR_READING_FILE:     return "Error reading file";
            case ErrorCode::ERROR_COMPILE:          return "Failed to compile shader";
            case ErrorCode::ERROR_LINK:             return "Failed to link program and shaders";
            case ErrorCode::ERROR_BIND_VARIABLE:    return "Failed to bind variable";
            default:                                return "Unknown error";
        }
    }

    // Read entire file as string
    std::string GLLayer::readFile(const std::string& filename) {
        if (filename.length() == 0) {
            return "";
        }

        FILE* sourceFile = fopen(filename.c_str(), "r");
        if (sourceFile == nullptr) {
            return "";
        }

        const int BUFFER_SIZE = 256;
        size_t bytesRead = 0;
        std::string source;
        char buffer[BUFFER_SIZE + 1];
        buffer[BUFFER_SIZE] = '\0';
        while (BUFFER_SIZE == (bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, sourceFile))) {
            source += buffer;
        }

        if (bytesRead > 0) {
            memset(buffer + bytesRead, '\0', BUFFER_SIZE - bytesRead);
            source += buffer;
        }
        fclose(sourceFile);

        return source;
    }
}