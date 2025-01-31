#pragma once

#include <memory>
#include <string>

#include "function/render/window_system.h"
#include "function/render/render_system.h"
#include "platform/rhi/vulkan/vulkan_rhi.h"
#include "core/log/log.h"
#include "core/macro.h"

namespace Pupil {
    class RuntimeGlobalContext {
    public:
        // create all global systems and initialize these systems
        void startSystems(const std::string& config_file_path);
        // destroy all global systems
        void shutdownSystems();

        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<RenderSystem> render_system;
        std::shared_ptr<VulkanRHI> rhi;
        std::shared_ptr<Log> log_system;
    };
    extern RuntimeGlobalContext runtime_global_context;
}