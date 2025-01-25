#pragma once

#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <string.h>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <set>

#include "core/log/log.h"
#include "platform/rhi/vulkan/vulkan_struct.h"

#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

namespace Pupil {
    class WindowSystem;
    struct VulkanInterface {
        std::shared_ptr<WindowSystem> window_system;
    };

    class VulkanRHI {
    public:
        VulkanRHI() = default;
        ~VulkanRHI();
        GLFWwindow* window{ nullptr };
        VkSurfaceKHR surface{ nullptr };
        VkInstance instance{ nullptr };
        // 校验层, 可以不开启
        std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        std::vector<const char*> extensionsName;
        // 硬件拓展
        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        // debugmessager回调函数
        VkDebugUtilsMessengerEXT callback;
        VulkanConfig config;
        static const uint8_t maxFrameFlight = 8;

        bool initiative(VulkanInterface interface, VulkanConfig config);
        void destroy();
    private:
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicDevice = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        VkCommandPool defaultCommandPool = VK_NULL_HANDLE;
        VkCommandPool commandPools[maxFrameFlight];
        VkCommandBuffer defaultCommandBuffer;
        VkCommandBuffer commandBuffers[maxFrameFlight];

        // function pointer
        PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
        PFN_vkCmdEndDebugUtilsLabelEXT   _vkCmdEndDebugUtilsLabelEXT;
        PFN_vkWaitForFences         _vkWaitForFences;
        PFN_vkResetFences           _vkResetFences;
        PFN_vkResetCommandPool      _vkResetCommandPool;
        PFN_vkBeginCommandBuffer    _vkBeginCommandBuffer;
        PFN_vkEndCommandBuffer      _vkEndCommandBuffer;
        PFN_vkCmdBeginRenderPass    _vkCmdBeginRenderPass;
        PFN_vkCmdNextSubpass        _vkCmdNextSubpass;
        PFN_vkCmdEndRenderPass      _vkCmdEndRenderPass;
        PFN_vkCmdBindPipeline       _vkCmdBindPipeline;
        PFN_vkCmdSetViewport        _vkCmdSetViewport;
        PFN_vkCmdSetScissor         _vkCmdSetScissor;
        PFN_vkCmdBindVertexBuffers  _vkCmdBindVertexBuffers;
        PFN_vkCmdBindIndexBuffer    _vkCmdBindIndexBuffer;
        PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
        PFN_vkCmdDrawIndexed        _vkCmdDrawIndexed;
        PFN_vkCmdClearAttachments   _vkCmdClearAttachments;

        // create
        VkResult createInstance();
        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pCallback
        );
        VkResult createWindowSurface();
        VkResult pickPhysicalDevice();
        VkResult createLogicalDevice();
        VkResult createCommandPool();
        VkResult createCommandBuffers();

        // destroy
        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT callback);
        void DestroySwapChain();
    };
}