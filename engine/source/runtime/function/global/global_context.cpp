#include "global_context.h"

#include "resource/type/data/mesh_data.h"
#include "function/render/window_system.h"
#include "function/render/render_system.h"
#include "resource/config_manager/config_manager.h"
#include "resource/asset_manager/asset_manager.h"
#include "platform/rhi/vulkan/vulkan_rhi.h"
#include "core/file_system/file_system.h"
#include "core/log/log.h"
#include "core/macro.h"

namespace Pupil {
    RuntimeGlobalContext runtime_global_context;

    void RuntimeGlobalContext::startSystems(const std::string& config_file_path) {
        log_system = std::make_shared<Log>();
        LOG_INFO("logger initialized");

        file_system = std::make_shared<FileSystem>();
        LOG_INFO("file system initialized")

        config_manager = std::make_shared<ConfigManager>();
        config_manager->initialize(config_file_path);
        LOG_INFO("config manager initialized");

        asset_manager = std::make_shared<AssetManager>();
        LOG_INFO("asset manager initialized");

        window_system = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        window_system->initialize(window_create_info);
        LOG_INFO("window system initialized");

        render_system = std::make_shared<RenderSystem>();
        RenderInterface r_interface = { window_system };
        render_system->initialize(r_interface);
        LOG_INFO("render system initialized");
    }
    void RuntimeGlobalContext::shutdownSystems() {
        rhi.reset();
        render_system.reset();
        window_system.reset();
        log_system.reset();
    }
}