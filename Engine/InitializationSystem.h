#pragma once
#include "System.h"
#include "Scene.h"
#include "Entity.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include "RendererFactory.h"

namespace SEVIAN {

    class InitializationSystem : public System
    {
        std::shared_ptr<RENDERER::RenderInterface> renderer;

    public:
        InitializationSystem ( std::shared_ptr<RENDERER::RenderInterface>  r ) : renderer ( r ) { }
       
        void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;

    };
}