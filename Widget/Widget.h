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
			float z = 0.0f;
			float width = 1000.0f;
			float height = 1000.0f;
			float scale = 100.0f;
			Style style {};

		};
		class Widget
		{
		public:
			std::string id;

			glm::vec2 position;      // Posición relativa al padre
			glm::vec2 size;          // Ancho y alto
			bool isVisible = true;
			glm::vec2 anchor = { 0.5, 0.5 };
			//void getBounds ();
			//void getSize ();
			bool isTransformDirty = true;
			bool isClippingEnabled = false; // Recorta hijos a su área
			bool isInteractive = true;      // Si responde a eventos
			using Callback = std::function<bool ( const Event& )>;
		private: 
			std::shared_ptr<RENDERER::RenderInterface> renderer;
			
			
		public:
			std::vector<std::unique_ptr<Widget>> children;
			Widget* parent = nullptr;
			//std::vector<std::shared_ptr<Widget>> children;
			//std::weak_ptr<Widget> parent;

			virtual ~Widget () = default;
			virtual void update ( float deltaTime ) = 0;
			//virtual void render () = 0;
			virtual std::shared_ptr<RENDERER::IRenderizable> getRenderObject () = 0;
			
			template<typename T, typename... Args>
			T* create ( Args&&... args ) {
				auto widget = std::make_unique<T> ( renderer, std::forward<Args> ( args )... );
				children.push_back ( std::move( widget ) );
				return children.back ().get ();
			}

			void appendChild ( std::unique_ptr<Widget> child );
			void removeChild ( std::unique_ptr<Widget> widget ) {
				children.erase ( std::remove ( children.begin (), children.end (), widget ), children.end () );
			}
			
			void init ();

			glm::mat4 getModelMatrix ();

			// Método para verificar si un punto se encuentra dentro del widget
			virtual bool hitTest ( const glm::vec2& point ) const {
				return point.x >= position.x && point.x <= position.x + size.x &&
					point.y >= position.y && point.y <= position.y + size.y;
			}

			// Método virtual para manejar eventos, que retorna true si el evento fue consumido
			bool handleEvent ( const Event& event ) {
				return false;
			}

			// Actualización del widget (puede incluir animaciones, etc.)
			
			void attachEvent ( EventType type, Callback callback, bool useCapture = false ) {
				if (useCapture) {
					captureCallbacks[type].push_back ( callback );
				}
				else {
					bubbleCallbacks[type].push_back ( callback );
				}
			}
			
			std::map<EventType, std::vector<Callback>> captureCallbacks;
			std::map<EventType, std::vector<Callback>> bubbleCallbacks;
			
			Widget* getWidgetAt ( float px, float py );
		};
	}
}
