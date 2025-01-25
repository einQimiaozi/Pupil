#pragma once

#include <memory>

#include "function/render/window_system.h"

namespace Pupil
{
    struct RHIInitInfo
    {
        std::shared_ptr<WindowSystem> window_system;
    };

    class RHI
    {
    public:
        virtual void initialize(RHIInitInfo init_info) = 0;

        virtual void createSwapchain() = 0;
        virtual void createSwapchainImageViews() = 0;
    };
};