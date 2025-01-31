#include "global_context.h"

namespace Pupil {
    RuntimeGlobalContext runtime_global_context;

    void RuntimeGlobalContext::startSystems(const std::string& config_file_path) {
        log_system = std::make_shared<Log>();
        LOG_INFO("logger initialized");
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