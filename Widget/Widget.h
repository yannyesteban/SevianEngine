#pragma once
#include <string>
#include <glm/glm.hpp> 
#include "RenderInterface.h"
#include "TextEngine.h"
//#include "Text.h"
#include "Event.h"
#include "Style.h"
#include <memory>


namespace SEVIAN {
	namespace WIDGET {

		struct TextInfo
		{
			
			std::string fontName = "";
			std::string text = "";
			float x = 0.0f;
			float y = 0.0f;
			float width = 1000.0f;
			float height = 1000.0f;
			float scale = 100.0f;
			Style style {};

		};
		class Widget
		{
		public:

			virtual ~Widget () = default;
			virtual void render ( ) = 0;
			virtual std::shared_ptr<RENDERER::IRenderizable> getRenderObject () = 0;
			// Actualización del widget (puede incluir animaciones, etc.)
			virtual void update ( float deltaTime ) = 0;
			
			std::string id;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float width = 0.0f;
			float height = 0.0f;
			glm::vec2 position;      // Posición relativa al padre
			glm::vec2 size;          // Ancho y alto
			bool isVisible = true;

			//void getBounds ();
			//void getSize ();

			void init ();

			glm::mat4 getModelMatrix ();

			// Método para verificar si un punto se encuentra dentro del widget
			bool hitTest ( const glm::vec2& point ) const {
				return point.x >= position.x && point.x <= position.x + size.x &&
					point.y >= position.y && point.y <= position.y + size.y;
			}

			// Método virtual para manejar eventos, que retorna true si el evento fue consumido
			bool handleEvent ( const X::Event& event ) {
				return false;
			}

			void attachEvent ( std::string, std::function<void ( const X::Event& )> callback ) { };

			
		};
	}
}
