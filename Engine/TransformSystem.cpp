#include "TransformSystem.h"

void SEVIAN::TransformComponent::update ( float deltaTime ) {
}

void SEVIAN::TransformSystem::init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {
}

void SEVIAN::TransformSystem::update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {

    for (auto& entity : entities) {
        //auto transform = entity->getComponent<TransformComponent> ();
        //auto hierarchy = entity->getComponent<HierarchyComponent> ();

		auto mesh = entity->getComponent<MeshComponent> ();
		auto position = entity->getComponent<PositionComponent> ();
		auto rotation = entity->getComponent<RotationComponent> ();
		auto scale = entity->getComponent<ScaleComponent> ();
		//auto camera = entity->getComponent<CameraComponent> ();
		//auto light = entity->getComponent<LightComponent> ();
		//auto name = entity->getComponent<NameComponent> ();
		auto transform = entity->getComponent<TransformComponent> ();

		
		if (!mesh) {
			continue;
		}


		UniformDataDept shadowMVP;
		glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position->position );
		//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

		glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
			glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
			glm::rotate ( glm::mat4 ( 1.0f ), rotation->rotation.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
		transform->modelMatrix = translation * rotationMat;	//ubo.model = /* rotation * */  translation;
		/*
		for (auto& entity : entities) {
        auto skeleton = entity->getComponent<SkeletonComponent>();
        if (!skeleton) continue;

        for (size_t i = 0; i < skeleton->bones.size(); ++i) {
            if (skeleton->bones[i].parentIndex == -1) {
                // Es la raíz del esqueleto
                skeleton->bones[i].currentTransform = skeleton->bones[i].currentTransform;
            } else {
                // Multiplicar por la transformación global del padre
                int parentIndex = skeleton->bones[i].parentIndex;
                skeleton->bones[i].currentTransform = 
                    skeleton->bones[parentIndex].currentTransform * skeleton->bones[i].currentTransform;
            }
        }
    }
		
		*/


        /*
        
        // Interpolación lineal de posición
        glm::vec3 direction = animation.targetPosition - transform.position;
        float distance = glm::length(direction);
        glm::vec3 step = glm::normalize(direction) * animation.speed * deltaTime;

        if (distance > glm::length(step)) {
            transform.position += step; // Movimiento incremental
        } else {
            transform.position = animation.targetPosition; // Llega al objetivo
        }

        // Rota lentamente
        transform.rotation.y += glm::radians(45.0f) * deltaTime; 
        */
    }



}

void SEVIAN::SkeletonComponent::update ( float deltaTime ) {
}

void SEVIAN::AnimationComponent::update ( float deltaTime ) {
}
