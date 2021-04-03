#pragma once
#include "../Common.h"
#include "Model.h"

namespace qb {
    class Cube {
    public:
        Cube(glm::ivec3 dims);
        ~Cube() = default;

        bool isSolved() const;

    private:
        struct Cubie {
            glm::fvec3 m_pos;
            glm::fvec3 m_angle; // Angle in degrees
        };

        int numCubies() const;
        bool isCorner(const Cubie& cubie) const;
        bool isEdge(const Cubie& cubie) const;
        bool isCentre(const Cubie& cubie) const;

        Model getModel(Cubie& cubie) const;
        std::vector<Model> getModel() const;

        glm::ivec3 m_dims{};
        std::vector<Cubie> m_cubies{};
    };
}