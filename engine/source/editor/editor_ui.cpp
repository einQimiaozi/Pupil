#include "editor_ui.h"
#include "core/macro.h"

namespace Pupil {
    EditorUI::EditorUI() {}
    // 实现window_ui的接口，这里用接口化方式主要是为了能够快速调用function layer中渲染层的window system能力生成ui
    void EditorUI::initialize(WindowUIInitInfo init_info) {}
}