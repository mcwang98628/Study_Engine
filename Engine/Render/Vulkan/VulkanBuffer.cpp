#include "stdafx.h"
#include "vulkanControl.h"
#include "VulkanBuffer.h"
#include "VulkanDevices.h"

using namespace StudyEngine;

struct VulkanBuffer::Impl 
{
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
	VkDeviceSize bufferSize = 0;
    VkBufferUsageFlagBits ToVKBuffertType(type tp) {
        switch(tp)
        {
            case VERTEX:
                return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            case INDEX:
                return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            case UNIFORM:
                return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            default:
                std::cout << "StudyEngine::VulkanBuffer::Impl: failed to create VKBuffer!" << tp << std::endl;
                return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
    }
};

VulkanBuffer::VulkanBuffer(const void* data, size_t dataSize, type usage) : m_impl(std::make_unique<Impl>())
{
    VkDeviceSize bufferSize = dataSize;

    m_bufferType = usage;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* tempData;

    vkMapMemory(VulkanDevices::GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &tempData);

    memcpy(tempData, data, bufferSize);
    vkUnmapMemory(VulkanDevices::GetDevice(), stagingBufferMemory);

    VkBufferUsageFlagBits bufferTypeBit = m_impl->ToVKBuffertType(usage);
    VulkanControl::Get()->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferTypeBit, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_impl->buffer, m_impl->bufferMemory);

    VulkanControl::Get()->copyBuffer(stagingBuffer, m_impl->buffer, bufferSize);

    vkDestroyBuffer(VulkanDevices::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDevices::GetDevice(), stagingBufferMemory, nullptr);
}

VulkanBuffer::~VulkanBuffer(){
    vkDestroyBuffer(VulkanDevices::GetDevice(), m_impl->buffer, nullptr);
    vkFreeMemory(VulkanDevices::GetDevice(), m_impl->bufferMemory, nullptr);

};

void* VulkanBuffer::GetBufferHandle() const
{
	return nullptr;
}

void VulkanBuffer::BindBuffer()
{
    VkCommandBuffer cmd = VulkanControl::Get()->GetCommandBuffer();
    if (m_bufferType == VERTEX)
    {
        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(cmd, 0, 1, &m_impl->buffer, &offsets);
    }
    else if (m_bufferType == INDEX)
    {
        vkCmdBindIndexBuffer(cmd, m_impl->buffer, 0, VK_INDEX_TYPE_UINT32);
    }
}