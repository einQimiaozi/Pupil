#include "editor.h"
#include <iostream>

namespace Pupil {
    PupilEditor::PupilEditor() {}
    void PupilEditor::initialize(PupilEngine* engine_runtime) {
        assert(engine_runtime);                // 确保engine正确被初始化了
        this->engine_runtime = engine_runtime; // 绑定engine_runtime

        editor_ui = std::make_shared<EditorUI>();
        // ui的信息里目前只带了一个window_system, window_system里放了glfw，所有窗口操作都从这里弄
        WindowUIInitInfo ui_init_info = { runtime_global_context.window_system };

        // todo: 补充一下ui界面
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