#pragma once
#include <memory>
#include "RenderInterface.h"
namespace SEVIAN {
    class RendererFactory
    {
    public:
        virtual ~RendererFactory () = default;
        virtual std::shared_ptr<RenderInterface> createRenderer () = 0;
        virtual std::shared_ptr<TextureInterface> createTexture () = 0;
        virtual std::shared_ptr<ShaderInterface> createShader () = 0;
        
    };
}