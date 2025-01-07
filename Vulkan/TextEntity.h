#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H


#include <msdfgen.h>
#include <msdfgen-ext.h>


#include "Device.h"
#include "TextureManager.h"
#include <RenderInterface.h>
#include "VulkanProperty.h"

//#include "VulkanRenderer.h"
using namespace SEVIAN;
namespace VULKAN {

	enum FontRenderType
	{
		SDF,
		PSDF,
		MSDF,
		MTSDF
	};

	class TextEntity :
		public Entity3D
	{
		
		std::string text;
		std::string font;
		
		Device* device;
		std::vector<Frame> frames;
		VkDescriptorPool descriptorPool;

		Glyph character;
		std::shared_ptr<Entity3D> prop;
		bool play = false;
		TextureManager* textureManager;
		Info3D info;

		Propertys propertys;

		VertexBuffer vertex;
		VertexBuffer indices;
		size_t indicesSizes;

		VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout texDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout shadowDescriptorSetLayout = VK_NULL_HANDLE;

		std::vector<VkDescriptorSet> bufDescriptorSets;
		std::vector<VkDescriptorSet> texDescriptorSets;
		std::vector<VkDescriptorSet> shadowDescriptorSets;

		std::vector<VulkanUBuffer> ubo;
		std::vector<VulkanUBuffer> ubo1;
		std::vector<std::shared_ptr<Entity3D>> tx;

		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

		VkPipelineLayout shadowPipelineLayout = VK_NULL_HANDLE;
		VkPipeline shadowPipeline = VK_NULL_HANDLE;

		FT_Library  library;
		FT_Face     face;
		std::map<char, Glyph> Characters;

		void createTextureImage2 ( const std::string& fontPath, char c, Glyph& character );

		void createTextureImage ( const std::string& fontPath, char c, Glyph& character );

		std::shared_ptr<Entity3D> init ( std::vector<VertexText> vertices, std::vector<uint32_t> indices, Glyph texture );
		std::shared_ptr<Entity3D> init2 ( std::vector<VertexText> vertices, std::vector<uint32_t> indices );

		void createImageView ( VkImage image, VkFormat format, VkImageView& imageView );

		void createTextureSampler ( VkSampler& sampler );

		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkVertexInputBindingDescription getBindingDescription2 ();
		std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions2 ();
		//VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		//VkPipeline pipeline = VK_NULL_HANDLE;
		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );

		void initGlyph ( FT_Face face, char c, Glyph& character );

		void createTextureImage ( Glyph& character );

	public:
		void fontInit ( std::string fontPath );
		void fontInit2 ( std::string fontPath );
		TextEntity (
			FontRenderType type,
			std::string text,
			std::string font,
			Device* device,
			std::vector<Frame> frames,
			VkDescriptorPool descriptorPool );
		TextEntity (
			
			std::string text,
			std::string font,
			Device* device,
			std::vector<Frame> frames,
			VkDescriptorPool descriptorPool);
		void render ( UniformBufferObject ubo ) override;
		void render2 ( UniformBufferObject ubo );
		void ShadowRender ( UniformBufferObject ubo ) override;

		void draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position, Camera camera, uint32_t width, uint32_t height );
		void draw2 ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position, Camera camera, uint32_t width, uint32_t height );
	};

}