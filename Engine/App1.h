#pragma once
#include <memory>
#include <VulkanFactory.h>
#include "World.h"
//#include "createSquareEntity.h"
#include "InitializationSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "MovementSystem.h"
namespace SEVIAN {
	

	class App1: public RENDERER::GameApp
	{
	private:
		World world;

		std::shared_ptr<RENDERER::RenderInterface> renderer;
		std::shared_ptr<RENDERER::TextureInterface> texture;
		std::shared_ptr<RENDERER::ShaderInterface> shader;

		
	public: 
		App1 ( std::shared_ptr<RendererFactory> factory );

		void onKeyPress ( int key );
		void onKeyRelease ( int key );
		void run ();
	
	};
}


