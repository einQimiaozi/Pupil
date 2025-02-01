#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "core/reflection/reflection.h"

namespace Pupil {
    static const char* DEPLOYMENT_CONFIG_PATH = "deployment/conf.ini";
    static const char* DEVELOPMENT_CONFIG_PATH = "development/conf.ini";
    
    REFLECTION_TYPE(ConfigManager)
    CLASS(ConfigManager, WhiteListFields) {
        REFLECTION_BODY(ConfigManager)
    public:
        std::filesystem::path root_path;
        std::filesystem::path asset_path;
        std::filesystem::path schema_path;
        std::filesystem::path editor_big_icon_path;
        std::filesystem::path editor_small_icon_path;
        std::filesystem::path editor_font_path;

        META(Enable)
        std::string default_world_path;
        META(Enable)
        std::string global_rendering_res_path;
        META(Enable)
        std::string global_particle_res_path;

        void initialize(const std::filesystem::path& config_file_path);
    };
}