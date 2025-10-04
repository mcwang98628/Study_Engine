#include "stdafx.h"
#include "Renderer.h"
#include "Vulkan/vulkanControl.h"
#include "Vulkan/VulkanDescriptorSet.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/VulkanDebugMessenger.h"
#include "Vulkan/VulkanDevices.h"
#include "Vulkan/VulkanCore.h"
#include "Vulkan/VulkanSwapChain.h"


namespace StudyEngine {
    VulkanDescriptorSet* Renderer::m_descriptorSets = nullptr;
    Renderer::Renderer()
    {
    }
    Renderer::~Renderer()
    {
    }
    bool Renderer::Init()
	{
        const std::string TEXTURE_PATH = "textures/viking_room.png";

        VulkanControl* vulkanController = VulkanControl::Get();
        VulkanCore::Init();
        //VulkanInstance::Init();
        //VulkanDebugMessenger::Init();
        //vulkanController->createSurface(WinApplication::GetWindow());

        VulkanDevices::Init();
        //vulkanController->pickPhysicalDevice();
        //vulkanController->createLogicalDevice();
        VulkanSwapChain::Init();
        //vulkanController->createSwapChain();
        VulkanSwapChain::CreateImageViews();
        //vulkanController->createImageViews();
        vulkanController->createRenderPass();
        vulkanController->createDescriptorSetLayout();
        vulkanController->createPipelineLayout();

        vulkanController->createCommandPool();
        VulkanSwapChain::CreateDepthResources();
        VulkanSwapChain::CreateFramebuffers();

        vulkanController->createTextureImage(TEXTURE_PATH);
        vulkanController->createTextureImageView();
        vulkanController->createTextureSampler();

        vulkanController->createDescriptorPool();

        m_descriptorSets = new VulkanDescriptorSet();

        vulkanController->createCommandBuffers();
        vulkanController->createSyncObjects();

        return true;
	}

    void Renderer::BeginRender()
    {
        VulkanControl::Get()->BeginRenderer();
    }

    void Renderer::EndRender()
    {
        VulkanControl::Get()->EndRenderer();
    }

    void Renderer::PreShutDown()
    {
        VulkanControl::Get()->WaitIdle();
    }

    bool Renderer::ShutDown()
	{
        VulkanControl::Get()->cleanUp();
		return false;
	}

    VulkanDescriptorSet* Renderer::GetCurrentDescriptorSet()
    {
        return m_descriptorSets;
    }


};


