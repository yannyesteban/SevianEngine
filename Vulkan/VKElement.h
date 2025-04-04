#pragma once
#include "Vulkan.h"


namespace SEVIAN::VULKAN {


    

    struct MemoryData
    {
        VkBuffer buffers;
        VkDeviceMemory buffersMemory;
        void* buffersMapped;
    };
    class VKElement :
        public RENDERER::IRenderizable
    {
    private:
        
    public:
        std::vector<std::shared_ptr<IRenderizable>> childs;
        void addChild ( std::shared_ptr<IRenderizable> child ) override;
        void updateTransform ( const RENDERER::Transform& parentTransform ) override;
        virtual void render ( VkCommandBuffer commandBuffer, uint32_t swapchainImageIndex, Camera2D camera );

        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipeline pipeline2 = VK_NULL_HANDLE;
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

        std::vector<PushConstantInfo*> pushConstantsInfo;
		std::unordered_map<RENDERER::DataResource, std::vector<VulkanUBuffer>> memoryData;
        std::unordered_map<RENDERER::DataResource, void *> data;

		std::vector<FrameData> frames;

        void addData ( RENDERER::DataResource, void* data ) override;
        std::unordered_map < RENDERER::DataResource, void*> getData () override;
        void* getData ( RENDERER::DataResource ) override;
    };
}
