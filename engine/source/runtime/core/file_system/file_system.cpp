#include "core/file_system/file_system.h"

namespace Pupil {
    std::vector<std::filesystem::path> FileSystem::files(const std::filesystem::path &dirs) {
        std::vector<std::filesystem::path> files;
        for (auto const& dir : std::filesystem::recursive_directory_iterator {dirs}) {
            if (dir.is_regular_file()) {
                files.push_back(dir);
            }
        }
        return files;
    }
}