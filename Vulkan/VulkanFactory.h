#pragma once

#include "RendererFactory.h"
#include "Renderer.h"

using namespace SEVIAN;
namespace VULKAN {
    class VulkanFactory : public RendererFactory
    {
    public:
        std::shared_ptr<RenderInterface> createRenderer () override {
            return std::make_unique<VulkanRenderer> ();
        }

        std::shared_ptr<TextureInterface> createTexture () override {
            return std::make_unique<VulkanTextureNO> ();
        }

        std::shared_ptr<ShaderInterface> createShader () override {
            return std::make_unique<VulkanShader> ();
        }
    };
}
