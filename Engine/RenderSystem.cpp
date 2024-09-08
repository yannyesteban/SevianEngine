#include "renderSystem.h"

namespace SEVIAN {
	void RenderSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {

		for (auto& entity : entities) {
			auto mesh = entity->getComponent<MeshComponent> ();
			auto material = entity->getComponent<MaterialComponent> ();
			auto texture = entity->getComponent<TextureComponent> ();
			auto sprite = entity->getComponent<SpriteComponent> ();
			auto model = entity->getComponent<ModelComponent> ();

			std::string t = "";


			if (texture) {
				t = texture->type;
			}



			if (mesh && t == "diffuse1" && texture) {
				Info3D info;
				
				renderer->addTexture ( texture->type, texture->path );

				info.entityId = entity->getID ();
				info.vertices = mesh->vertices;
				info.indices = mesh->indices;
				
				info.texture = texture->type;
				info.path = texture->path;
				auto e = renderer->createEntity ( info );
				std::cout << " model " << texture->path << "</n";
				units[info.entityId] = std::move ( e );
				continue;
			}
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
			auto rotation = entity->getComponent<RotationComponent> ();
			auto scale = entity->getComponent<ScaleComponent> ();


			auto material = entity->getComponent<MaterialComponent> ();
			auto texture = entity->getComponent<TextureComponent> ();
			auto model = entity->getComponent<ModelComponent> ();
			

			auto camera = entity->getComponent<CameraComponent> ();

			

			if (camera) {
				std::cout << "... PRE SET " << camera->position.z << "\n";
				Tools::setCamera ( camera );

			}
			if (!units[entity->getID ()]) {
				continue;
			}
			if (!model && !(mesh && material && texture)) {
				continue;
			}

			auto cam = *Tools::getCamera ();
			UniformBufferObject ubo {};

			std::cout << "ROTATION: " << rotation->rotation.x << ":" << rotation->rotation.y << ":" << rotation->rotation.z << "\n";
			glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position->position );
			//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

			glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
				glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
				glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
			ubo.model = translation * rotationMat;


			//ubo.model = /* rotation * */  translation;


			// Posición de la cámara arriba en el eje Z
			glm::vec3 cameraPos = cam.position;//glm::vec3 ( 0.0f, 0.0f, -5.0f );

			//glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 5.0f );

			// Punto al que está mirando la cámara (el origen en este caso)
			glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
			// Dirección "up" (hacia el eje Y)
			//glm::vec3 up = camera.up; // glm::vec3 ( 0.0f, 1.0f, 0.0f );
			glm::vec3 up = glm::vec3 ( 0.0f, 10.0f, 0.0f );

			ubo.view = glm::lookAt ( cameraPos, target, up );

			//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
			ubo.proj = glm::perspective ( glm::radians ( 45.0f ), 1300 / (float) 600, 0.1f, 100.0f );



			if (model) {
				
				//std::cout << "..." << cam.position.z << "\n";
				// renderer->drawEntity ( entity->getID (), position->position, cam );
				//renderer->draw ( mesh->prop, position->position, cam);
				 //auto ee = units[entity->getID ()]; // Aquí no se usa .get() si units es std::shared_ptr<PropertyRender>


				//renderer->draw ( units[entity->getID ()], position->position, cam );
				renderer->draw ( units[entity->getID ()], ubo );

				continue;
			}


			if (mesh && material && texture) {
				//auto cam = *Tools::getCamera ();
				renderer->draw ( units[entity->getID ()], ubo );

				// renderer->drawEntity ( entity->getID (), position->position, cam );
				//renderer->draw ( mesh->prop, position->position, cam);
				 //auto ee = units[entity->getID ()]; // Aquí no se usa .get() si units es std::shared_ptr<PropertyRender>


				//renderer->draw ( units[entity->getID ()], position->position, cam );


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