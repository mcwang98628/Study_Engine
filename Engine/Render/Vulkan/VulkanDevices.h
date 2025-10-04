#pragma once 
#include "stdafx.h"
#include <vulkan/vulkan.h>

using namespace std;

//typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
//typedef struct VkDevice_T* VkDevice;

namespace StudyEngine {
    class VulkanDevices {
        public:
            VulkanDevices();
            ~VulkanDevices();
            static void Init();
            static VkPhysicalDevice GetPhysicalDevice();
            static VkDevice GetDevice();
            static void CleanUp();
        private:
            struct Impl;
            static std::unique_ptr<Impl> m_impl;
            
    };
};