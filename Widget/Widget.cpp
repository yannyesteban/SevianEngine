#include "Widget.h"


namespace SEVIAN {
    namespace WIDGET {
        void Widget::init () {
        }
        glm::mat4 Widget::getModelMatrix () {


            auto translation = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( position, 1.0f ) );


            glm::mat4 rotationMatrix = glm::mat4 ( 1.0f );


            auto scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );



            auto modelMatrix = translation * rotationMatrix * scaleMatrix;
            return modelMatrix;
        }

        Widget* Widget::getWidgetAt ( float px, float py ) {
            for (auto it = children.rbegin (); it != children.rend (); ++it) {
                if ((*it)->hitTest ( glm::vec2( px, py ) )) {
                    auto target = (*it)->getWidgetAt ( px, py );
                    if (target) return target;
                }
            }
            if (hitTest ( glm::vec2 ( px, py ) )) return this;
            return nullptr;
        }

        

        void Widget::update ( float deltaTime ) {
        }

       
        std::shared_ptr<RENDERER::IRenderizable> Widget::getRenderObject () {
            return std::shared_ptr<RENDERER::IRenderizable> ();
        }

        void Widget::appendChild ( std::unique_ptr<Widget> child ) {
            child->parent = this; // Establece el puntero al padre
            children.push_back ( std::move ( child ) ); // Transfiere ownership
        }

        bool Widget::dispatchEvent ( Event& event ) {
            event.target = this;  // El widget actual es el objetivo del evento

            // Buscar todos los ancestros (para la fase de captura)
            std::vector<Widget*> path;
            Widget* current = this;
            while (current) {
                path.push_back ( current );
                current = current->parent;
            }
            std::reverse ( path.begin (), path.end () );

            // Fase de captura (de la raíz al objetivo)
            event.phase = EventPhase::CAPTURING_PHASE;
            for (size_t i = 0; i < path.size () - 1; ++i) {
                event.currentTarget = path[i];
                if (!path[i]->processListeners ( event, true )) {
                    return false;  // Evento detenido
                }
            }

            // Fase en el objetivo
            event.phase = EventPhase::AT_TARGET;
            event.currentTarget = this;
            if (!processListeners ( event, true ) || !processListeners ( event, false )) {
                return false;  // Evento detenido
            }

            // Fase de burbujeo (del objetivo a la raíz)
            if (event.bubbles) {
                event.phase = EventPhase::BUBBLING_PHASE;
                for (int i = static_cast<int>(path.size ()) - 2; i >= 0; --i) {
                
                    
                    // ojo aqui
                    
                    
                    
                    
                    //for (size_t i = path.size () - 2; i < path.size (); --i) {
                    event.currentTarget = path[i];
                    if (!path[i]->processListeners ( event, false )) {
                        return false;  // Evento detenido
                    }
                }
            }

            return true;  // Evento procesado completamente
        }
        bool Widget::processListeners ( Event& event, bool capture ) {
            auto it = listeners.find ( event.type );
            if (it != listeners.end ()) {
                auto listenersCopy = it->second;  // Crear copia para permitir modificaciones durante iteración
                for (const auto& info : listenersCopy) {
                    if (info.useCapture == capture) {
                        bool result = info.listener->handleEvent ( event );

                        // Si el listener debe ejecutarse solo una vez, eliminarlo
                        if (info.once) {
                            removeEventListener ( event.type, info.listener, info.useCapture );
                        }

                        // Si el listener detuvo la propagación
                        if (event.stopPropagation || event.stopImmediatePropagation) {
                            return false;
                        }

                        // Si el listener devolvió false, considera que el evento fue manejado
                        if (!result) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    }
}


