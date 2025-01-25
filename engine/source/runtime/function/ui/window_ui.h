#pragma once

#include "function/render/window_system.h"
#include <memory>

namespace Pupil {
    // ���ڽ����ʼ����Ϣ
    struct WindowUIInitInfo {
        std::shared_ptr<WindowSystem> window_system;
    };

    // ���ӿ��࣬uiʵ��WindowUI����
    class WindowUI {
    public:
        virtual void initialize(WindowUIInitInfo init_info) = 0;
        // ���麯����ʹ���������ֻ�Ǽ̳к����Ľӿ�(������Ϊ�����������)
    };
}