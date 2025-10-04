#pragma once 
#include "stdafx.h"
#include "VulkanUniformBuffer.h"
//#include <vulkan/vulkan.h>

using namespace std;

typedef struct VkDescriptorSet_T* VkDescriptorSet;

namespace StudyEngine {
    class VulkanDescriptorSet {
    public:
        VulkanDescriptorSet();
        void UpdateDescriptorSet(VulkanUniformBuffer* uBuffer, uint32_t destBinding, size_t dataSize, int frameIndex);
        vector<VkDescriptorSet>& GetDescriptorSetHandle();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
};