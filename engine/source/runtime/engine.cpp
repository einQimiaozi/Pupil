#include "engine.h"
#include "runtime/tools/reflection/reflection_register.h"
#include "runtime/core/macro.h"
#include "function/global/global_context.h"
#include "function/render/render_system.h"
#include "function/render/window_system.h"

namespace Pupil {
    void PupilEngine::initialize(const std::string& config_file_path) {
        Reflection::TypeMetaRegister::metaRegister();
        runtime_global_context.startSystems(config_file_path);
    }

    void PupilEngine::shutdown() {
        runtime_global_context.shutdownSystems();
    }

    float PupilEngine::calculateDeltaTime() {
        using namespace std::chrono;
        steady_clock::time_point tick_time_point = steady_clock::now();
        duration<float> time_span = duration_cast<duration<float>>(tick_time_point - tick_time_point_last);
        tick_time_point_last = tick_time_point;
        return time_span.count();
    }

    void PupilEngine::rendererTick(float delta_time) {
        runtime_global_context.render_system->tick(delta_time);
        return;
    }

    bool PupilEngine::tickOneFrame(float delta_time) {
        bool tick_result = false;

        rendererTick(delta_time);

        tick_result = runtime_global_context.window_system->tickOneFrame();
        return !tick_result;
    }
}