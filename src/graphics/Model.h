#pragma once

#include "../Common.h"

namespace ic {
    class Model {
    public:
        struct ModelVertex {
            GLfloat m_pos[3];
            GLfloat m_col[3];
        };

        struct ModelFace {
            GLushort m_idx[3];
        };

        Model() = default;
        Model(int numVertices, int numFaces);
        Model(Model&) = delete;
        Model(Model&& other);
        ~Model();
        operator=(Model&& other);

        inline size_t verticesSize() { return m_numVertices * sizeof(ModelVertex); }
        inline size_t facesSize()    { return m_numFaces    * sizeof(ModelFace); }

        int m_numVertices = 0;
        int m_numFaces = 0;
        ModelVertex* m_vertices = nullptr;
        ModelFace* m_faces = nullptr;
    };
}