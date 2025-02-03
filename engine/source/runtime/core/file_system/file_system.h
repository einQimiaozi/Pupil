#pragma once

#include <vector>
#include <filesystem>

namespace Pupil {
    class FileSystem {
    public:
        std::vector<std::filesystem::path> files(const std::filesystem::path &dirs);
    };
}