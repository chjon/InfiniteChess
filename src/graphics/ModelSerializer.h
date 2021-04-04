#pragma once

#include "../Common.h"
#include "Model.h"

namespace ic {
    class ModelSerializer {
    public:
        static bool serialize(const std::string& filename, const Model& model);
        static Model deserialize(const std::string& filename);
    };
}