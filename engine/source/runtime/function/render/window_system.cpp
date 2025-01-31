#include "window_system.h"

namespace Pupil {
    void WindowSystem::initialize(WindowCreateInfo& create_info) {
        if (!glfwInit()) {
            return;
        }

        width = create_info.width;
        height = create_info.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        if (!(window = glfwCreateWindow(width, height, create_info.title, nullptr, nullptr))) {
            glfwTerminate();
            return;
        }
        // glfwSetWindowUserPointer(window, this);
    }

    void WindowSystem::setTitle(const char* title) {
        this->title = title;
        glfwSetWindowTitle(window, title);
    }

    bool WindowSystem::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    uint32_t WindowSystem::getFps() {
        static double time0 = glfwGetTime();
        static double time1;
        static double dt;
        static int dframe = -1;
        static uint32_t fps = 0.0;
        time1 = glfwGetTime();
        dframe++;
        if ((dt = time1 - time0) >= 1) {
            fps = (uint32_t)dframe / dt;
            time0 = time1;
            dframe = 0;
        }
        return fps;
    }

    bool WindowSystem::tickOneFrame() {
        static std::stringstream title;
        title << "PupilEngine - " << this->getFps() << " FPS";
        this->setTitle(title.str().c_str());
        title.str("");

        this->pollEvents();
        return !this->shouldClose();
    }
}