#include "stdafx.h"
#include "VulkanCore.h"
//#include "vulkan/vulkan.h"
#include "vulkanControl.h"
#include <GLFW/glfw3.h>


namespace StudyEngine {
	struct VulkanCore::Impl {
        static VkDebugUtilsMessengerEXT debugMessenger;
		static VkInstance instance;
		static VkSurfaceKHR surface;

		//static VkPhysicalDevice physicalDevice;
		//static VkDevice logicalDevice;
		//static VkQueue graphicQueue;
		//static VkQueue presentQueue;
		//static VkSwapchainKHR swapChain;
		//static VkFormat swapChainFormat;
		//static VkExtent2D swapchainExtent;
		//static VkCommandPool commandPool;

        static void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }

        static std::vector<const char*> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if (enableValidationLayers) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

            return VK_FALSE;
        }

        static bool checkValidationLayerSupport() {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char* layerName : validationLayers) {
                bool layerFound = false;

                for (const auto& layerProperties : availableLayers) {
                    if (strcmp(layerName, layerProperties.layerName) == 0) {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound) {
                    return false;
                }
            }

            return true;
        }

		static void CreateVulkanInstance()
		{
            if (enableValidationLayers && !checkValidationLayerSupport()) {
                throw std::runtime_error("validation layers requested, but not available!");
            }

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Hello Triangle";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            auto extensions = getRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();

                debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debugCreateInfo.pfnUserCallback = debugCallback;

                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
            }
            else {
                createInfo.enabledLayerCount = 0;

                createInfo.pNext = nullptr;
            }

            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                throw std::runtime_error("failed to create instance!");
            }
		}

        static void CreateDebugMessenger()
        {
            if (!enableValidationLayers) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = m_impl->debugCallback;

            auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func == nullptr || func(instance, &createInfo, nullptr, &m_impl->debugMessenger) != VK_SUCCESS) {
                throw std::runtime_error("failed to set up debug messenger!");
            }

            //if (VulkanControl::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &m_impl->debugMessenger) != VK_SUCCESS) {
            //    throw std::runtime_error("failed to set up debug messenger!");
            //}
        }

        static void CreateSurface() {
            if (glfwCreateWindowSurface(instance, WinApplication::GetWindow(), nullptr, &surface) != VK_SUCCESS) {
                throw std::runtime_error("failed to create window surface!");
            }
        }

        static void CleanUp()
        {
            if (enableValidationLayers) {
                DestroyDebugUtilsMessengerEXT(nullptr);
            }
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
        }

	};
	VulkanCore::VulkanCore()
	{

	}
    VulkanCore::~VulkanCore()
    {
        CleanUp();
        /*if (enableValidationLayers) {
            m_impl->DestroyDebugUtilsMessengerEXT(nullptr);
        }*/
    }
    void VulkanCore::Init()
    {
        if (!m_impl)
        {
            m_impl = make_unique<Impl>();
        }
        m_impl->CreateVulkanInstance();
        m_impl->CreateDebugMessenger();
        m_impl->CreateSurface();
    }
    VkSurfaceKHR VulkanCore::GetSurface()
    {
        return m_impl->surface;
    }
    VkInstance VulkanCore::GetInstance()
    {
        return m_impl->instance;
    }
    void VulkanCore::CleanUp()
    {
        m_impl->CleanUp();
    }
}



std::unique_ptr<StudyEngine::VulkanCore::Impl> StudyEngine::VulkanCore::m_impl;

VkDebugUtilsMessengerEXT StudyEngine::VulkanCore::Impl::debugMessenger;
VkInstance StudyEngine::VulkanCore::Impl::instance;
VkSurfaceKHR StudyEngine::VulkanCore::Impl::surface;
