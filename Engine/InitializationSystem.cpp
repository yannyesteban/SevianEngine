#include "InitializationSystem.h"
#include "Models.h"
#include "VulkanProperty.h"
namespace SEVIAN {
    void InitializationSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {
    }
    void InitializationSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {
       


        for (auto& entity : entities) {
            auto mesh = entity->getComponent<MeshComponent> ();
            auto material = entity->getComponent<MaterialComponent> ();
            auto texture = entity->getComponent<TextureComponent> ();

            if (mesh && material && texture) {
                renderer->addTexture ( texture->name, texture->path );
                //mesh->prop = new VulkanProperty ();
                
                auto vulkanProp = renderer->init ( mesh->vertices, mesh->indices, texture->name );
                mesh->prop = vulkanProp;
                    
                //auto x = renderer->init ( mesh->vertices, mesh->indices, texture->type );
                //mesh->prop = &x;
                std::cout << "anneliesse morales \n" << texture->path << "\n\n";
            }
        }
    }
}
