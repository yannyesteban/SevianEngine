#pragma once

#include "Vulkan.h"
#include "Renderer.h"
#include "TextEngine.h"
#include "Device.h"
#include "TextureManager.h"
#include "TextEntity.h"
//#include "RenderInterface.h"
#include "VulkanRendererManager.h"
namespace SEVIAN::VULKAN {
    class TextManager : public RENDERER::iTextManager, public VulkanRendererManager
    {
    private:
		VulkanInfo vulkan;
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;


		VulkanTexture texture;

		std::string text;
		std::string font;

		Device* device;
		std::vector<Frame> frames;
		VkDescriptorPool descriptorPool;

		//Glyph character;
		//Glyph character2;
		std::shared_ptr<RENDERER::Entity3D> prop;
		bool play = false;
		TextureManager* textureManager;
		Info3D info;

		//Propertys propertys;

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
		std::vector<std::shared_ptr<RENDERER::Entity3D>> tx;

		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

		VkPipelineLayout shadowPipelineLayout = VK_NULL_HANDLE;
		VkPipeline shadowPipeline = VK_NULL_HANDLE;


		std::map<char, AtlasGlyphInfo> Characters;

		//std::map<std::string, Font> fonts;
		//std::map<std::string, VulkanTexture > fontTextures;
		

		//std::optional<Font> defaultFont;
		//std::optional<VulkanTexture> defaultTexture;
		
		VulkanTexture defaultTexture;

		struct FontData
		{
			Font font;
			VulkanTexture texture;
		} ;

		std::map<std::string, FontData> fonts;
		

		std::string defaultFont;

		VulkanTexture getTexture ( std::string fontName );

	public:
		
		void addFont ( Font font, bool setDefault ) override;
		Font getFont ( std::string name ) override;
		

		std::unique_ptr<RENDERER::IRenderizable> createText ( float x, float y, std::vector<Quad> quads, std::string textureName ) override;
		std::unique_ptr<RENDERER::IRenderizable> createText ( TEXT::Box box ) override;
		

		VulkanTexture createAtlasTexture ( std::vector<uint8_t> msdfData, int atlasWidth, int atlasHeight );

		void createImageView ( VkImage image, VkFormat format, VkImageView& imageView );

		void createTextureSampler ( VkSampler& sampler );

		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );
    
        TextManager ( VulkanInfo vulkan);

		TextManager ( std::shared_ptr<VulkanRenderer> renderer );

    };

}

