#pragma once

#include <cassert>
#include <memory>

#include "engine.h"
#include "editor_ui.h"
#include "function/global/global_context.h"
#include "core/macro.h"

namespace Pupil {
    class PupilEditor {
    public:
        PupilEditor();
        void initialize(PupilEngine* engine_runtime);
        void run();
        void shutdown();

    protected:
        std::shared_ptr<EditorUI> editor_ui;
        PupilEngine* engine_runtime = nullptr;
    };
}
