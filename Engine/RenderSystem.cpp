#include "renderSystem.h"


namespace SEVIAN {
    void RenderSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {

        for (auto& entity : entities) {
            auto mesh = entity->getComponent<MeshComponent> ();
            auto material = entity->getComponent<MaterialComponent> ();
            auto texture = entity->getComponent<TextureComponent> ();

            auto sprite = entity->getComponent<SpriteComponent> ();



            if (mesh && material && texture) {

               

                Info3D info;

                renderer->addTexture ( texture->type, texture->path );

                info.entityId = entity->getID ();
                info.indices = mesh->indices;
                info.path = texture->path;
                info.texture = texture->type;
                info.vertices = mesh->vertices;
                //renderer->initEntity ( info );
                std::cout << "anneliesse morales 2024 \n" << texture->path << "\n\n";
                auto e = renderer->createEntity ( info );
                units[info.entityId] = std::move ( e );


                if (sprite) {
                    Sprite3D info2;

                    info2.entityId = entity->getID ();
                    info2.position = sprite->position;
                    info2.texture = sprite->texture;

                    std::cout << " sprite " << sprite->color.a << "</n";
                }
            }


        }


    }
    
    void RenderSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {

        renderer->beginFrame ();
        for (auto& entity : entities) {
            auto mesh = entity->getComponent<MeshComponent> ();
            auto position = entity->getComponent<PositionComponent> ();
            auto material = entity->getComponent<MaterialComponent> ();
            auto texture = entity->getComponent<TextureComponent> ();

            auto camera = entity->getComponent<CameraComponent> ();

            if (camera) {
                Tools::setCamera ( (Camera *)camera );
            }

            if (mesh && material && texture) {
                auto cam = *Tools::getCamera ();

               // renderer->drawEntity ( entity->getID (), position->position, cam );
               //renderer->draw ( mesh->prop, position->position, cam);
                //auto ee = units[entity->getID ()]; // Aquí no se usa .get() si units es std::shared_ptr<PropertyRender>
                renderer->draw ( units[entity->getID ()], position->position, cam );
                //auto e = units[entity->getID ()].get ();
                //renderer->draw (e, position->position, cam ); <--error
                //renderer->drawText ( "YANNY", position->position, cam );

            }
        }

        renderer->endFrame ();
    }
    void StaticRenderSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {
    }
}