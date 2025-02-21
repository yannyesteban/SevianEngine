#pragma once
#include "Vulkan.h"


namespace SEVIAN::VULKAN {

    class VKElement :
        public RENDERER::IRenderizable
    {
    private:
        
    public:
        std::vector<std::shared_ptr<IRenderizable>> childs;
        void addChild ( std::shared_ptr<IRenderizable> child ) override;
        void updateTransform ( const RENDERER::Transform& parentTransform ) override;

        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;



        std::vector<VkDescriptorSet> descriptorSets;

        size_t indicesSizes;

        //VkBuffer vertexBuffer = VK_NULL_HANDLE;
        //VkBuffer indexBuffer = VK_NULL_HANDLE;
        VertexBuffer vertexBuffer;
        VertexBuffer indicesBuffer;


        VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;


        VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

        std::vector<VulkanUBuffer> ubo;
    };
}
