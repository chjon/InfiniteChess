#include "Common.h"
#include "graphics/Camera.h"
#include "graphics/GLLayer.h"
#include "graphics/ModelSerializer.h"
#include "keyboard/KeyEventHandler.h"

ic::KeyEventHandler keh;
GLuint program;
GLuint vboVertices, vboColours, iboFaces;
GLint attributeCoord3d;

ic::Model mdlTriangle;
ic::Camera camera(glm::vec2(0, 0), 0.f, 8.f, 64.f, 9.f, 0.01f);
int screenWidth = 1120, screenHeight = 630;

void render() {
    // Enable alpha
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Clear the screen
	glClearColor(0.25, 0.25, 0.25, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glValidateProgram(program);
	glUseProgram(program);

    // Bind vertex and index buffers
    using mv = ic::Model::ModelVertex;
    ic::GLLayer::bindAttributeAndVertices(vboVertices, attributeCoord3d, sizeof(mv::m_pos) / sizeof(GLfloat), sizeof(mv), offsetof(mv, m_pos));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboFaces);

    // Bind value to uniform variable
    camera.onRender();

    int size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	
	glDisableVertexAttribArray(attributeCoord3d);
    glDisableVertexAttribArray(iboFaces);

    // Double-buffered screen update
    glutSwapBuffers();
}

void exitHandler() {
    ic::Logger::log("Cleaning up and exiting");
    glDeleteProgram(program);
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColours);
    glDeleteBuffers(1, &iboFaces);
    ic::Logger::close();
}
void resizeHandler(int x, int y) {
    camera.setDimensions(glm::vec2(x, y));
    glViewport(0, 0, x, y);
}

void refreshFrame(int val) {
    camera.onRefresh(keh);
    glutTimerFunc(20, &refreshFrame, 0);
    glutPostRedisplay();
}

void onKeyUp  (unsigned char key, int x, int y) { keh.handle(key, x, y, false); }
void onKeyDown(unsigned char key, int x, int y) { keh.handle(key, x, y, true ); }
void onMouse  (int x, int y) {}

int main (int argc, char ** argv) {
    using namespace ic;
	std::string title = "Infinite Chess";

    GLLayer::ErrorCode errCode = GLLayer::ErrorCode::SUCCESS;
    Logger::init("output.log");

    // Initialize GL
    Logger::log("Initializing GL");
    errCode = GLLayer::initializeGL(argc, argv, screenWidth, screenHeight, title.c_str(), &render, &exitHandler);
    if (errCode != GLLayer::ErrorCode::SUCCESS) {
        Logger::log(GLLayer::getErrorString(errCode));
        return EXIT_FAILURE;
    }

    // Initialize keyboard
    KeyEventHandler::init(&onKeyUp, &onKeyDown);

    // Initialize program
    program = glCreateProgram();

    // Load models
    mdlTriangle = std::move(ModelSerializer::deserialize("mdl/triangle.mdl"));

    // Bind buffer objects
    GLLayer::bindBufferObject(vboVertices, GL_ARRAY_BUFFER,         mdlTriangle.verticesSize(), mdlTriangle.m_vertices, GL_STATIC_DRAW);
    GLLayer::bindBufferObject(vboColours,  GL_ARRAY_BUFFER,         mdlTriangle.verticesSize(), mdlTriangle.m_vertices, GL_STATIC_DRAW);
    GLLayer::bindBufferObject(iboFaces,    GL_ELEMENT_ARRAY_BUFFER, mdlTriangle.facesSize(),    mdlTriangle.m_faces,    GL_STATIC_DRAW);

    // Initialize shaders
    if (!GLLayer::compileAndLink(program, "src/graphics/shaders/triangle.v.glsl", GL_VERTEX_SHADER  )) return EXIT_FAILURE;
    if (!GLLayer::compileAndLink(program, "src/graphics/shaders/triangle.f.glsl", GL_FRAGMENT_SHADER)) return EXIT_FAILURE;

    // Link program
    if(!GLLayer::linkProgram(program)) return EXIT_FAILURE;

    // Bind variables
    if (!GLLayer::bindVariable(program, attributeCoord3d, "coord3d", false)) return EXIT_FAILURE;
    if (camera.init(program)) return EXIT_FAILURE;

    // Initialize keyboard event handler
    Logger::log("Initializing keyboard event handler");
    // glColor3f(1.0f, 0.0f, 0.0f);
    keh.registerListener('\x1B', [] (int x, int y) { glutLeaveMainLoop(); });

    // Initialize camera
    Logger::log("Initializing camera");
    glutReshapeFunc(&resizeHandler);
    glutPassiveMotionFunc(&onMouse);
    glutMotionFunc(&onMouse);

    glutTimerFunc(20, &refreshFrame, 0);

    // Event loop
    glutMainLoop();
    
    // Free resources
    exitHandler();

    return 0;
}