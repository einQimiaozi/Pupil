#pragma once

#include <memory>

#include "function/render/window_system.h"

namespace Pupil {
    struct RenderInterface {
        std::shared_ptr<WindowSystem> window_system;
    };

    class WindowSystem;

    class RenderSystem {
    public:
        RenderSystem() = default;
        ~RenderSystem() = default;
        void initialize(RenderInterface interface);
    };
}