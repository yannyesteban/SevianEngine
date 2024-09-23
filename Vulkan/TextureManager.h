#pragma once
#include "Vulkan.h"
#include "Device.h"
#include "Vertex.h"
#include <string>
#include <map>

namespace SEVIAN {
	class TextureManager
	{
	private:
		Device* device;
		std::map<std::string, std::shared_ptr<VulkanTexture>> mTextures;
		std::unique_ptr<VulkanTexture> createTexture ( const char* src );
	public:
		TextureManager( Device* device ): device(device){ }
		void add ( std::string name, std::string path );
		void add ( TextureInfo info );
		std::shared_ptr<VulkanTexture> get ( std::string name );
	};
}


