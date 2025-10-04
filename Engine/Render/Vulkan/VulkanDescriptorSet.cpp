#include "stdafx.h"
// #include "vulkan/vulkan.h"
#include "VulkanDescriptorSet.h"
#include "VulkanUniformBuffer.h"
#include "vulkanControl.h"
#include "vulkanDevices.h"
using namespace StudyEngine;
using namespace std;

struct VulkanDescriptorSet::Impl 
{
    vector<VkDescriptorSet> descriptorSets = vector<VkDescriptorSet>(MAX_FRAMES_IN_FLIGHT, VK_NULL_HANDLE);
};

VulkanDescriptorSet::VulkanDescriptorSet() : m_impl(std::make_unique<Impl>())
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, VulkanControl::Get()->descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = VulkanControl::Get()->descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(VulkanDevices::GetDevice(), &allocInfo, m_impl->descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = VulkanControl::Get()->textureImageView;
        imageInfo.sampler = VulkanControl::Get()->textureSampler;

        VkWriteDescriptorSet descriptorWrites{};

        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = m_impl->descriptorSets[i];
        descriptorWrites.dstBinding = 3;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(VulkanDevices::GetDevice(), 1, &descriptorWrites, 0, nullptr);
    }

}

void VulkanDescriptorSet::UpdateDescriptorSet(VulkanUniformBuffer* uBuffer, uint32_t destBinding, size_t dataSize, int frameIndex)
{
    VkDescriptorBufferInfo uniformBufferInfo{};
    uniformBufferInfo.buffer = uBuffer->GetBufferHandle();
    uniformBufferInfo.offset = 0;
    uniformBufferInfo.range = dataSize;

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = m_impl->descriptorSets[frameIndex];
    descriptorWrite.dstBinding = destBinding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &uniformBufferInfo;
    VkDevice dv = VulkanDevices::GetDevice();
    vkUpdateDescriptorSets(dv, 1, &descriptorWrite, 0, nullptr);
}

vector<VkDescriptorSet>& VulkanDescriptorSet::GetDescriptorSetHandle()
{
    return m_impl->descriptorSets;
}
