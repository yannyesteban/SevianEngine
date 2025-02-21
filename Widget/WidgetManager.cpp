#include "WidgetManager.h"

namespace SEVIAN::WIDGET {

    WidgetManager::WidgetManager ( std::shared_ptr<RENDERER::RenderInterface> renderer ):renderer( renderer ) {

    }

    void WidgetManager::addWidget ( std::shared_ptr<Widget> child ) {

        widgets.emplace_back ( child );
    }

    void WidgetManager::render ( std::shared_ptr<RENDERER::RenderInterface> render, Camera2D camera ) {
        for (auto& widget : widgets) {


            UniformBufferObject ubo {};

            ubo.model = widget->getModelMatrix ();
            ubo.view = camera.getView ();
            ubo.proj = camera.getProjection ();

            render->draw ( widget->getRenderObject (), { ubo } );

            /*if (widget->isVisible)
                widget->render ( );
                */
        }
    }

}


