#include "Cube.h"

namespace qb {
    static void forXYZ(glm::ivec3 dims, std::function<void(glm::ivec3)> callback) {
        glm::ivec3 iter{};
        for (iter.x = 0; iter.x < dims.x; ++iter.x) {
            for (iter.y = 0; iter.y < dims.y; ++iter.y) {
                for (iter.z = 0; iter.z < dims.z; ++iter.z) {
                    callback(iter);
                }
            }
        }
    }

    Cube::Cube(glm::ivec3 dims) :
        m_dims(dims)
    {
        forXYZ(dims, [&](glm::ivec3 iter) {
            if (
                (iter.x == 0 || iter.x == dims.x - 1) ||
                (iter.y == 0 || iter.x == dims.y - 1) ||
                (iter.z == 0 || iter.x == dims.z - 1)
            ) {
                m_cubies.push_back(Cubie{
                    glm::fvec3{ iter.x, iter.y, iter.z },
                    glm::fvec3{ 0, 0, 0 },
                });
            }
        });
    }

    bool Cube::isSolved() const {
        const glm::fvec3& orientation = m_cubies[0].m_angle;
        for (const Cubie& cubie : m_cubies) {
            if (isCentre(cubie)) {
                if (cubie.m_pos.x == 0 || cubie.m_pos.x == m_dims.x - 1) {
                    if (cubie.m_angle.x == orientation.x) return false;
                } else if (cubie.m_pos.y == 0 || cubie.m_pos.y == m_dims.y - 1) {
                    if (cubie.m_angle.y == orientation.y) return false;
                } else if (cubie.m_pos.z == 0 || cubie.m_pos.z == m_dims.z - 1) {
                    if (cubie.m_angle.z == orientation.z) return false;
                } else {
                    abort(); // This should never happen
                }
            } else if (cubie.m_angle != orientation) return false;
        }

        return true;
    }

    bool Cube::isCorner(const Cubie& cubie) const {
        return (
            (cubie.m_pos.x == 0 || cubie.m_pos.x == m_dims.x - 1) &&
            (cubie.m_pos.y == 0 || cubie.m_pos.y == m_dims.y - 1) &&
            (cubie.m_pos.z == 0 || cubie.m_pos.z == m_dims.z - 1)
        );
    }

    bool Cube::isEdge(const Cubie& cubie) const {
        return !(
            (cubie.m_pos.x == 0 || cubie.m_pos.x == m_dims.x - 1) ^
            (cubie.m_pos.y == 0 || cubie.m_pos.y == m_dims.y - 1) ^
            (cubie.m_pos.z == 0 || cubie.m_pos.z == m_dims.z - 1)
        );
    }

    bool Cube::isCentre(const Cubie& cubie) const {
        return !isCorner(cubie) && !isEdge(cubie);
    }

    int Cube::numCubies() const {
        return
            (m_dims.x - 0) * (m_dims.y - 0) * (m_dims.z - 0) -
            (m_dims.x - 2) * (m_dims.y - 2) * (m_dims.z - 2);
    }

    template<typename T>
    static void setVec3(T* list, const int v1, const int v2, const int v3) {
        list[0] = static_cast<T>(v1);
        list[1] = static_cast<T>(v2);
        list[2] = static_cast<T>(v3);
    }

    template<typename T>
    static void setVec3(T* list, const float v1, const float v2, const float v3) {
        list[0] = static_cast<T>(v1);
        list[1] = static_cast<T>(v2);
        list[2] = static_cast<T>(v3);
    }

    Model Cube::getModel(Cubie& cubie) const {
        const int NUM_VERTICES = 8;     // A cube has 8 vertices
        const int NUM_FACES = 6 * 2;    // A cube has 6 faces (each divided into 2 triangles)
        Model model(NUM_VERTICES, NUM_FACES);

        for (int z = 0; z <= 1; ++z) {
            for (int y = 0; y <= 1; ++y) {
                for (int x = 0; x <= 1; ++x) {
                    const int i = z << 2 | y << 1 | x << 0;
                    setVec3<GLfloat>(model.m_vertices[i].m_pos, x + cubie.m_pos.x, cubie.m_pos.y + y, cubie.m_pos.z + z);
                    setVec3<GLfloat>(model.m_vertices[i].m_col, x, y, z);
                }
            }
        }

        int i = -1;
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b000, 0b010, 0b011); // D
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b011, 0b001, 0b000);
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b111, 0b110, 0b100); // U
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b100, 0b101, 0b111);
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b000, 0b001, 0b101); // F
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b101, 0b100, 0b000);
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b111, 0b011, 0b010); // B
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b010, 0b110, 0b111);
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b000, 0b100, 0b110); // L
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b110, 0b010, 0b000);
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b111, 0b101, 0b001); // R
        setVec3<GLushort>(model.m_faces[++i].m_idx, 0b001, 0b011, 0b111);

        return model;
    }

    std::vector<Model> Cube::getModel() const {
        std::vector<Model> models;
        for (Cubie cubie : m_cubies) {
            models.push_back(getModel(cubie));
        }

        return models;
    }
}