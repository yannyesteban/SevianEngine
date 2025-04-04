#pragma once
#include <string>
#include <memory>
#include <glm/glm.hpp> 
#include "RenderInterface.h"
#include "TextEngine.h"
#include "EventTarget.h"
//#include "Text.h"
#include "Event.h"
#include "Style.h"
#include "Layout.h"

#include "StyleComponet.h"

//class StyleSystem
//{
//public:
//	void update ( Widget* root ) {
//		// Recorrer árbol de widgets y aplicar:
//		// 1. Herencia de estilos
//		// 2. Resolución de valores calculados
//		// 3. Aplicar estilos al renderer
//	}
//};
namespace SEVIAN {
	namespace WIDGET {
		
		struct Dim
		{

			float top = 0.0f;
			float right = 0.0f;
			float bottom = 0.0f;
			float left = 0.0f;
			
			
		};
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

		
		class Widget : public EventTarget
		{
		protected:
			std::shared_ptr<RENDERER::IRenderizable> object;

		public:
			std::string id;

			glm::vec2 position;
			glm::vec2 size;
			Dim border;
			Dim padding;
			Dim margin;

			bool isVisible = true;
			glm::vec2 anchor = { 0.5, 0.5 };
			//void getBounds ();
			//void getSize ();
			bool layoutDirty = true;
			bool isTransformDirty = true;
			bool isClippingEnabled = false; // Recorta hijos a su área
			
			using Callback = std::function<bool ( const Event& )>;
			bool draggable = false;
			std::vector<std::unique_ptr<Widget>> children;
			Widget* parent = nullptr;
			bool canFocus = true;  // Si el widget puede recibir foco
			bool focused = false;     // Si actualmente tiene el foco
			bool isInteractive = true;      // Si responde a eventos
			bool positioned = false;
			ZIndex zIndex;
		private: 
			std::shared_ptr<RENDERER::RenderInterface> renderer;
			
			//std::vector<std::unique_ptr<StyleComponent>> styleComponents;
			std::unordered_map<std::type_index, std::unique_ptr<StyleComponent>> styleComponents;
			
		public:
			virtual ~Widget () = default;
			// Actualización del widget (puede incluir animaciones, etc.)
			virtual void update ( float deltaTime );
			virtual std::shared_ptr<RENDERER::IRenderizable> getRenderObject ( Camera2D camera );
			// Update the layout (can be called explicitly)
			std::unique_ptr<Layout> layout;
			void updateLayout (int borrar);
			
			//virtual void render () = 0;
			
			virtual void setFocusable ( bool value ) { canFocus = value; }

			template<typename T, typename... Args>
			T* create ( Args&&... args ) {
				auto widget = std::make_unique<T> ( renderer, std::forward<Args> ( args )... );
				children.push_back ( std::move( widget ) );
				return children.back ().get ();
			}

			template<typename T, typename... Args>
			T* addStyle ( Args&&... args ) {

				removeStyle<T> ();

				// Crear nuevo componente
				auto component = std::make_unique<T> ( std::forward<Args> ( args )... );
				T* ptr = component.get ();
				//styleComponents.push_back ( std::move ( component ) );
				styleComponents[std::type_index ( typeid(T) )] = std::move ( component );
				// Inicializar componente
				//ptr->init ( this );
				return ptr;
			}

			template<typename T>
			T* getStyle () const {
				auto it = styleComponents.find ( std::type_index ( typeid(T) ) );
				if (it != styleComponents.end ()) {
					return static_cast<T*>(it->second.get ());
				}
				return nullptr;
			}
			template<typename T>
			T* getStyleV2 () {
				for (auto& component : styleComponents) {
					if (auto ptr = dynamic_cast<T*>(component.get ())) {
						return ptr;
					}
				}
				return nullptr;
			}
			template<typename T>
			void removeStyle () {
				auto it = styleComponents.find ( std::type_index ( typeid(T) ) );
				if (it != styleComponents.end ()) {
					styleComponents.erase ( it );
				}
			}
			template<typename T>
			void removeStyleV2 () {
				auto it = std::remove_if ( styleComponents.begin (), styleComponents.end (),
					[]( auto& component ) { return dynamic_cast<T*>(component.get ()) != nullptr; } );

				styleComponents.erase ( it, styleComponents.end () );
			}

