#include "Camera.h"
#include "GLLayer.h"

using namespace ic;

Camera::Camera(glm::vec2 pos, float ang, float minZoom, float maxZoom, float zoom, float stepSize) :
    m_pos(pos),
    m_ang(ang),
    m_minZoom(minZoom),
    m_maxZoom(maxZoom),
    m_zoom(zoom),
    m_stepSize(stepSize)
{}

int Camera::init(GLint program) {
    if (!GLLayer::bindVariable(program, m_wts_handle, "m_wts", true)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void Camera::setDimensions(glm::vec2 dim) {
    m_dim = dim;
    m_screenCentre = 0.5f * dim;
}

void Camera::onRender() {
    const float scaleFactor = m_zoom / std::min(m_dim.x, m_dim.y);
    glm::mat3x3 wts = glm::mat3(1.0f)
        * glm::translate(glm::mat3(1.0f), m_pos + m_screenCentre)
        * glm::rotate   (glm::mat3(1.0f), m_ang)
        * glm::scale    (glm::mat3(1.0f), glm::vec2(scaleFactor, scaleFactor))
        * glm::translate(glm::mat3(1.0f), -m_screenCentre)
    ;
    glUniformMatrix3fv(m_wts_handle, 1, false, glm::value_ptr(wts));
}

void Camera::onRefresh(const KeyEventHandler& keh) {
    if (keh.isDown('w')) m_pos.y += m_stepSize * m_zoom;
    if (keh.isDown('s')) m_pos.y -= m_stepSize * m_zoom;
    if (keh.isDown('d')) m_pos.x += m_stepSize * m_zoom;
    if (keh.isDown('a')) m_pos.x -= m_stepSize * m_zoom;
    if (keh.isDown('q')) m_ang   += m_stepSize;
    if (keh.isDown('e')) m_ang   -= m_stepSize;
    if (keh.isDown('z')) m_zoom   = std::min(m_maxZoom, m_zoom * (1 + m_stepSize));
    if (keh.isDown('x')) m_zoom   = std::max(m_minZoom, m_zoom * (1 - m_stepSize));
}