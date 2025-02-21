#include "InitializationSystem.h"
#include "Models.h"
#include "VulkanProperty.h"
namespace SEVIAN {
    void InitializationSystem::init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {
    }
    void InitializationSystem::update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {
       


        for (auto& entity : entities) {
            auto mesh = entity->getComponent<MeshComponent> ();
            auto material = entity->getComponent<MaterialComponent> ();
            auto texture = entity->getComponent<TextureComponent> ();

           
        }
    }
}
