#include "Camera.h"
#include "GLLayer.h"

using namespace ic;

Camera::Camera(glm::vec2 pos, float ang, float stepSize) :
    m_pos(pos),
    m_ang(ang),
    m_stepSize(stepSize)
{}

int Camera::init(GLint program) {
    if (!GLLayer::bindVariable(program, m_pos_handle, "m_pos", true)) return EXIT_FAILURE;
    if (!GLLayer::bindVariable(program, m_ang_handle, "m_ang", true)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void Camera::onRender() {
    glUniform2f(m_pos_handle, m_pos.x, m_pos.y);
    glUniform1f(m_ang_handle, m_ang);
}

void Camera::onRefresh(const KeyEventHandler& keh) {
    if (keh.isDown('w')) m_pos.y += m_stepSize;
    if (keh.isDown('s')) m_pos.y -= m_stepSize;
    if (keh.isDown('d')) m_pos.x += m_stepSize;
    if (keh.isDown('a')) m_pos.x -= m_stepSize;
    if (keh.isDown('q')) m_ang   += m_stepSize;
    if (keh.isDown('e')) m_ang   -= m_stepSize;
}