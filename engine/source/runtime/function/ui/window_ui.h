#pragma once

#include <memory>

#include "function/render/window_system.h"
#include "function/render/render_system.h"

namespace Pupil {
    struct WindowUIInitInfo {
        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<RenderSystem> render_system;
    };

    class WindowUI {
    public:
        virtual void initialize(WindowUIInitInfo init_info) = 0;
    };
}