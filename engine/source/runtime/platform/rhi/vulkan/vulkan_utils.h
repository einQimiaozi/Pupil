#pragma once

#include <unordered_map>
#include <string>

#include "runtime/platform/rhi/vulkan/vulkan_struct.h"
#include "runtime/core/macro.h"

namespace Pupil {
	class VulkanRHI;

	static std::unordered_map<VkResult, std::string> VkErrorMsg = {
		{VK_SUCCESS, "VK_SUCCESS"},
		{ VK_NOT_READY, "VK_NOT_READY" },
		{ VK_TIMEOUT, "VK_TIMEOUT" },
		{ VK_EVENT_SET, "VK_EVENT_SET" },
		{ VK_EVENT_RESET, "VK_EVENT_RESET" },
		{ VK_INCOMPLETE, "VK_INCOMPLETE" },
		{ VK_ERROR_OUT_OF_HOST_MEMORY, "VK_ERROR_OUT_OF_HOST_MEMORY" },
		{ VK_ERROR_OUT_OF_DEVICE_MEMORY, "VK_ERROR_OUT_OF_DEVICE_MEMORY" },
		{ VK_ERROR_INITIALIZATION_FAILED, "VK_ERROR_INITIALIZATION_FAILED" },
		{ VK_ERROR_DEVICE_LOST, "VK_ERROR_DEVICE_LOST" },
		{ VK_ERROR_MEMORY_MAP_FAILED, "VK_ERROR_MEMORY_MAP_FAILED" },
		{ VK_ERROR_LAYER_NOT_PRESENT, "VK_ERROR_LAYER_NOT_PRESENT" },
		{ VK_ERROR_EXTENSION_NOT_PRESENT, "VK_ERROR_EXTENSION_NOT_PRESENT" },
		{ VK_ERROR_FEATURE_NOT_PRESENT, "VK_ERROR_FEATURE_NOT_PRESENT" },
		{ VK_ERROR_INCOMPATIBLE_DRIVER, "VK_ERROR_INCOMPATIBLE_DRIVER" },
		{ VK_ERROR_TOO_MANY_OBJECTS, "VK_ERROR_TOO_MANY_OBJECTS" },
		{ VK_ERROR_FORMAT_NOT_SUPPORTED, "VK_ERROR_FORMAT_NOT_SUPPORTED" },
		{ VK_ERROR_FRAGMENTED_POOL, "VK_ERROR_FRAGMENTED_POOL" },
		{ VK_ERROR_UNKNOWN, "VK_ERROR_UNKNOWN" },
		{ VK_ERROR_OUT_OF_POOL_MEMORY, "VK_ERROR_OUT_OF_POOL_MEMORY" },
		{ VK_ERROR_INVALID_EXTERNAL_HANDLE, "VK_ERROR_INVALID_EXTERNAL_HANDLE" },
		{ VK_ERROR_FRAGMENTATION, "VK_ERROR_FRAGMENTATION" },
		{ VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS, "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS" },
		{ VK_ERROR_SURFACE_LOST_KHR, "VK_ERROR_SURFACE_LOST_KHR" },
		{ VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" },
		{ VK_SUBOPTIMAL_KHR, "VK_SUBOPTIMAL_KHR" },
		{ VK_ERROR_OUT_OF_DATE_KHR, "VK_ERROR_OUT_OF_DATE_KHR" },
		{ VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" },
		{ VK_ERROR_VALIDATION_FAILED_EXT, "VK_ERROR_VALIDATION_FAILED_EXT" },
		{ VK_ERROR_INVALID_SHADER_NV, "VK_ERROR_INVALID_SHADER_NV" },
		{ VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT" },
		{ VK_ERROR_NOT_PERMITTED_EXT, "VK_ERROR_NOT_PERMITTED_EXT" },
		{ VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT" },
		{ VK_THREAD_IDLE_KHR, "VK_THREAD_IDLE_KHR" },
		{ VK_THREAD_DONE_KHR, "VK_THREAD_DONE_KHR" },
		{ VK_OPERATION_DEFERRED_KHR, "VK_OPERATION_DEFERRED_KHR" },
		{ VK_OPERATION_NOT_DEFERRED_KHR, "VK_OPERATION_NOT_DEFERRED_KHR" },
		{ VK_PIPELINE_COMPILE_REQUIRED_EXT, "VK_PIPELINE_COMPILE_REQUIRED_EXT" },
		{ VK_ERROR_OUT_OF_POOL_MEMORY_KHR, "VK_ERROR_OUT_OF_POOL_MEMORY_KHR" },
		{ VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR, "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR" },
		{ VK_ERROR_FRAGMENTATION_EXT, "VK_ERROR_FRAGMENTATION_EXT" },
		{ VK_ERROR_INVALID_DEVICE_ADDRESS_EXT, "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT" },
		{ VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR, "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR" },
		{ VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT, "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT" },
		{ VK_RESULT_MAX_ENUM, "VK_RESULT_MAX_ENUM" }
	};

	static std::vector<const char*> getRequiredExtensions(bool enableValidationLayers) {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		std::cout << "============ RequiredExtensions ============\n";
		for (int i = 0; i < extensions.size(); i++) {
			std::cout << extensions[i] << "\n";
		}
		std::cout << "============================================\n";
		return extensions;
	}

	static bool checkExtensionSupport(std::vector<const char*> extensionsName) {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		for (const char* checkExtension : extensionsName) {
			bool check = false;
			for (const auto& extension : extensions) {
				if (strcmp(extension.extensionName, checkExtension) == 0) {
					check = true;
					break;
				}
			}
			if (!check) {
				return false;
			}
		}
		return true;
	}

	static bool checkValidationLayerSupport(std::vector<const char*> validationLayers) {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (const char* checkLayer : validationLayers) {
			bool check = false;
			for (const auto& layer : layers) {
				if (strcmp(layer.layerName, checkLayer) == 0) {
					check = true;
					break;
				}
			}
			if (!check) {
				return false;
			}
		}
		return true;
	}

	static uint32_t getVulkanVersion() {
		uint32_t apiVersion = VK_API_VERSION_1_0;
		if (vkEnumerateInstanceVersion) {
			vkEnumerateInstanceVersion(&apiVersion);
		}
		return apiVersion;
	}

	static std::string getVulkanVersionStr() {
		uint32_t apiVersion = getVulkanVersion();
		uint32_t major = VK_API_VERSION_MAJOR(apiVersion);
		uint32_t minor = VK_API_VERSION_MINOR(apiVersion);
		uint32_t patch = VK_API_VERSION_PATCH(apiVersion);
		std::stringstream ss;
		ss << major << '.' << minor << '.' << patch;
		std::string version = ss.str();
		ss.str("");
		return version;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		std::cerr << "validationlayer: " << pCallbackData->pMessage << "\n";
		return VK_FALSE;
	}

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
		}
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
		}
		return details;
	}

	static bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions) {
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string>requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
		return requiredExtensions.empty();
	}

	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			if (queueFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}
			i++;
		}
		return indices;
	}

	static bool isDeviceSuitable(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions, VkSurfaceKHR surface) {
		bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions);
		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
		bool checkFeatures = true;
		if (!supportedFeatures.samplerAnisotropy) {
			checkFeatures = false;
		}
		return findQueueFamilies(physicalDevice, surface).isComplete() && extensionsSupported && swapChainAdequate && checkFeatures;
	}

	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	}

	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
		return mode;
	}

	static 	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actualExtent = {
			static_cast <uint32_t>(width) ,
			static_cast <uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}

	static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		LOG_ERROR("failed to find suitable memory type!");
		return 0;
	}

	static VkImageView_t createImageView(
		VkDevice logicDevice,
		VkImage& image,
		VkFormat           format,
		VkImageAspectFlags image_aspect_flags,
		VkImageViewType    view_type,
		uint32_t           layout_count,
		uint32_t           miplevels
	) {
		VkImageView_t imageView_t;
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = view_type;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = image_aspect_flags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = miplevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = layout_count;

		imageView_t.result = vkCreateImageView(logicDevice, &viewInfo, nullptr, &imageView_t.imageView);
		return imageView_t;
	}

	static VkResult createImage(
		VkPhysicalDevice      physicalDevice,
		VkDevice              logicDevice,
		uint32_t              width,
		uint32_t              height,
		VkFormat              format,
		VkImageTiling         tiling,
		VkImageUsageFlags     usageFlags,
		VkMemoryPropertyFlags memoryPropertyFlags,
		VkImage& image,
		VkDeviceMemory& imageMemory,
		VkImageCreateFlags    flags,
		uint32_t              layers,
		uint32_t              miplevels,
		VkSampleCountFlagBits numSamples
	) {
		VkResult result;
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast <uint32_t>(width);
		imageInfo.extent.height = static_cast <uint32_t>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = miplevels;
		imageInfo.arrayLayers = layers;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usageFlags;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = numSamples;
		imageInfo.flags = flags;

		if ((result = vkCreateImage(logicDevice, &imageInfo, nullptr, &image)) != VK_SUCCESS) {
			return result;
		}

		// alloc image memoty
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(logicDevice, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, memoryPropertyFlags);
		if ((result = vkAllocateMemory(logicDevice, &allocInfo, nullptr, &imageMemory)) != VK_SUCCESS) {
			return result;
		}
		vkBindImageMemory(logicDevice, image, imageMemory, 0);
		return VK_SUCCESS;
	}

	static VkFormat_t findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector <VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		VkFormat_t result{
			VK_RESULT_MAX_ENUM,
			VK_FORMAT_UNDEFINED
		};
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
			if ((tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) || 
				(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)) {
				result.result = VK_SUCCESS;
				result.format = format;
			}
			return result;
		}
	}

	static VkFormat_t findDepthFormat(VkPhysicalDevice physicalDevice) {
		return findSupportedFormat(
			physicalDevice,
			{
				VK_FORMAT_D32_SFLOAT,
				VK_FORMAT_D32_SFLOAT_S8_UINT,
				VK_FORMAT_D24_UNORM_S8_UINT
			},
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	static void copyBufferToImage(VulkanRHI* rhi, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) {
		VkCommandBuffer commandBuffer = rhi->beginSingleTimeCommands();

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0 ,0 ,0 };
		region.imageExtent = { width, height ,1 };
		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		rhi->endSingleTimeCommands(commandBuffer);
	}

	static void transitionImageLayout(VulkanRHI* rhi, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount, uint32_t mipLevels, VkImageAspectFlags aspectMask) {
		VkCommandBuffer commandBuffer = rhi->beginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = aspectMask;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			// 规则: 如果是从未定义布局，到传输目的布局
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			// 规则: 如果是从可写入布局，也就是传输目的布局，到shader可读布局
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
			// 规则: 深度附着布局转换到传输源, 一般是刚做完的深度测试拿来给下一阶段管线用
			barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			// 规则: 传输源到深度附着布局
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
			// 规则: 输出布局到源布局
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			// 规则: 从未定义布局转到深度附着布局
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
			// 规则: 从未定义布局转换到颜色附着，一般msaa之类的需要使用颜色附着的图像需要配置这个规则
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}else {
			LOG_ERROR("unsupported layout transition!");
			return;
		}

		// 创建布局变换barrier
		/*
		 * VkCommandBuffer                             commandBuffer,
		 * VkPipelineStageFlags                        srcStageMask,				// 指定屏障前的管线阶段
		 * VkPipelineStageFlags                        dstStageMask,				// 指定屏障后的管线阶段
		 * VkDependencyFlags                           dependencyFlags,			// 是否允许读取资源目前写入的部分，这个没看懂
		 * uint32_t                                    memoryBarrierCount,		// 配置是否为内存屏障
		 * const VkMemoryBarrier*                      pMemoryBarriers,
		 * uint32_t                                    bufferMemoryBarrierCount,	// 配置是否为缓冲内存屏障
		 * const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
		 * uint32_t                                    imageMemoryBarrierCount,	// 配置是否为图像内存屏障
		 * const VkImageMemoryBarrier*                 pImageMemoryBarriers);
		*/
		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		rhi->endSingleTimeCommands(commandBuffer);
	}

	static void generateMipmapsTexture(VulkanRHI* rhi, VkImage& image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t layerCount, uint32_t mipLevels) {
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(rhi->physicalDevice, imageFormat, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
			LOG_ERROR("generateMipmaps(): texture image format does not support linear blit ting!");
		}

		VkCommandBuffer commandBuffer = rhi->beginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		for (uint32_t i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			VkImageBlit blit = {};
			blit.srcOffsets[0] = { 0 , 0 , 0 };
			blit.srcOffsets[1] = { mipWidth , mipHeight , 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = layerCount;

			blit.dstOffsets[0] = { 0 , 0 , 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1 ,mipHeight > 1 ? mipHeight / 2 : 1 , 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = layerCount;

			vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}
		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		// 结束指令提交
		rhi->endSingleTimeCommands(commandBuffer);
	}

	static void generateMipmapsImage(VulkanRHI* rhi, VkImage& image, uint32_t width, uint32_t height, uint32_t mipLevels) {
        VkCommandBuffer commandBuffer = rhi->beginSingleTimeCommands();

        for (uint32_t i = 1; i < mipLevels; i++)
        {
            VkImageBlit imageBlit {};
            imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.srcSubresource.layerCount = 1;
            imageBlit.srcSubresource.mipLevel   = i - 1;
            imageBlit.srcOffsets[1].x           = std::max((int32_t)(width >> (i - 1)), 1);
            imageBlit.srcOffsets[1].y           = std::max((int32_t)(height >> (i - 1)), 1);
            imageBlit.srcOffsets[1].z           = 1;

            imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.dstSubresource.layerCount = 1;
            imageBlit.dstSubresource.mipLevel   = i;
            imageBlit.dstOffsets[1].x           = std::max((int32_t)(width >> i), 1);
            imageBlit.dstOffsets[1].y           = std::max((int32_t)(height >> i), 1);
            imageBlit.dstOffsets[1].z           = 1;

            VkImageSubresourceRange mipSubRange {};
            mipSubRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
            mipSubRange.baseMipLevel = i;
            mipSubRange.levelCount   = 1;
            mipSubRange.layerCount   = 1;

            VkImageMemoryBarrier barrier {};
            barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.srcAccessMask       = 0;
            barrier.dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image               = image;
            barrier.subresourceRange    = mipSubRange;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
            vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

            barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        }

        VkImageSubresourceRange mipSubRange {};
        mipSubRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
        mipSubRange.baseMipLevel = 0;
        mipSubRange.levelCount   = mipLevels;
        mipSubRange.layerCount   = 1;

        VkImageMemoryBarrier barrier {};
        barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask       = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask       = VK_ACCESS_SHADER_READ_BIT;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image               = image;
        barrier.subresourceRange    = mipSubRange;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,0, nullptr, 0, nullptr, 1, &barrier);
        rhi->endSingleTimeCommands(commandBuffer);
    }
}