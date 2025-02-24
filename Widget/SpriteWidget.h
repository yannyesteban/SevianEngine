#pragma once
#include "Widget.h"
namespace SEVIAN::WIDGET {
    class SpriteWidget :
        public Widget
    {
    public:
       
        std::shared_ptr<RENDERER::IRenderizable> getRenderObject () override;
        void update ( float deltaTime ) override;

    };

}


