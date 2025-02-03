#pragma once

#include <memory>
#include <string>

namespace Pupil {
    class FileSystem;
    class ConfigManager;
    class AssetManager;
    class WindowSystem;
    class RenderSystem;
    class VulkanRHI;
    class Log;

    class RuntimeGlobalContext {
    public:
        // create all global systems and initialize these systems
        void startSystems(const std::string& config_file_path);
        // destroy all global systems
        void shutdownSystems();

        std::shared_ptr<FileSystem> file_system;
        std::shared_ptr<ConfigManager> config_manager;
        std::shared_ptr<AssetManager> asset_manager;
        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<RenderSystem> render_system;
        std::shared_ptr<VulkanRHI> rhi;
        std::shared_ptr<Log> log_system;
    };
    
    extern RuntimeGlobalContext runtime_global_context;
}