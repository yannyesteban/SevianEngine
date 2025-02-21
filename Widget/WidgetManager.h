#pragma once
#include <string>
#include <glm/glm.hpp> 
#include <memory>
#include "Widget.h"
#include <vector>
#include <RenderInterface.h>

namespace SEVIAN {
    namespace WIDGET {

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
            std::shared_ptr<RENDERER::RenderInterface> renderer;

        public:
            WidgetManager ( std::shared_ptr<RENDERER::RenderInterface> renderer );
            template<typename T, typename... Args>
            std::shared_ptr<T> create ( Args&&... args ) {
                auto widget = std::make_shared<T> ( renderer, std::forward<Args> ( args )... );
                widgets.push_back ( widget );
                return widget;
            }

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
            void render ( std::shared_ptr<RENDERER::RenderInterface> render, Camera2D camera );
            /*
            bool handleEvent ( const Event& event ) {
                for (auto it = widgets.rbegin (); it != widgets.rend (); ++it) {
                    // Primero se verifica si el evento cae en el área del widget
                    if ((*it)->hitTest ( event.mousePos )) {
                        if ((*it)->handleEvent ( event )) {
                            // Si el widget consume el evento, se detiene la propagación.
                            return true;
                        }
                    }
                }
                return false;
            }
            */

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
}