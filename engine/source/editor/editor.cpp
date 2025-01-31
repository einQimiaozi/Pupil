#include "editor.h"
#include <iostream>

namespace Pupil {
    PupilEditor::PupilEditor() {}
    void PupilEditor::initialize(PupilEngine* engine_runtime) {
        assert(engine_runtime);                
        this->engine_runtime = engine_runtime;

        editor_ui = std::make_shared<EditorUI>();
        WindowUIInitInfo ui_init_info = { runtime_global_context.window_system };

        editor_ui->initialize(ui_init_info);
    }
    void PupilEditor::run() {
        LOG_INFO("Everything is ready, but please don't forget Pupil");
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