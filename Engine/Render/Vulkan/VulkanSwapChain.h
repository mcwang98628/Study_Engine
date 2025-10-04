#pragma once 
#include "stdafx.h"
#include <vulkan/vulkan.h>

using namespace std;

//typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
//typedef enum VkFormat_T* VkFormat;
//typedef struct VkExtent2D_T* VkExtent2D;
//typedef struct VkFramebuffer_T* VkFramebuffer;
//

namespace StudyEngine {
    class VulkanSwapChain {
        public:
            VulkanSwapChain();
            ~VulkanSwapChain();
            static void Init();
            static VkSwapchainKHR GetSwapChain();
            static void CreateImageViews();
            static void CreateFramebuffers();


            static VkFormat GetImageFormat();
            static VkExtent2D GetSwapChainExtent();
            static VkFramebuffer GetFrameBuffers(int index);


            static void RecreateSwapChain();

            static void CleanUp();

            static void CreateDepthResources();


        private:
            struct Impl;
            static std::unique_ptr<Impl> m_impl;
    };
};