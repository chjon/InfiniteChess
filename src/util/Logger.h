#pragma once
#include <string>
#include "../Common.h"

namespace qb {
    class Logger {
    public:
        static void init(const std::string filename);
        static void log(const glm::mat4& mat);
        static void log(float val);
        static void log(const std::string& message);
        static void close();
    private:
        static FILE* m_logFile;
    };
}