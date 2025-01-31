#pragma once

#include "function/render/window_system.h"
#include <memory>

namespace Pupil {
    struct WindowUIInitInfo {
        std::shared_ptr<WindowSystem> window_system;
    };

    class WindowUI {
    public:
        virtual void initialize(WindowUIInitInfo init_info) = 0;
    };
}