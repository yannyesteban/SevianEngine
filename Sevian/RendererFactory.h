#pragma once
#include <memory>
#include "RenderInterface.h"
namespace SEVIAN {
    class RendererFactory
    {
    public:
        virtual ~RendererFactory () = default;
        virtual std::shared_ptr<RENDERER::RenderInterface> createRenderer () = 0;
        virtual std::shared_ptr<RENDERER::TextureInterface> createTexture () = 0;
        virtual std::shared_ptr<RENDERER::ShaderInterface> createShader () = 0;
        
    };
}