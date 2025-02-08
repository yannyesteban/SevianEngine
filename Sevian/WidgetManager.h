#pragma once
#include <string>
#include <glm/glm.hpp> 
#include <memory>
#include "Widget.h"
#include <vector>

namespace SEVIAN {

   
    enum class EventType
    {
        MouseMove,
        MouseDown,
        MouseUp,
        KeyPress,
        // Agrega otros tipos de eventos según sea necesario
    };

    struct Event
    {
        EventType type;
        glm::vec2 mousePos;   // Para eventos de mouse
        int keyCode;     // Para eventos de teclado
        // Otros datos del evento...
    };


	class WidgetManager
	{
	private:
        std::vector<std::shared_ptr<Widget>> widgets;

	public:
        void addWidget ( std::shared_ptr<Widget> child );

        void removeWidget ( std::shared_ptr<Widget> widget ) {
            widgets.erase ( std::remove ( widgets.begin (), widgets.end (), widget ), widgets.end () );
        }

        // Actualiza todos los widgets
        void update ( float deltaTime ) {
            for (auto& widget : widgets) {
                widget->update ( deltaTime );
            }
        }

        // Renderiza todos los widgets
        void render (/*VkCommandBuffer commandBuffer*/ ) {
            for (auto& widget : widgets) {
                if (widget->isVisible )
                    widget->render (/*commandBuffer*/ );
            }
        }

        // Propaga eventos a los widgets, típicamente en orden inverso (desde el widget de primer plano)
        bool handleEvent ( const Event& event ) {
            // Puedes recorrer en orden inverso si los últimos agregados están en primer plano
            for (auto it = widgets.rbegin (); it != widgets.rend (); ++it) {
                //if ((*it)->handleEvent ( event )) return true;
            }
            return false;
        }
	};

}