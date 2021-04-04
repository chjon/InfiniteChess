#include "Model.h"

namespace ic {
    Model::Model(int numVertices, int numFaces) :
        m_numVertices(numVertices),
        m_numFaces(numFaces)
    {
        m_vertices = reinterpret_cast<Model::ModelVertex*>(malloc(numVertices * sizeof(ModelVertex)));
        m_faces = reinterpret_cast<Model::ModelFace*>(malloc(m_numFaces * sizeof(Model::ModelFace)));
    }

    Model::Model(Model&& other) {
        m_numVertices = other.m_numVertices;
        m_numFaces = other.m_numFaces;
        m_vertices = other.m_vertices;
        m_faces = other.m_faces;

        other.m_vertices = nullptr;
        other.m_faces = nullptr;
    }

    Model::~Model() {
        if (m_vertices != nullptr) {
            free(m_vertices);
            m_vertices = nullptr;
        }

        if (m_faces != nullptr) {
            free(m_faces);
            m_faces = nullptr;
        }
    }

    Model::operator=(Model&& other) {
        free(m_vertices);
        free(m_faces);

        m_numVertices = other.m_numVertices;
        m_numFaces = other.m_numFaces;
        m_vertices = other.m_vertices;
        m_faces = other.m_faces;

        other.m_vertices = nullptr;
        other.m_faces = nullptr;

        return 0;
    }
}