			void appendChild ( std::unique_ptr<Widget> child );
			void removeChild ( Widget* widget ) {
				children.erase ( std::remove_if ( children.begin (), children.end (),
					[&]( const std::unique_ptr<Widget>& w ) { return w.get () == widget; } ), children.end () );

				/* auto it = std::find_if(children.begin(), children.end(), 
                    [widget](const std::unique_ptr<Widget>& child) {
                        return child.get() == widget;
                    });
                if (it != children.end()) {
                    children.erase(it);
                    isTransformDirty = true;
                }*/
			}
			/*void removeChild ( std::unique_ptr<Widget> widget ) {
				children.erase ( std::remove ( children.begin (), children.end (), widget ), children.end () );
			}*/
			
			void init ();

			virtual glm::mat4 getModelMatrix ();

			// Método para verificar si un punto se encuentra dentro del widget
			virtual bool hitTest ( const glm::vec2& point ) const {
				return point.x >= position.x && point.x <= position.x + size.x &&
					point.y >= position.y && point.y <= position.y + size.y;
			}

			// Método virtual para manejar eventos, que retorna true si el evento fue consumido
			bool handleEvent ( const Event& event ) {
				return false;
			}

			
			Widget* getWidgetAt ( float px, float py );

			bool dispatchEvent ( Event& event ) override;
			bool processListeners ( Event& event, bool capture );


			void setLayout ( std::unique_ptr<Layout> newLayout ) {
				layout = std::move ( newLayout );
				//layout->setParent ( this );
				markLayoutDirty ();
			}

			void markLayoutDirty () {
				layoutDirty = true;
				if (parent) {
					parent->markLayoutDirty ();
				}
			}

			void updateLayout () {
				if (layout && layoutDirty) {
					//layout->arrangeChildren ();
					layoutDirty = false;
				}
			}

			void setPosition ( float x, float y ) {
				position.x = x;
				position.y = y;
				
			}

			void setX ( float x ) {
				position.x = x;
			}

			float getX () const {
				return position.x;
			}


			void setY ( float y ) {
				position.y = y;
			}

			float getY () const {
				return position.y;
			}

			glm::vec2 getPosition () const {
				return position;
			}




			void setSize ( float w, float h ) {
				size.x = w;
				size.y = h;

				
			}

			void setWith ( float w ) {
				size.x = w;
			}
			void setHeight ( float h ) {
				size.y = h;
			}

			glm::vec2 getSize () const {
				return size;
			}

			float getWith () {
				return size.x;
			}
			float getheight () {
				return size.y;
			}

			void setMargin ( Dim _margin ) {
				margin = _margin;
			}
			Dim getMargin () {
				return margin;
			}

			void setBorder ( Dim _border ) {
				border = _border;
			}
			Dim getBorder () {
				return border;
			}

			void setPadding ( Dim _padding ) {
				padding = _padding;
			}
			Dim getPadding () {
				return padding;
			}

			// Set the layout for this widget
			/*void setLayout (std::unique_ptr<Layout> layoutManager) {
				layout = std::move ( layoutManager );
			}*/
			// Método actualizado para calcular layout
			//void updateLayout () {
			//	if (auto* layout = getStyle<LayoutComponent> ()) {
			//		// Cálculos de layout basados en propiedades
			//		if (layout->display == Display::Flex) {
			//			//calculateFlexLayout ();
			//		}
			//	}

			//	for (auto& child : children) {
			//		child->updateLayout ();
			//	}
			//}
		};
	}
}
