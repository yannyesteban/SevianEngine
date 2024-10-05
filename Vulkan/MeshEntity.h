#pragma once
#include "Device.h"
#include "TextureManager.h"
#include "RenderInterface.h"
//#include "VulkanRenderer.h"
using namespace SEVIAN;

VkVertexInputBindingDescription getBindingDescriptionGeneric ();
std::vector<VkVertexInputAttributeDescription> getAttributeDescriptionsGeneric ();

namespace VULKAN {
    class MeshEntity : public Entity3D {
    
    private:
		Device* device;
		TextureManager* textureManager;
		Info3D info;


		VertexBuffer vertex;
		VertexBuffer indices;

		VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout texDescriptorSetLayout = VK_NULL_HANDLE;

		std::vector<VkDescriptorSet> bufDescriptorSets;
        std::vector<VkDescriptorSet> texDescriptorSets;

		size_t indicesSizes;

		//VkPipeline pipeline = VK_NULL_HANDLE;
		//VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		//VkBuffer vertexBuffer = VK_NULL_HANDLE;
		//VkBuffer indexBuffer = VK_NULL_HANDLE;
		//VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
		//VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
		std::vector<VulkanUBuffer> ubo;

		//Pipeline pipeline;
		VkPipelineLayout pipelineLayout;
;		VkPipeline pipeline;



    public: 
		MeshEntity ( Device* device, TextureManager* textureManager, Info3D info, VkPipelineLayout pipelineLayout, VkPipeline pipeline, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSetLayout texDescriptorSetLayout );

        void render ( UniformBufferObject ubo ) override;

       
       
    };

    
}


