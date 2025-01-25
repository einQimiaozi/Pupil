#include "global_context.h"

namespace Pupil {
    RuntimeGlobalContext runtime_global_context; // ����ʱ������(ȫ�ֱ���)

    void RuntimeGlobalContext::startSystems(const std::string& config_file_path) {
        // ��ʼ��logϵͳ
        log_system = std::make_shared<Log>();
        // ��ʼ���Ӵ�ϵͳ
        window_system = std::make_shared<WindowSystem>();
        // todo: ��config�ж�ȡwindow create info
        WindowCreateInfo window_create_info;
        window_system->initialize(window_create_info);
        LOG_INFO("window system initialized");
        // ��ʼ����Ⱦϵͳ
        render_system = std::make_shared<RenderSystem>();
        RenderInterface r_interface = { window_system };
        render_system->initialize(r_interface);

        // ��ʼ��rhi
        rhi = std::make_shared<VulkanRHI>();
        VulkanInterface v_interface = { window_system };
        VulkanConfig v_config;
        rhi->initiative(v_interface, v_config);

        // todo: ��ʼ������ϵͳ
    }
    void RuntimeGlobalContext::shutdownSystems() {
        rhi.reset();
        render_system.reset();
        window_system.reset();
        log_system.reset();
    }
}