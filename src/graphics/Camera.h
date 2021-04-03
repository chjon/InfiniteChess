#pragma once
#include "../Common.h"
#include "../keyboard/KeyEventHandler.h"

namespace qb {
    class Camera {
    public:
        Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float nearPlane, float farPlane, float fov);
        const glm::mat4 getCamera();
        void update(const KeyEventHandler& keh);
        void setDimensions(glm::vec2 screenDims);
        void setMouse(int x, int y);
    private:
        glm::vec4 m_pos;
        glm::vec4 m_dir;
        glm::vec4 m_up;
        glm::vec2 m_screenDims;
        glm::vec2 m_screenCentre;
        glm::vec2 m_mouseAngle;
        float m_near;
        float m_far;
        float m_yFOV;
        float m_stepSize;
    };
}