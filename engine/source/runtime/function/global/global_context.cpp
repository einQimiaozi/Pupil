#include "global_context.h"

namespace Pupil {
    RuntimeGlobalContext runtime_global_context; // 运行时上下文(全局变量)

    void RuntimeGlobalContext::startSystems(const std::string& config_file_path) {
        // 初始化log系统
        log_system = std::make_shared<Log>();
        LOG_INFO("logger initialized");
        // 初始化视窗系统
        window_system = std::make_shared<WindowSystem>();
        // todo: 从config中读取window create info
        WindowCreateInfo window_create_info;
        window_system->initialize(window_create_info);
        LOG_INFO("window system initialized");
        // 初始化渲染系统
        render_system = std::make_shared<RenderSystem>();
        RenderInterface r_interface = { window_system };
        render_system->initialize(r_interface);
        LOG_INFO("render system initialized");
        // todo: 初始化其他系统
    }
    void RuntimeGlobalContext::shutdownSystems() {
        rhi.reset();
        render_system.reset();
        window_system.reset();
        log_system.reset();
    }
}