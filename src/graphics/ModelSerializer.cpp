#include "ModelSerializer.h"
#include "Model.h"

namespace qb {
    bool ModelSerializer::serialize(const std::string& filename, const Model& model) {
        if (filename.length() == 0) {
            return false;
        }

        FILE* modelFile = fopen(filename.c_str(), "w");
        if (modelFile == nullptr) {
            return false;
        }

        // Write vertices
        fprintf(modelFile, "v %d\n", model.m_numVertices);
        size_t sizeOfVertex = sizeof(Model::ModelVertex) / sizeof(GLfloat);
        for (int i = 0; i < model.m_numVertices; ++i) {
            for (unsigned int j = 0; j < sizeOfVertex; ++j) {
                fprintf(modelFile, "%f ", reinterpret_cast<GLfloat*>(&model.m_vertices[i])[j]);
            }

            fprintf(modelFile, "\n");
        }

        // Write faces
        fprintf(modelFile, "f %d\n", model.m_numFaces);
        size_t sizeOfFace = sizeof(Model::ModelFace) / sizeof(GLushort);
        for (int i = 0; i < model.m_numFaces; ++i) {
            for (unsigned int j = 0; j < sizeOfFace; ++j) {
                fprintf(modelFile, "%d ", reinterpret_cast<GLushort*>(&model.m_faces[i])[j]);
            }

            fprintf(modelFile, "\n");
        }

        fclose(modelFile);
        return true;
    }

    Model ModelSerializer::deserialize(const std::string& filename) {
        Logger::log("Deserializing model: " + filename);

        Model retVal{};
        if (filename.length() == 0) {
            Logger::log("Invalid file length");
            return Model{};
        }

        std::ifstream inReader;
        inReader.open(filename);
        if (!inReader.is_open()) {
            Logger::log("File could not be opened");
            return Model{};
        }

        // Read vertices
        char vertexMarker = 0;
        inReader >> vertexMarker >> retVal.m_numVertices;
        if (vertexMarker != 'v') {
            Logger::log("Missing token: expected vertex marker");
            inReader.close();
            return Model{};
        }

        size_t sizeOfVertex = sizeof(Model::ModelVertex) / sizeof(GLfloat);
        retVal.m_vertices = reinterpret_cast<Model::ModelVertex*>(malloc(retVal.m_numVertices * sizeof(Model::ModelVertex)));
        for (int i = 0; i < retVal.m_numVertices; ++i) {
            for (unsigned int j = 0; j < sizeOfVertex; ++j) {
                inReader >> reinterpret_cast<GLfloat*>(&retVal.m_vertices[i])[j];
            }
        }

        // Read faces
        char faceMarker = 0;
        inReader >> faceMarker >> retVal.m_numFaces;
        if (faceMarker != 'f') {
            Logger::log("Missing token: expected face marker");
            inReader.close();
            return Model{};
        }

        size_t sizeofFace = sizeof(Model::ModelFace) / sizeof(GLushort);
        retVal.m_faces = reinterpret_cast<Model::ModelFace*>(malloc(retVal.m_numFaces * sizeof(Model::ModelFace)));
        for (int i = 0; i < retVal.m_numFaces; ++i) {
            for (unsigned int j = 0; j < sizeofFace; ++j) {
                inReader >> reinterpret_cast<GLshort*>(&retVal.m_faces[i])[j];
            }
        }

        inReader.close();
        return retVal;
    }
}