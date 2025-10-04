#include "stdafx.h"
#include "VulkanSwapChain.h"
#include "VulkanDevices.h"
#include "VulkanCore.h"
#include "vulkanControl.h"
#include <GLFW/glfw3.h>

namespace StudyEngine {
    struct VulkanSwapChain::Impl {
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return availableFormat;
                }
            }

            return availableFormats[0];
        }
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
            for (const auto& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            }
            else {
                int width, height;
                //glfwGetFramebufferSize(curWindow, &width, &height);

                VkExtent2D actualExtent = {
                    static_cast<uint32_t>(WIDTH),
                    static_cast<uint32_t>(HEIGHT)
                };

                actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }

        void CreateImageViews() {
            swapChainImageViews.resize(swapChainImages.size());

            for (uint32_t i = 0; i < swapChainImages.size(); i++) {
                swapChainImageViews[i] = VulkanControl::Get()->createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
            }
        }

        void Init()
        {
            SwapChainSupportDetails swapChainSupport = VulkanControl::Get()->querySwapChainSupport(VulkanDevices::GetPhysicalDevice());

            VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
            VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
            VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = VulkanCore::GetSurface();

            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = VulkanControl::Get()->findQueueFamilies(VulkanDevices::GetPhysicalDevice());
            uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

            if (indices.graphicsFamily != indices.presentFamily) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;

            if (vkCreateSwapchainKHR(VulkanDevices::GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
                throw std::runtime_error("failed to create swap chain!");
            }

            vkGetSwapchainImagesKHR(VulkanDevices::GetDevice(), swapChain, &imageCount, nullptr);
            swapChainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(VulkanDevices::GetDevice(), swapChain, &imageCount, swapChainImages.data());

            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;
        }

        void CleanUp()
        {
            vkDestroyImageView(VulkanDevices::GetDevice(), depthImageView, nullptr);
            vkDestroyImage(VulkanDevices::GetDevice(), depthImage, nullptr);
            vkFreeMemory(VulkanDevices::GetDevice(), depthImageMemory, nullptr);

            for (auto framebuffer : swapChainFramebuffers) {
                vkDestroyFramebuffer(VulkanDevices::GetDevice(), framebuffer, nullptr);
            }

            for (auto imageView : swapChainImageViews) {
                vkDestroyImageView(VulkanDevices::GetDevice(), imageView, nullptr);
            }

            vkDestroySwapchainKHR(VulkanDevices::GetDevice(), swapChain, nullptr);
        }

        void CreateDepthResources()
        {
            VkFormat depthFormat = VulkanControl::Get()->findDepthFormat();
            VkExtent2D swapChainExtent = VulkanSwapChain::GetSwapChainExtent();
            VulkanControl::Get()->createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);

            depthImageView = VulkanControl::Get()->createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        }
        void CreateFramebuffers()
        {
            swapChainFramebuffers.resize(swapChainImageViews.size());

            for (size_t i = 0; i < swapChainImageViews.size(); i++) {
                std::array<VkImageView, 2> attachments = {
                    swapChainImageViews[i],
                    depthImageView
                };

                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = VulkanControl::Get()->GetRenderPass();
                framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
                framebufferInfo.pAttachments = attachments.data();
                framebufferInfo.width = swapChainExtent.width;
                framebufferInfo.height = swapChainExtent.height;
                framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(VulkanDevices::GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create framebuffer!");
                }
            }
        }
    };
    VulkanSwapChain::VulkanSwapChain() {
    }
    VulkanSwapChain::~VulkanSwapChain()
    {
    }
    void VulkanSwapChain::Init()
    {
        if (!m_impl)
        {
            m_impl = make_unique<Impl>();
        }
        m_impl->Init();
    }

    VkSwapchainKHR VulkanSwapChain::GetSwapChain()
    {
        return m_impl->swapChain;
    }

    void VulkanSwapChain::CreateImageViews()
    {
        m_impl->CreateImageViews();
    }

    void VulkanSwapChain::CreateFramebuffers()
    {
        m_impl->CreateFramebuffers();
    }

    VkFormat VulkanSwapChain::GetImageFormat()
    {
        return m_impl->swapChainImageFormat;
    }

    VkExtent2D VulkanSwapChain::GetSwapChainExtent()
    {
        return m_impl->swapChainExtent;
    }

    VkFramebuffer VulkanSwapChain::GetFrameBuffers(int index)
    {
        return m_impl->swapChainFramebuffers[index];

    }

    void VulkanSwapChain::RecreateSwapChain()
    {
        int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(WinApplication::GetWindow(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(VulkanDevices::GetDevice());
        //cleanupSwapChain();
        m_impl->CleanUp();
        
        m_impl->Init();
        //createSwapChain();
        m_impl->CreateImageViews();
        //createImageViews();
        m_impl->CreateDepthResources();
        m_impl->CreateFramebuffers();
    }


    void VulkanSwapChain::CleanUp() {
        m_impl->CleanUp();
    }

    void VulkanSwapChain::CreateDepthResources() {

        m_impl->CreateDepthResources();
    }
}

std::unique_ptr<StudyEngine::VulkanSwapChain::Impl> StudyEngine::VulkanSwapChain::m_impl;

