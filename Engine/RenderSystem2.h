#pragma once
#include "System.h"
#include <RenderInterface.h>



namespace SEVIAN {
    class RenderSystem2 :
        public System


    {

        //int device;
        std::shared_ptr<RenderInterface> renderer;

        std::unordered_map<uint32_t, std::shared_ptr<Entity3D>> units;

    public:
        RenderSystem2 ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
    };


}
