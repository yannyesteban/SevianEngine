#include "TextureManager.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace VULKAN {
	std::unique_ptr<VulkanTexture> TextureManager::createTexture ( const char* src ) {
		auto texture = std::make_unique<VulkanTexture> ();

		//int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load ( src, &texture->texWidth, &texture->texHeight, &texture->texChannels, STBI_rgb_alpha );
		VkDeviceSize imageSize = texture->texWidth * texture->texHeight * 4;

		std::cout << "height " << texture->texWidth << " height " << texture->texHeight << "\n";

		if (!pixels) {
			throw std::runtime_error ( "failed to load texture image!" );
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		device->createBuffer ( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

		void* data;
		vkMapMemory ( device->device, stagingBufferMemory, 0, imageSize, 0, &data );
		memcpy ( data, pixels, static_cast<size_t>(imageSize) );
		vkUnmapMemory ( device->device, stagingBufferMemory );

		stbi_image_free ( pixels );

		device->createImage ( texture->texWidth, texture->texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture->textureImage, texture->textureImageMemory );

		device->transitionImageLayout ( texture->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
		device->copyBufferToImage ( stagingBuffer, texture->textureImage, static_cast<uint32_t>(texture->texWidth), static_cast<uint32_t>(texture->texHeight) );
		device->transitionImageLayout ( texture->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );
		texture->imageView = device->createImageView ( texture->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
		texture->sampler = device->createTextureSampler ();


		return texture;
	}

	void TextureManager::add ( std::string name, std::string path ) {
		if (mTextures.find ( name ) != mTextures.end ()) {

			std::cerr << "Warning: Texture with name '" << name << "' already exists. Skipping assignment." << std::endl;
			return;
		}

		mTextures[name] = createTexture ( path.c_str () );
	}

	void TextureManager::add ( ::SEVIAN::TextureInfo info ) {
		if (mTextures.find ( info.name ) != mTextures.end ()) {

			std::cerr << "Warning: Texture with name '" << info.name << "' already exists. Skipping assignment." << std::endl;
			return;
		}

		mTextures[info.name] = createTexture ( info.path.c_str () );

	}

	std::shared_ptr<VulkanTexture> TextureManager::get ( std::string name ) {
		auto texture = mTextures[name];

		return texture;

	}
}

