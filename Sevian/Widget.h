#pragma once
#include <string>
#include <glm/glm.hpp> 

namespace SEVIAN {


	class Widget
	{
	private:


	public: 

		virtual ~Widget () = default;
		virtual void render (/*VkCommandBuffer commandBuffer*/ ) = 0;

		// Actualizaci�n del widget (puede incluir animaciones, etc.)
		virtual void update ( float deltaTime ) { }

		std::string id;
		glm::vec2 position;      // Posici�n relativa al padre
		glm::vec2 size;          // Ancho y alto
		bool isVisible = true;


	};


}


