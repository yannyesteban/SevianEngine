#pragma once
#include "Widget.h"
namespace SEVIAN::WIDGET {
    class SpriteWidget :
        public Widget
    {
    public:
       
        //std::shared_ptr<RENDERER::IRenderizable> getRenderObject ( Camera2D camera ) override;
        void update ( float deltaTime ) override;

    };

}


