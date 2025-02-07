#pragma once

#include <memory>
#include <optional>
#include <assert.h>

#include "runtime/function/render/render_swap_context.h"

namespace Pupil {
    class WindowSystem;
    class VulkanRHI;
    class RenderResourceBase;

    struct RenderInterface {
        std::shared_ptr<WindowSystem> window_system;
    };

    class RenderSystem {
    public:
        RenderSystem() = default;
        ~RenderSystem() = default;
        void initialize(RenderInterface interface);
        void tick(float delta_time);
    private:
        std::shared_ptr<VulkanRHI> rhi;
        std::shared_ptr<RenderResourceBase> render_resource;
    };
}