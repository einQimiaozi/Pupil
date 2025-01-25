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
		std::cout << "RequiredExtensions ========================\n";
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

	static std::string getVulkanVersion() {
		uint32_t apiVersion = VK_API_VERSION_1_0;
		if (vkEnumerateInstanceVersion) {
			vkEnumerateInstanceVersion(&apiVersion);
		}
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

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
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
}