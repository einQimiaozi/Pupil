#pragma once

#include <unordered_map>
#include <string>

#include "platform/rhi/vulkan/vulkan_struct.h"

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

	// 检查层是否支持
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

	// debugmessager回调函数
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		std::cerr << "validationlayer: " << pCallbackData->pMessage << "\n";
		return VK_FALSE;
	}

	// 查询物理设备支持的交换链信息
	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		SwapChainSupportDetails details;
		// 1. 查询交换链上的表面特性，这个查询用于判断交换链和我们的表面拓展是否兼容
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
		// 2. 查询表面支持的格式
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		if (formatCount != 0) {
			// 确保表面支持的格式列表空间大小合法
			// VkSurfaceFormatKHR这个就是表面支持的格式的结构体
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
		}
		// 3. 查询表面支持的呈现模式
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			// 确保表面呈现模式列表空间大小合法
			// VkPresentModeKHR这个就是表面呈现模式的结构体
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
		}
		// 对于我们的这个程序来说，我们只需要交换链支持至少一种图像格式，和一种和我们窗口表面兼容的呈现模式即可
		return details;
	}

	// 检查设备级拓展是否支持
	static bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions) {
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		// 拷贝了一个set，去重 + O(1)时间查询availableExtensions和deviceExtensions是否有重合
		std::set<std::string>requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
		// 查到一个extension就从requiredExtensions里删除一个，如果最后requiredExtensions的size为0，则说明所有requiredExtensions均被设备支持
		return requiredExtensions.empty();
	}

	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		// 设备队列族索引
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		// 遍历设备支持的队列族，我们找到一个下属有至少一个队列且支持图形的队列族
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			// 支持查询呈现队列族
			VkBool32 presentSupport = false;
			// 检查当前物理设备是否具有呈现能力
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			// 如果当前物理设备支持呈现，则将呈现队列族设置为当前队列族，这里没太看明白，感觉就是如果硬件设备支持呈现那队列族就一定支持？
			if (queueFamily.queueCount > 0 && presentSupport) {
				// 呈现队列族和图形队列族是同一个队列族也没关系，实际上这样性能会更好，减少队列之间的数据交换
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}
			i++;
		}
		return indices;
	}

	// 检测物理设备是否能获取到队列族
	static bool isDeviceSuitable(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions, VkSurfaceKHR surface) {
		// 检查设备级拓展
		bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions);
		// 检查表面和交换链的兼容性
		bool swapChainAdequate = false;
		// 首先设备级拓展必须支持，才能检测交换链是否和表面兼容，否则交换链这个拓展都不支持的话就不用检测了
		if (extensionsSupported) {
			// 获取设备级的交换链支持信息
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
			// 至少查询到一个支持的图像格式和一个支持的呈现模式
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		// 获取硬件设备特性，这里是为了检查一些我们希望使用的特性
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
		// 检查设备是否支持各向异性过滤
		bool checkFeatures = true;
		if (!supportedFeatures.samplerAnisotropy) {
			checkFeatures = false;
		}
		return findQueueFamilies(physicalDevice, surface).isComplete() && extensionsSupported && swapChainAdequate && checkFeatures;
	}

	// 查询一个合适的交换链格式
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		// 1. 选择颜色格式和颜色空间，这里我们尽可能支持SRGB空间，对应的宏是VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			// 如果当前支持的格式只有VK_FORMAT_UNDEFINED，我们就强行返回SRGB格式和色彩空间
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}
		// 2.否则我们循环查找是否有VK_FORMAT_B8G8R8A8_UNORM和VK_COLOR_SPACE_SRGB_NONLINEAR_KHR的组合
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		// 3. 如果都多个颜色格式和颜色空间，但是没有哪一种恰好符合VK_FORMAT_B8G8R8A8_UNORM和VK_COLOR_SPACE_SRGB_NONLINEAR_KHR的组合，我们就返回format中的第一个，算是勉强能用
		return availableFormats[0];
	}

	// 选择呈现模式
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		/*
		* 呈现模式有四种
		* VK_PRESENT_MODE_IMMEDIATE_KHR：应用程序提交的图像会被立即传输到屏幕上，可能会导致撕裂现象。
		* VK_PRESENT_MODE_FIFO_KHR：交换链变成一个先进先出的队列，每次从队列头部取出一张图像进行显示，应用程序渲染的图像提交给交换链后，会被放在队列尾部。当队列为满时，应用程序需要进行等待。
		* 这一模式非常类似现在常用的垂直同步。刷新显示的时刻也被叫做垂直回扫。
		* VK_PRESENT_MODE_FIFO_RELAXED_KHR：这一模式和上一模式的唯一区别是，如果应用程序延迟，导致交换链的队列在上一次垂直回扫时为空，那么，如果应用程序在下一次垂直回扫前提交图像，图像会立即被显示。这一模式可能会导致撕裂现象。
		* VK_PRESENT_MODE_MAILBOX_KHR：这一模式是第二种模式的另一个变种。它不会在交换链的队列满时阻塞应用程序，队列中的图像会被直接替换为应用程序新提交的图像。这一模式可以用来实现三倍缓冲，避免撕裂现象的同时减小了延迟问题。
		* VK_PRESENT_MODE_MAILBOX_KHR效果最好，有它就用它，VK_PRESENT_MODE_FIFO_KHR保证一定可用，VK_PRESENT_MODE_IMMEDIATE_KHR在性能较差的设备上效果最好
		*/
		// 所以我们选择呈现模式的逻辑是: 遍历所有呈现模式，有VK_PRESENT_MODE_MAILBOX_KHR就返回，有VK_PRESENT_MODE_IMMEDIATE_KHR就记录一下，如果最后没有记录，则使用VK_PRESENT_MODE_FIFO_KHR
		VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
		return mode;
	}

	static 	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
		// 如果当前表面给的currentExtent为uint32_t的最大值，那么就是告诉我们，当前的表面允许我们自定义图像分辨率，否则就不能自定义，直接返回capabilities中的currentExtent
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		// 获取当前窗口的实际宽高，改变窗口大小的时候会用到这个功能, 默认情况下我们的交换链分辨率以窗口大小为准
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actualExtent = {
			static_cast <uint32_t>(width) ,
			static_cast <uint32_t>(height)
		};

		// 做一下clamp，防止当前窗口不支持我们自定义的分辨率
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}

	// 选择内存类型
	static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		// 1. vkGetPhysicalDeviceMemoryProperties可以根据物理设备查询设备可用的内存类型
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
		// 2. 查找合适的类型，我们通过typeFilter查询我们需要的类型，typeFilter本质上就是个位, properties用于实际筛选我们需要的特性
		// ps: 从显卡角度看，最合适的内存类型是VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT标记的类型, 该类型的内存读写极快，但是没办法被cpu读取
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		throw std::runtime_error("failed to find suitable memory type!");
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
			// 获取设备支持的format的属性，这里我们可以传入一个format数组，用于检测我们需要的多个format
			/*
			* VkFormatProperties的成员如下
			* linearTilingFeatures：数据格式支持线性tiling模式
			* optimalTilingFeatures：数据格式支持优化tiling模式
			* bufferFeatures：数据格式支持缓冲
			* 所以首先我们需要检测，设备是否支持我们想要的图像存储格式
			* 如果我们需要线性存储，则需要设备支持linearTilingFeatures
			* 如果我们需要优化存储，则需要设备支持optimalTilingFeatures
			*/
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
}