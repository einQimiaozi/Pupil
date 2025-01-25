#include "engine.h"

namespace Pupil {
    void PupilEngine::initialize(const std::string& config_file_path) {
        runtime_global_context.startSystems(config_file_path); // ��ʼ����ϵͳ
    }

    void PupilEngine::shutdown() {
        runtime_global_context.shutdownSystems();
    }

    float PupilEngine::calculateDeltaTime() {
        using namespace std::chrono;                                    // ������using
        steady_clock::time_point tick_time_point = steady_clock::now(); // ���ڼ���ʱ����ʱ������
        duration<float> time_span = duration_cast<duration<float>>(tick_time_point - tick_time_point_last); // ���ڼ���ʱ��������
        tick_time_point_last = tick_time_point;                                     // ����ʱ��
        return time_span.count();                                                   // ����ʱ���
    }

    bool PupilEngine::tickOneFrame(float delta_time) {
        bool tick_result = false;

        tick_result = runtime_global_context.window_system->tickOneFrame();

        return !tick_result;
    }
}