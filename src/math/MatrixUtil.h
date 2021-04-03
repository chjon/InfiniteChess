#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace glm {
    inline mat4 translate(vec3 translation) { return glm::translate(glm::mat4(1.0f), translation); }
    inline mat4 rotate(float angle, vec3 axis) { return glm::rotate(glm::mat4(1.0f), angle, axis); };
    inline mat4 rotateX(float a) { return glm::rotate(glm::mat4(1.0f), a, glm::vec3(1.0, 0.0, 0.0)); }
    inline mat4 rotateY(float a) { return glm::rotate(glm::mat4(1.0f), a, glm::vec3(0.0, 1.0, 0.0)); }
    inline mat4 rotateZ(float a) { return glm::rotate(glm::mat4(1.0f), a, glm::vec3(0.0, 0.0, 1.0)); }
}