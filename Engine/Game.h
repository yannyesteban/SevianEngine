#pragma once
#include <cstdint>
#include <string>
#include <memory>

#include "Renderer.h"
#include "World.h"
#include "createSquareEntity.h"
#include "SpriteManager.h"
#include "WidgetManager.h"

namespace SEVIAN {

	struct GameInfo
	{
		std::shared_ptr<RENDERER::RenderInterface> render;
		std::shared_ptr<RENDERER::iSpriteManager> spriteManager;
		std::shared_ptr<WIDGET::WidgetManager> widgetManager;
	};

	enum RenderEngine
	{
		VK,
		OPENGL,
		DIRECTX,
		METAL
	};

	struct PreGameInfo
	{
		RenderEngine engine;
		std::string title;
		uint32_t width;
		uint32_t height;

	};



	class Game
	{
	public:
		Game ( PreGameInfo  info ) : info ( info ) { };

		int play ();

		//App1 ( std::shared_ptr<RendererFactory> factory );


	private:
		PreGameInfo info;
		std::shared_ptr<RENDERER::RenderInterface> render;
		//std::shared_ptr<RENDERER::iSpriteManager> spriteManager;
		//std::shared_ptr<RENDERER::iTextManager> renderText;

	};

}



