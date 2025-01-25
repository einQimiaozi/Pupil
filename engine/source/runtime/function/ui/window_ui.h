#pragma once

#include "function/render/window_system.h"
#include <memory>

namespace Pupil {
    // 窗口界面初始化信息
    struct WindowUIInitInfo {
        std::shared_ptr<WindowSystem> window_system;
    };

    // 纯接口类，ui实现WindowUI即可
    class WindowUI {
    public:
        virtual void initialize(WindowUIInitInfo init_info) = 0;
        // 纯虚函数：使派生类仅仅只是继承函数的接口(本类作为抽象类而存在)
    };
}