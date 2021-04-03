#include "Camera.h"
#define M_PI 3.141592653589793f

using namespace qb;

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float nearPlane, float farPlane, float fov) :
    m_pos(glm::vec4(pos.x, pos.y, pos.z, 1)),
    m_dir(glm::vec4(dir.x, dir.y, dir.z, 0)),
    m_up(glm::vec4(up.x, up.y, up.z, 1)),
    m_near(nearPlane),
    m_far(farPlane),
    m_yFOV(fov),
    m_stepSize(0.001f)
{}

inline float getXFOV(float aspectRatio, float yFOV) {
    return 2.0f * glm::atan(glm::tan(glm::radians(yFOV / 2.0f)) * aspectRatio);
}

const glm::mat4 Camera::getCamera() {
    float aspectRatio = static_cast<float>(m_screenDims.x) / static_cast<float>(m_screenDims.y);
    glm::mat4 view = glm::lookAt(glm::vec3(m_pos), glm::vec3(m_pos + m_dir), glm::vec3(m_up));
    glm::mat4 proj = glm::perspective(getXFOV(aspectRatio, m_yFOV), aspectRatio, m_near, m_far);
    return proj * view;
}

void Camera::update(const KeyEventHandler& keh) {
    if (keh.isDown('w')) m_pos.y += m_stepSize;
    if (keh.isDown('s')) m_pos.y -= m_stepSize;
    if (keh.isDown('d')) m_pos.x += m_stepSize;
    if (keh.isDown('a')) m_pos.x -= m_stepSize;
    if (keh.isDown('z')) m_pos.z += m_stepSize;
    if (keh.isDown('x')) m_pos.z -= m_stepSize;
}

void Camera::setDimensions(glm::vec2 screenDims) {
    m_screenDims   = screenDims;
    m_screenCentre = screenDims * 0.5f;
};

void Camera::setMouse(int x, int y) {
    // glm::vec2 delta(m_screenCentre.x - x, m_screenCentre.y - y);
    // if (delta.x) {
    //     m_dir = glm::normalize(glm::rotate(0.001f * delta.x, m_up) * m_dir);
    // }

    // if (delta.y) {
    //     m_dir = glm::normalize(glm::rotate(-0.001f * delta.y, glm::cross(glm::vec3(m_up), glm::vec3(m_dir))) * m_dir);
    // }

    // glutWarpPointer(m_screenCentre.x, m_screenCentre.y);
}