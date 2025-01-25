#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace Pupil {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    struct QueueFamilyIndices {
        int graphicsFamily = -1;
        int presentFamily = -1;
        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    struct VulkanConfig {
        bool enableValidation = true;
        bool fullscreen = false;
        bool vsync = true;
        bool overlay = true;
        bool enableRateShading = false;
        bool enablePointLightShadow = true;
    };
}