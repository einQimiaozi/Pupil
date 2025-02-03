#include "resource/config_manager/config_manager.h"

namespace Pupil {
    void ConfigManager::initialize(const std::filesystem::path& config_file_path) {
        std::ifstream config_file(config_file_path);
        std::string   config_line;
        while (std::getline(config_file, config_line)) {
            size_t seperate_pos = config_line.find_first_of('=');
            if (seperate_pos > 0 && seperate_pos < (config_line.length() - 1)) {
                std::string name  = config_line.substr(0, seperate_pos);
                std::string value = config_line.substr(seperate_pos + 1, config_line.length() - seperate_pos - 1);
                if (name == "BinaryRootFolder") {
                    root_path = config_file_path.parent_path() / value;
                }else if (name == "AssetFolder") {
                    asset_path = root_path / value;
                }else if (name == "SchemaFolder") {
                    schema_path = root_path / value;
                }else if (name == "DefaultWorld") {
                    default_world_path = value;
                }else if (name == "BigIconFile") {
                    editor_big_icon_path = root_path / value;
                }else if (name == "SmallIconFile") {
                    editor_small_icon_path = root_path / value;
                }else if (name == "FontFile") {
                    editor_font_path = root_path / value;
                }else if (name == "GlobalRenderingRes") {
                    global_rendering_res_path = value;   
                }else if (name == "GlobalParticleRes") {
                    global_particle_res_path = value;
                }
            }
        }
        std::cout << "BinaryRootFolder: " << root_path << "\n";
        std::cout << "AssetFolder: " << asset_path << "\n";
        std::cout << "SchemaFolder: " << schema_path << "\n";
        std::cout << "DefaultWorld: " << default_world_path << "\n";
        std::cout << "BigIconFile: " << editor_big_icon_path << "\n";
        std::cout << "SmallIconFile: " << editor_small_icon_path << "\n";
        std::cout << "FontFile: " << editor_font_path << "\n";
        std::cout << "GlobalRenderingRes: " << global_rendering_res_path << "\n";
        std::cout << "GlobalParticleRes: " << global_particle_res_path << "\n";
    }
}