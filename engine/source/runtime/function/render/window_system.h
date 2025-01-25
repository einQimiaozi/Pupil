#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "GLFW/glfw3.h"

namespace Pupil {
    // 窗口创建时信息
    struct WindowCreateInfo {
        int         width = 1280;
        int         height = 720;
        const char* title = "PupilEngine";
        bool        is_fullscreen = false;
    };

    class WindowSystem {
    public:
        WindowSystem() = default;
        void        initialize(WindowCreateInfo& create_info);
        uint32_t    getFps();

        void        setTitle(const char* title);
        static void pollEvents() { glfwPollEvents(); }
        bool        shouldClose() const;

        bool        tickOneFrame();

        GLFWwindow* window = nullptr;

    private:
        int         width = 0;
        int         height = 0;
        const char* title = nullptr;
    };
}