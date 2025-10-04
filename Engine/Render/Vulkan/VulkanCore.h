#pragma once 
#include "stdafx.h"
#include <vulkan/vulkan.h>
using namespace std;

//typedef struct VkSurface_T* VkSurfaceKHR;
//typedef struct VkInstance_T* VkInstance;

namespace StudyEngine {
    class VulkanCore {
        public:
            VulkanCore();
            ~VulkanCore();
            static void Init();
            static VkSurfaceKHR GetSurface();
            static VkInstance GetInstance();

            static void CleanUp();

        private:
            struct Impl;
            static std::unique_ptr<Impl> m_impl;
    };
};