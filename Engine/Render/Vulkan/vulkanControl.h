#pragma once
#include "stdafx.h"
#include "Application/WinApplication.h"
#include <vulkan/vulkan.h>

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace StudyEngine {
    class VulkanControl {
    public:
        static VulkanControl* Get() {
            if (m_graphics == nullptr) {
                m_graphics = new VulkanControl();
            }
            return m_graphics;
            
        }

        //static VkPhysicalDevice physicalDevice;
        //static VkDevice logicalDevice;
        //static VkQueue graphicQueue;
        //static VkQueue presentQueue;
        //static VkSwapchainKHR swapChain;
        //static VkFormat swapChainFormat;
        //static VkExtent2D swapchainExtent;
        //static VkCommandPool commandPool;

        //VkPhysicalDevice physicalDevice;
        //VkDevice logicalDevice;
        // VkQueue graphicQueue;
        // VkQueue presentQueue;
        // VkSwapchainKHR swapChain;
        // VkFormat swapChainFormat;
        // VkExtent2D swapchainExtent;
        VkCommandPool commandPool;

        VkRenderPass renderPass;

        VkDescriptorSetLayout descriptorSetLayout;

        VkPipelineLayout pipelineLayout;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;

        VkSampler textureSampler;

        VkDescriptorPool descriptorPool;

        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        // static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        // static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        VkQueue* GetGraphicsQueue() { return &graphicsQueue; }
        VkQueue* GetPresentQueue() { return &presentQueue; }
        VkRenderPass GetRenderPass() { return renderPass; }



        VulkanControl();
        //void createSurface(GLFWwindow* window);
        // void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        //void createImageViews();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createRenderPass();
        void createDescriptorSetLayout();
        void createPipelineLayout();
        void createCommandPool();
        // void createDepthResources();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        //void createFramebuffers();
        void createTextureImage(std::string texturePath);
        void createTextureImageView();
        void createTextureSampler();
        void createDescriptorPool();
        void createCommandBuffers();
        void createSyncObjects();
        void beginRenderPass(VkCommandBuffer commandBuffer);
        void endRenderPass(VkCommandBuffer commandBuffer);
        VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
        void cleanUp();

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void SetViewportAndScissors(VkCommandBuffer commandBuffer);

        void BeginRenderer();
        void EndRenderer();
        VkCommandBuffer GetCommandBuffer();
        uint32_t GetCurrentFrameIndex();
        void WaitIdle();
        VkFormat findDepthFormat();


        ~VulkanControl();
    private:

        bool framebufferResized = false;
        uint32_t imageIndex;

        static VulkanControl* m_graphics;


        uint32_t currentFrame;

        //VkInstance instance;
        //VkSurfaceKHR surface;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        //void cleanupSwapChain();
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        void SubmitCommands();
        void PresentFrame();
    };
};
