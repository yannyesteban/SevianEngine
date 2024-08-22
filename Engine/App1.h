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
	

	class App1: public GameApp
	{
	private:
		World world;

		std::shared_ptr<RenderInterface> renderer;
		std::shared_ptr<TextureInterface> texture;
		std::shared_ptr<ShaderInterface> shader;

		
	public: 
		App1 ( std::shared_ptr<RendererFactory> factory );

		void onKeyPress ( int key );
		void onKeyRelease ( int key );
		void run ();
	
	};
}


