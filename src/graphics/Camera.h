#pragma once
#include "../Common.h"
#include "../keyboard/KeyEventHandler.h"

namespace ic {
    class Camera {
    public:
        Camera(glm::vec2 pos, float ang, float minZoom, float maxZoom, float zoom, float stepSize);
        int init(GLint program);
        void setDimensions(glm::vec2 dim);
        void onRender();
        void onRefresh(const KeyEventHandler& keh);
    private:
        glm::vec2 m_pos;
        glm::vec2 m_dim;
        glm::vec2 m_screenCentre;
        float m_ang;
        float m_minZoom;
        float m_maxZoom;
        float m_zoom;
        float m_stepSize;

        // OpenGL handles
        GLint m_wts_handle;
    };
}