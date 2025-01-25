#include "editor.h"
#include <iostream>

namespace Pupil {
    PupilEditor::PupilEditor() {}
    void PupilEditor::initialize(PupilEngine* engine_runtime) {
        assert(engine_runtime);                // ȷ��engine��ȷ����ʼ����
        this->engine_runtime = engine_runtime; // ��engine_runtime

        editor_ui = std::make_shared<EditorUI>();
        // ui����Ϣ��Ŀǰֻ����һ��window_system, window_system�����glfw�����д��ڲ�����������Ū
        WindowUIInitInfo ui_init_info = { runtime_global_context.window_system };

        // todo: ����һ��ui����
        editor_ui->initialize(ui_init_info);
    }
    void PupilEditor::run() {
        LOG_INFO("Hello Pupil!");
        assert(engine_runtime);
        assert(editor_ui);
        while (true) {
            if (runtime_global_context.window_system->shouldClose()) {
                return;
            }
            float delta_time = engine_runtime->calculateDeltaTime();
            engine_runtime->tickOneFrame(delta_time);
        }
    }
    void PupilEditor::shutdown() {}
}