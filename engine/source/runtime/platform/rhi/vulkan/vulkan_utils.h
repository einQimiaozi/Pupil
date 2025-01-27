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

	// �����Ƿ�֧��
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

	// debugmessager�ص�����
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		std::cerr << "validationlayer: " << pCallbackData->pMessage << "\n";
		return VK_FALSE;
	}

	// ��ѯ�����豸֧�ֵĽ�������Ϣ
	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		SwapChainSupportDetails details;
		// 1. ��ѯ�������ϵı������ԣ������ѯ�����жϽ����������ǵı�����չ�Ƿ����
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
		// 2. ��ѯ����֧�ֵĸ�ʽ
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		if (formatCount != 0) {
			// ȷ������֧�ֵĸ�ʽ�б�ռ��С�Ϸ�
			// VkSurfaceFormatKHR������Ǳ���֧�ֵĸ�ʽ�Ľṹ��
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
		}
		// 3. ��ѯ����֧�ֵĳ���ģʽ
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			// ȷ���������ģʽ�б�ռ��С�Ϸ�
			// VkPresentModeKHR������Ǳ������ģʽ�Ľṹ��
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
		}
		// �������ǵ����������˵������ֻ��Ҫ������֧������һ��ͼ���ʽ����һ�ֺ����Ǵ��ڱ�����ݵĳ���ģʽ����
		return details;
	}

	// ����豸����չ�Ƿ�֧��
	static bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions) {
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		// ������һ��set��ȥ�� + O(1)ʱ���ѯavailableExtensions��deviceExtensions�Ƿ����غ�
		std::set<std::string>requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
		// �鵽һ��extension�ʹ�requiredExtensions��ɾ��һ����������requiredExtensions��sizeΪ0����˵������requiredExtensions�����豸֧��
		return requiredExtensions.empty();
	}

	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		// �豸����������
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		// �����豸֧�ֵĶ����壬�����ҵ�һ������������һ��������֧��ͼ�εĶ�����
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			// ֧�ֲ�ѯ���ֶ�����
			VkBool32 presentSupport = false;
			// ��鵱ǰ�����豸�Ƿ���г�������
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			// �����ǰ�����豸֧�ֳ��֣��򽫳��ֶ���������Ϊ��ǰ�����壬����û̫�����ף��о��������Ӳ���豸֧�ֳ����Ƕ������һ��֧�֣�
			if (queueFamily.queueCount > 0 && presentSupport) {
				// ���ֶ������ͼ�ζ�������ͬһ��������Ҳû��ϵ��ʵ�����������ܻ���ã����ٶ���֮������ݽ���
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}
			i++;
		}
		return indices;
	}

	// ��������豸�Ƿ��ܻ�ȡ��������
	static bool isDeviceSuitable(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions, VkSurfaceKHR surface) {
		// ����豸����չ
		bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions);
		// ������ͽ������ļ�����
		bool swapChainAdequate = false;
		// �����豸����չ����֧�֣����ܼ�⽻�����Ƿ�ͱ�����ݣ����򽻻��������չ����֧�ֵĻ��Ͳ��ü����
		if (extensionsSupported) {
			// ��ȡ�豸���Ľ�����֧����Ϣ
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
			// ���ٲ�ѯ��һ��֧�ֵ�ͼ���ʽ��һ��֧�ֵĳ���ģʽ
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		// ��ȡӲ���豸���ԣ�������Ϊ�˼��һЩ����ϣ��ʹ�õ�����
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
		// ����豸�Ƿ�֧�ָ������Թ���
		bool checkFeatures = true;
		if (!supportedFeatures.samplerAnisotropy) {
			checkFeatures = false;
		}
		return findQueueFamilies(physicalDevice, surface).isComplete() && extensionsSupported && swapChainAdequate && checkFeatures;
	}

	// ��ѯһ�����ʵĽ�������ʽ
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		// 1. ѡ����ɫ��ʽ����ɫ�ռ䣬�������Ǿ�����֧��SRGB�ռ䣬��Ӧ�ĺ���VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			// �����ǰ֧�ֵĸ�ʽֻ��VK_FORMAT_UNDEFINED�����Ǿ�ǿ�з���SRGB��ʽ��ɫ�ʿռ�
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}
		// 2.��������ѭ�������Ƿ���VK_FORMAT_B8G8R8A8_UNORM��VK_COLOR_SPACE_SRGB_NONLINEAR_KHR�����
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		// 3. ����������ɫ��ʽ����ɫ�ռ䣬����û����һ��ǡ�÷���VK_FORMAT_B8G8R8A8_UNORM��VK_COLOR_SPACE_SRGB_NONLINEAR_KHR����ϣ����Ǿͷ���format�еĵ�һ����������ǿ����
		return availableFormats[0];
	}

	// ѡ�����ģʽ
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		/*
		* ����ģʽ������
		* VK_PRESENT_MODE_IMMEDIATE_KHR��Ӧ�ó����ύ��ͼ��ᱻ�������䵽��Ļ�ϣ����ܻᵼ��˺������
		* VK_PRESENT_MODE_FIFO_KHR�����������һ���Ƚ��ȳ��Ķ��У�ÿ�δӶ���ͷ��ȡ��һ��ͼ�������ʾ��Ӧ�ó�����Ⱦ��ͼ���ύ���������󣬻ᱻ���ڶ���β����������Ϊ��ʱ��Ӧ�ó�����Ҫ���еȴ���
		* ��һģʽ�ǳ��������ڳ��õĴ�ֱͬ����ˢ����ʾ��ʱ��Ҳ��������ֱ��ɨ��
		* VK_PRESENT_MODE_FIFO_RELAXED_KHR����һģʽ����һģʽ��Ψһ�����ǣ����Ӧ�ó����ӳ٣����½������Ķ�������һ�δ�ֱ��ɨʱΪ�գ���ô�����Ӧ�ó�������һ�δ�ֱ��ɨǰ�ύͼ��ͼ�����������ʾ����һģʽ���ܻᵼ��˺������
		* VK_PRESENT_MODE_MAILBOX_KHR����һģʽ�ǵڶ���ģʽ����һ�����֡��������ڽ������Ķ�����ʱ����Ӧ�ó��򣬶����е�ͼ��ᱻֱ���滻ΪӦ�ó������ύ��ͼ����һģʽ��������ʵ���������壬����˺�������ͬʱ��С���ӳ����⡣
		* VK_PRESENT_MODE_MAILBOX_KHRЧ����ã�������������VK_PRESENT_MODE_FIFO_KHR��֤һ�����ã�VK_PRESENT_MODE_IMMEDIATE_KHR�����ܽϲ���豸��Ч�����
		*/
		// ��������ѡ�����ģʽ���߼���: �������г���ģʽ����VK_PRESENT_MODE_MAILBOX_KHR�ͷ��أ���VK_PRESENT_MODE_IMMEDIATE_KHR�ͼ�¼һ�£�������û�м�¼����ʹ��VK_PRESENT_MODE_FIFO_KHR
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
		// �����ǰ�������currentExtentΪuint32_t�����ֵ����ô���Ǹ������ǣ���ǰ�ı������������Զ���ͼ��ֱ��ʣ�����Ͳ����Զ��壬ֱ�ӷ���capabilities�е�currentExtent
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		// ��ȡ��ǰ���ڵ�ʵ�ʿ�ߣ��ı䴰�ڴ�С��ʱ����õ��������, Ĭ����������ǵĽ������ֱ����Դ��ڴ�СΪ׼
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actualExtent = {
			static_cast <uint32_t>(width) ,
			static_cast <uint32_t>(height)
		};

		// ��һ��clamp����ֹ��ǰ���ڲ�֧�������Զ���ķֱ���
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}

	// ѡ���ڴ�����
	static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		// 1. vkGetPhysicalDeviceMemoryProperties���Ը��������豸��ѯ�豸���õ��ڴ�����
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
		// 2. ���Һ��ʵ����ͣ�����ͨ��typeFilter��ѯ������Ҫ�����ͣ�typeFilter�����Ͼ��Ǹ�λ, properties����ʵ��ɸѡ������Ҫ������
		// ps: ���Կ��Ƕȿ�������ʵ��ڴ�������VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT��ǵ�����, �����͵��ڴ��д���죬����û�취��cpu��ȡ
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
			// ��ȡ�豸֧�ֵ�format�����ԣ��������ǿ��Դ���һ��format���飬���ڼ��������Ҫ�Ķ��format
			/*
			* VkFormatProperties�ĳ�Ա����
			* linearTilingFeatures�����ݸ�ʽ֧������tilingģʽ
			* optimalTilingFeatures�����ݸ�ʽ֧���Ż�tilingģʽ
			* bufferFeatures�����ݸ�ʽ֧�ֻ���
			* ��������������Ҫ��⣬�豸�Ƿ�֧��������Ҫ��ͼ��洢��ʽ
			* ���������Ҫ���Դ洢������Ҫ�豸֧��linearTilingFeatures
			* ���������Ҫ�Ż��洢������Ҫ�豸֧��optimalTilingFeatures
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