#pragma once

#include <memory>
#include <optional>
#include <assert.h>

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_scene.h"

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
        std::shared_ptr<RenderCamera> render_camera;
        std::shared_ptr<RenderScene> render_scene;
    };
}