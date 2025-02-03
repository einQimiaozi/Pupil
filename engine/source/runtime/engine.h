#pragma once

#include <cassert>
#include <chrono>
#include <memory>
#include <string>

#include "engine.h"
#include "function/render/window_system.h"
#include "function/global/global_context.h"
#include "core/log/log.h"

namespace Pupil {
    class PupilEngine {
    public:
        void initialize(const std::string& config_file_path);
        void shutdown();

        float calculateDeltaTime();
        bool tickOneFrame(float delta_time);
    private:
        void rendererTick(float delta_time);

    protected:
        std::chrono::steady_clock::time_point tick_time_point_last = std::chrono::steady_clock::now();
    };
}