#pragma once

#include <memory>

namespace Pupil {
    class WindowSystem;
    class VulkanRHI;

    struct RenderInterface {
        std::shared_ptr<WindowSystem> window_system;
    };

    class RenderSystem {
    public:
        RenderSystem() = default;
        ~RenderSystem() = default;
        void initialize(RenderInterface interface);
    private:
        std::shared_ptr<VulkanRHI> rhi;
    };
}