#pragma once
#include "RenderInterface.h"
#include "Device.h"

using namespace SEVIAN;
namespace SEVIAN {
	namespace VULKAN {
		class VulkanProperty : public RENDERER::Entity3D
		{
		public:

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
			std::vector<VulkanUBuffer> buffers;

			void render ( UniformBufferObject ubo ) override;
			void ShadowRender ( UniformBufferObject ubo ) override;



		};


		class Element : public RENDERER::Entity3D
		{
		public:
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
			std::vector<VulkanUBuffer> buffers;

			// No es necesario volver a declarar los m�todos render y ShadowRender 
			// ya que se heredan de la clase base Entity3D
		};
		struct Bounds
		{
			double l, b, r, t;
		};

		/*
		class Glyph : public Element
		{
		public:
			double l, b, r, t;
			float u0, v0, u1, v1, w, h, offsetX, offsetY;
			int            width;
			int            height;
			int            bearingX;
			int            bearingY;
			float           advance;
			int charIndex;
			char ch;
			Bounds bounds;
			void* bitmap;

			VkImage        textureImage = VK_NULL_HANDLE;
			VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
			VkImageView    textureImageView = VK_NULL_HANDLE;
			VkSampler      textureSampler = VK_NULL_HANDLE;

			void render ( UniformBufferObject ubo ) override;
			void ShadowRender ( UniformBufferObject ubo ) override;


		};
		*/

	}



}