#include "engine.h"

namespace Pupil {
    void PupilEngine::initialize(const std::string& config_file_path) {
        runtime_global_context.startSystems(config_file_path); // 初始化子系统
    }

    void PupilEngine::shutdown() {
        runtime_global_context.shutdownSystems();
    }

    float PupilEngine::calculateDeltaTime() {
        using namespace std::chrono;                                    // 作用域using
        steady_clock::time_point tick_time_point = steady_clock::now(); // 用于计量时间点的时钟类型
        duration<float> time_span = duration_cast<duration<float>>(tick_time_point - tick_time_point_last); // 用于计量时间间隔类型
        tick_time_point_last = tick_time_point;                                     // 更新时间
        return time_span.count();                                                   // 返回时间差
    }

    bool PupilEngine::tickOneFrame(float delta_time) {
        bool tick_result = false;

        tick_result = runtime_global_context.window_system->tickOneFrame();

        return !tick_result;
    }
}