#include "RenderSystem2.h"
#include "Models.h"
#include "TransformSystem.h"





//#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>        // Cabecera principal de GLM
#include <glm/gtc/matrix_transform.hpp> // Para glm::translate, glm::scale, glm::rotate, etc.
#include <glm/gtc/quaternion.hpp>      // Para glm::quat y funciones relacionadas con cuaterniones
//#include <glm/gtc/matrix_transform.hpp> // (Ya incluida arriba, pero para asegurar)
#include <glm/gtc/type_ptr.hpp>  


void SEVIAN::RenderSystem2::init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) {
}

void SEVIAN::RenderSystem2::update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {


	renderer->beginFrame ();
	renderer->beginRenderPass ( 1 ); // draw 


	CameraComponent* lastCamera = nullptr;
	Camera2DComponent* lastCamera2D = nullptr;
	LightComponent* lastLight = nullptr;
	UniformBufferObject shadowUBO {};


	
	for (auto& entity : entities) {

		auto mesh = entity->getComponent<RenderComponent> ();
		auto position = entity->getComponent<PositionComponent> ();
		auto rotation = entity->getComponent<RotationComponent> ();
		auto scale = entity->getComponent<ScaleComponent> ();
		auto camera = entity->getComponent<CameraComponent> ();
		auto camera2D = entity->getComponent<Camera2DComponent> ();
		auto light = entity->getComponent<LightComponent> ();
		auto transform = entity->getComponent<TransformComponent> ();
		auto skeleton = entity->getComponent<SkeletonComponent> (); // Nueva línea

		if (camera) {
			lastCamera = camera;
			continue;
		}
		if (camera2D) {
			lastCamera2D = camera2D;
			continue;
		}
		if (light) {
			lastLight = light;
			continue;
		}
		if (lastCamera == nullptr || lastLight == nullptr || lastCamera2D == nullptr) {
			continue;
		}
		if (!mesh) {
			continue;
		}

		UniformBufferObject ubo {};

		//position->position = glm::vec3 ( -500.5, 200.0, 0.0 );
		ubo.model = transform->modelMatrix;  //glm::translate ( glm::mat4 ( 1.0f ), position->position );// transform->modelMatrix;
		ubo.view = lastCamera2D->view; //glm::mat4 ( 1.0f );// lastCamera2D->view; //;
		ubo.proj = lastCamera2D->proj;

		
		
		
		//ubo.boneTransforms[0] = glm::mat4 ( 1.0f ); // Inicializa con identidad
		//ubo.boneTransforms[0][0] = glm::vec4 ( 0.0f, 0.7f, 0.6f, 1.0f ); // Codifica rojo
		//ubo.boneTransforms[0][1] = glm::vec4 ( 0.0f, 0.0f, 0.0f, 1.0f ); // Codifica verde (opcional)
		//ubo.boneTransforms[0][2] = glm::vec4 ( 0.0f, 0.0f, 0.0f, 1.0f ); // Codifica azul (opcional)
		//ubo.boneTransforms[0][3] = glm::vec4 ( 0.0f, 0.0f, 0.0f, 1.0f ); // Última fila (opcional)
		//std::shared_ptr<RENDERER::iElement> miComponente = std::make_shared<RENDERER::iElement> ( mesh->element );
		//RENDERER::GlobalInfo global = { ubo };
		
		
		
		//renderer->draw ( mesh->element, { ubo } );
		
	}

	static int index = 0;
	auto width = 1000.0f;
	auto height = 1000.0f;
	


	Camera2D camera2D { width, height };

	
	info.widgetManager->render ( renderer, camera2D );

	std::cout << "timer " << index << "\n";
	index++;
	renderer->endRenderPass ();
	renderer->endFrame ();


}
