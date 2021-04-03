#include "Logger.h"

namespace qb {
    FILE* Logger::m_logFile = nullptr;

    void Logger::init(const std::string filename) {
        m_logFile = fopen(filename.c_str(), "w");
    }

    void Logger::log(const glm::mat4& mat) {
        if (m_logFile == nullptr) return;
        fprintf(m_logFile, "[");
        for (int i = 0; i < 4; i++) {
            fprintf(m_logFile, "[");
            for (int j = 0; j < 4; j++) {
                fprintf(m_logFile, "%f,", mat[i][j]);
            }
            fprintf(m_logFile, "],");
        }
        fprintf(m_logFile, "]\n");
    }

    void Logger::log(float val) {
        if (m_logFile == nullptr) return;
        fprintf(m_logFile, "%f\n", val);
    }

    void Logger::log(const std::string& message) {
        if (m_logFile == nullptr) return;
        fprintf(m_logFile, "%s\n", message.c_str());
    }

    void Logger::close() {
        if (m_logFile != nullptr) {
            fclose(m_logFile);
            m_logFile = nullptr;
        }
    }
}