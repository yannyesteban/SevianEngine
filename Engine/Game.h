#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <VulkanRenderer.h>
#include "World.h"
#include "createSquareEntity.h"

namespace SEVIAN { 
	enum RenderEngine
	{
		VK,
		OPENGL,
		DIRECTX,
		METAL
	};

	struct GameInfo
	{
		RenderEngine engine;
		std::string title;
		uint32_t width;
		uint32_t height;

	};

	

	class Game
	{
	public:
		Game ( GameInfo  info ) : info ( info ) { };

		int play ();
		
		//App1 ( std::shared_ptr<RendererFactory> factory );


	private: 
		GameInfo info;
		std::shared_ptr<RenderInterface> render;
	};

}



