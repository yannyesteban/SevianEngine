#pragma once
#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
//#include "VulkanProperty.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include <RendererFactory.h>

namespace SEVIAN {




    class RenderSystem : public System
    {
        //int device;
        std::shared_ptr<RenderInterface> renderer;

        std::unordered_map<uint32_t, std::shared_ptr<PropertyRender>> units;

    public:
        RenderSystem ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entities ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities ) override;

       

    };


    class StaticRenderSystem : public System
    {
        //int device;
        std::shared_ptr<RenderInterface> renderer;

    public:
        StaticRenderSystem ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }

        void update ( std::vector<std::shared_ptr<Entity>>& entities ) override;

    };
}