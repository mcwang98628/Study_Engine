#include "stdafx.h"
#include "GameObject.h"
#include "Render/Vulkan/VulkanControl.h"
#include "Render/Renderer.h"
#include "Render/Vulkan/VulkanDevices.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace StudyEngine {
    GameObject::GameObject()
    {
    }

    GameObject::~GameObject()
    {
        VkDevice device = VulkanDevices::GetDevice();
        delete m_pipeline;

        delete m_vertexBuffer;
        delete m_indexBuffer;
    }

    void GameObject::Update(float deltaTime)
    {

    }

    void GameObject::Draw()
    {
        VkCommandBuffer cmd = VulkanControl::Get()->GetCommandBuffer();
        uint32_t frameInd = VulkanControl::Get()->GetCurrentFrameIndex();
        VkDeviceSize offsets = 0;
        VulkanDescriptorSet* descriptorSet = Renderer::GetCurrentDescriptorSet();

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanControl::Get()->getPipelineLayout(), 0, 1, descriptorSet->GetDescriptorSetHandle().data(), 0, nullptr);
        m_pipeline->BindCommandBuffer(cmd);
        m_vertexBuffer->BindBuffer();
        m_indexBuffer->BindBuffer();
        vkCmdDrawIndexed(cmd, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
    }


    void GameObject::LoadModel(std::string modelPath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = Vector3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                );

                vertex.texCoord = Vector2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                );

                vertex.normal = Vector3(1.0f, 1.0f, 1.0f);
                m_vertices.push_back(vertex);
                m_indices.push_back(static_cast<uint32_t>(m_vertices.size()) - 1);

            }
        }
        m_vertexBuffer = new VulkanBuffer(m_vertices.data(), sizeof(m_vertices[0]) * m_vertices.size(), VulkanBuffer::VERTEX);
        m_indexBuffer = new VulkanBuffer(m_indices.data(), sizeof(m_indices[0]) * m_indices.size(), VulkanBuffer::INDEX);

    }

    void GameObject::UpdateDescriptorSets()
    {

    }

    void GameObject::BindGraphicPipeline(std::string vertShaderPath, std::string fragShaderPath)
    {
        m_pipeline = new VulkanPipeline(vertShaderPath, fragShaderPath);
    }

    void GameObject::CreateUniformBuffers()
    {
    }
    void GameObject::DrawIndexed()
    {

    }
}
