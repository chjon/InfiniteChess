#pragma once
#include "../Common.h"
#include "../keyboard/KeyEventHandler.h"

namespace ic {
    class Camera {
    public:
        Camera(glm::vec2 pos, float ang, float stepSize);
        int init(GLint program);
        void setDimensions(glm::vec2 dim) { m_dim = dim; }
        void onRender();
        void onRefresh(const KeyEventHandler& keh);
    private:
        glm::vec2 m_pos;
        glm::vec2 m_dim;
        glm::vec2 m_screenCentre;
        float m_ang;
        float m_stepSize;

        // OpenGL handles
        GLint m_pos_handle;
        GLint m_ang_handle;
        GLint m_dim_handle;
    };
}