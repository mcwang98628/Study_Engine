#include "stdafx.h"
#include "VulkanUniformBuffer.h"
#include "vulkanControl.h"
#include "VulkanDevices.h"
using namespace StudyEngine;

struct VulkanUniformBuffer::Impl
{
	void* bufferData;
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
	void* bufferMapped = nullptr;
};

VulkanUniformBuffer::VulkanUniformBuffer(size_t dataSize) : m_impl(std::make_unique<Impl>())
{
	VkDeviceSize bufferSize = dataSize;
	VulkanControl::Get()->createBuffer(bufferSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		m_impl->buffer,
		m_impl->bufferMemory);
	vkMapMemory(VulkanDevices::GetDevice(), m_impl->bufferMemory, 0, bufferSize, 0, &m_impl->bufferMapped);

}

void VulkanUniformBuffer::UpdateUniformData(void* data, size_t dataSize)
{
	m_impl->bufferData = data;
	memcpy(m_impl->bufferMapped, m_impl->bufferData, dataSize);
}

VkBuffer VulkanUniformBuffer::GetBufferHandle() const
{
	return m_impl->buffer;
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
	vkDestroyBuffer(VulkanDevices::GetDevice(), m_impl->buffer, nullptr);
	vkFreeMemory(VulkanDevices::GetDevice(), m_impl->bufferMemory, nullptr);
}
