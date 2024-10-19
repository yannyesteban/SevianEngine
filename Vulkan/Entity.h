#pragma once

#include "Vulkan.h"
#include "VulkanProperty.h"
//#include "RenderInterface.h"

namespace VULKAN {
    struct Entity : Entity3D
    {

        VertexBuffer vertex;
        VertexBuffer indices;

        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        std::vector<VkDescriptorSet> descriptorSets;

        size_t indicesSizes;

        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkBuffer indexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
        VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
        std::vector<VulkanUBuffer> ubo;
        std::vector<VulkanUBuffer> ubo2;
        std::vector<VulkanUBuffer> light;
        std::vector<VulkanUBuffer> me;

        void render ( ::SEVIAN::UniformBufferObject ubo );
        void ShadowRender ( ::SEVIAN::UniformBufferObject ubo );

    };
}