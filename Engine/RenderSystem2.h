#pragma once
#include "Game.h"
#include "System.h"
#include <RenderInterface.h>



namespace SEVIAN {
    class RenderSystem2 :
        public System


    {

        //int device;
        std::shared_ptr<RENDERER::RenderInterface> renderer;
        GameInfo info;
        std::unordered_map<uint32_t, std::shared_ptr<RENDERER::Entity3D>> units;

    public:
        RenderSystem2 ( std::shared_ptr<RENDERER::RenderInterface>  r ) : renderer ( r ) { }
        RenderSystem2 ( GameInfo info) : info ( info ), renderer (info.render) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
    };


}
