#pragma once
#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
//#include "VulkanProperty.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include "TransformSystem.h"
#include <RendererFactory.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace SEVIAN {




    class RenderSystem : public System
    {
        //int device;
        std::shared_ptr<RENDERER::RenderInterface> renderer;

        std::unordered_map<uint32_t, std::shared_ptr<RENDERER::Entity3D>> units;

    public:
        RenderSystem ( std::shared_ptr<RENDERER::RenderInterface>  r ) : renderer ( r ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;

       

    };

    /*
    class StaticRenderSystem : public System
    {
        //int device;
        std::shared_ptr<RenderInterface> renderer;

    public:
        StaticRenderSystem ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }

        void init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;

    };
    */
}