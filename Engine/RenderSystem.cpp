#include "renderSystem.h"

namespace SEVIAN {
    void RenderSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {

        for (auto& entity : entities) {
            auto mesh = entity->getComponent<MeshComponent> ();
            auto material = entity->getComponent<MaterialComponent> ();
            auto texture = entity->getComponent<TextureComponent> ();
            auto sprite = entity->getComponent<SpriteComponent> ();
            auto model = entity->getComponent<ModelComponent> ();


            if (model) {

                Model3D info;
                renderer->addTexture ( model->texture, model->texture );

                info.entityId = entity->getID ();
                info.model = model->path;
                info.texture = model->texture;
                auto e = renderer->createModel ( info );
                std::cout << " model " << model->path << "</n";
                units[info.entityId] = std::move ( e );
                continue;
            }

            if (mesh && material && texture && sprite) {

                Info3D info;

                renderer->addTexture ( texture->type, texture->path );

                info.entityId = entity->getID ();
                info.indices = mesh->indices;
                info.path = texture->path;
                info.texture = texture->type;
                info.vertices = mesh->vertices;
                //renderer->initEntity ( info );
                std::cout << "anneliesse morales 2024 \n" << texture->path << "\n\n";
                //auto e = renderer->createEntity ( info );
                //units[info.entityId] = std::move ( e );

                if (sprite) {
                    Sprite3D info2;

                    info2.entityId = entity->getID ();
                    info2.position = sprite->position;
                    info2.texture = sprite->texture;
                    info2.height = sprite->height;
                    info2.width = sprite->width;
                    info2.color = sprite->color;
                    auto e = renderer->createSprite ( info2 );
                    std::cout << " sprite " << sprite->color.g << "</n";
                    units[info.entityId] = std::move ( e );
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
            auto model = entity->getComponent<ModelComponent> ();


            
            auto camera = entity->getComponent<CameraComponent> ();

            if (camera) {
                Tools::setCamera ( (Camera *)camera );
            }

            if (model) {
                auto cam = *Tools::getCamera ();

                // renderer->drawEntity ( entity->getID (), position->position, cam );
                //renderer->draw ( mesh->prop, position->position, cam);
                 //auto ee = units[entity->getID ()]; // Aquí no se usa .get() si units es std::shared_ptr<PropertyRender>

                if (units[entity->getID ()]) {
                    renderer->draw ( units[entity->getID ()], position->position, cam );
                }
                continue;
            }


            if (mesh && material && texture) {
                auto cam = *Tools::getCamera ();

               // renderer->drawEntity ( entity->getID (), position->position, cam );
               //renderer->draw ( mesh->prop, position->position, cam);
                //auto ee = units[entity->getID ()]; // Aquí no se usa .get() si units es std::shared_ptr<PropertyRender>

                if (units[entity->getID ()]) {
                    renderer->draw ( units[entity->getID ()], position->position, cam );
                }
                
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