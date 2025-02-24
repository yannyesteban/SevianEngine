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

    }
}


