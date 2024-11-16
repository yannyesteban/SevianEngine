#include "renderSystem.h"

namespace SEVIAN {
	void RenderSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {

		for (auto& entity : entities) {

			auto index = entity->getID ();

			auto mesh = entity->getComponent<MeshComponent> ();
			auto material = entity->getComponent<MaterialComponent> ();
			auto texture = entity->getComponent<TextureComponent> ();
			auto sprite = entity->getComponent<SpriteComponent> ();
			auto model = entity->getComponent<ModelComponent> ();

			if (mesh /* && t == "diffuse1"*/ && texture) {
				Info3D info;
				
				renderer->addTexture ( texture->name, texture->path );

				info.entityId = index;
				info.vertices = mesh->vertices;
				info.indices = mesh->indices;
				
				info.texture = texture->name;
				info.path = texture->path;
				auto e = renderer->createEntity ( info );
				std::cout << " model " << texture->path << "</n";
				mesh->prop = std::move ( e );
				//units[index] = std::move ( e );
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

				renderer->addTexture ( texture->name, texture->path );

				info.entityId = entity->getID ();
				info.indices = mesh->indices;
				info.path = texture->path;
				info.texture = texture->name;
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
		
		CameraComponent * lastCamera = nullptr;
		LightComponent* lastLight = nullptr;
		UniformBufferObject shadowUBO {};
		renderer->beginRenderPass ( 0 );// shadows render pass
		for (auto& entity : entities) {

			auto mesh = entity->getComponent<MeshComponent> ();
			auto position = entity->getComponent<PositionComponent> ();
			auto rotation = entity->getComponent<RotationComponent> ();
			auto scale = entity->getComponent<ScaleComponent> ();
			auto camera = entity->getComponent<CameraComponent> ();
			auto light = entity->getComponent<LightComponent> ();
			auto name = entity->getComponent<NameComponent> ();

			if (name && (name->name != Key::N5 && name->name != Key::N0 && name->name != Key::N1)) {
				//continue;
			}
			if (camera) {
				lastCamera = camera;
				continue;
			}
			if (light) {
				lastLight = light;
				continue;
			}
			if (lastCamera == nullptr || lastLight == nullptr) {
				continue;
			}
			if (!mesh) {
				continue;
			}

			
			UniformDataDept shadowMVP;
			glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position->position );
			//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

			glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
				glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
				glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
			shadowUBO.model = translation * rotationMat;	//ubo.model = /* rotation * */  translation;

			// Matriz de vista desde el punto de vista de la luz
			shadowUBO.lightView = lastLight->view;
			shadowUBO.lightProj = lastLight->proj;
			shadowUBO.lightPos = lastLight->position;
			//shadowUBO.cameraPos = lastCamera->position;
			shadowMVP.MVP = lastLight->proj * lastLight->view * shadowUBO.model;
			shadowUBO.MVP = lastLight->proj * lastLight->view * shadowUBO.model;
			mesh->prop->ShadowRender ( shadowUBO );
			//renderer->drawText ( "YANNY", position->position, cam );

		}

		renderer->endRenderPass ();

		renderer->beginRenderPass ( 1 ); // draw 
		for (auto& entity : entities) {
			
			auto mesh = entity->getComponent<MeshComponent> ();
			auto position = entity->getComponent<PositionComponent> ();
			auto rotation = entity->getComponent<RotationComponent> ();
			auto scale = entity->getComponent<ScaleComponent> ();
			auto camera = entity->getComponent<CameraComponent> ();
			auto light = entity->getComponent<LightComponent> ();

			if (camera) {
				lastCamera = camera;
				continue;
			}
			if (light) {
				lastLight = light;
				continue;
			}
			if (lastCamera == nullptr || lastLight == nullptr) {
				continue;
			}
			if (!mesh) {
				continue;
			}

			UniformBufferObject ubo {};

			glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position->position );
			//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

			glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
									glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
									glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
			ubo.model = translation * rotationMat;	//ubo.model = /* rotation * */  translation;

			ubo.view = lastCamera->view;
			ubo.proj = lastCamera->proj;
			
			ubo.lightView = shadowUBO.lightView;// lastLight->view;
			ubo.lightProj = shadowUBO.lightProj;// lastLight->proj;

			ubo.lightColor = lastLight->color;
			ubo.lightIntensity = lastLight->intensity;
			
			ubo.lightPos = shadowUBO.lightPos;//lastLight->position;
			ubo.cameraPos = lastCamera->position;
			ubo.MVP = shadowUBO.lightProj * shadowUBO.lightView * ubo.model;
			mesh->prop->render ( ubo );
			
			
		}
		Camera cam = {};
		renderer->drawText ( "YANNY", glm::vec3(0.0, 0.0, 0.0), cam);

		renderer->endRenderPass ();
		renderer->endFrame ();
	}
	void StaticRenderSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {
	}
}