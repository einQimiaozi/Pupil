#pragma once

#include "function/ui/window_ui.h"

namespace Pupil {
    class EditorUI : public WindowUI {
    public:
        EditorUI();
        virtual void initialize(WindowUIInitInfo init_info) override final;
    };
}