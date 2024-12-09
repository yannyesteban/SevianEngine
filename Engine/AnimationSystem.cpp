#include "AnimationSystem.h"
#include <glm/gtx/quaternion.hpp>
namespace SEVIAN {
    glm::vec3 interpolatePosition ( const std::vector<KeyFrame>& keys, float time ) {
        if (keys.empty ()) return glm::vec3 ( 0.0f );

        for (size_t i = 0; i < keys.size () - 1; ++i) {
            if (time < keys[i + 1].time) {
                float factor = (time - keys[i].time) / (keys[i + 1].time - keys[i].time);
                return glm::mix ( keys[i].position, keys[i + 1].position, factor );
            }
        }
        return keys.back ().position; // Tiempo mayor al último keyframe
    }

    glm::quat interpolateRotation ( const std::vector<KeyFrame>& keys, float time ) {
        if (keys.empty ()) return glm::quat ( 1.0f, 0.0f, 0.0f, 0.0f ); // Quaternion identidad

        for (size_t i = 0; i < keys.size () - 1; ++i) {
            if (time < keys[i + 1].time) {
                float factor = (time - keys[i].time) / (keys[i + 1].time - keys[i].time);
                return glm::slerp ( keys[i].rotation, keys[i + 1].rotation, factor );
            }
        }
        return keys.back ().rotation; // Tiempo mayor al último keyframe
    }

    glm::vec3 interpolateScale ( const std::vector<KeyFrame>& keys, float time ) {
        if (keys.empty ()) return glm::vec3 ( 1.0f ); // Escala identidad

        for (size_t i = 0; i < keys.size () - 1; ++i) {
            if (time < keys[i + 1].time) {
                float factor = (time - keys[i].time) / (keys[i + 1].time - keys[i].time);
                return glm::mix ( keys[i].scale, keys[i + 1].scale, factor );
            }
        }
        return keys.back ().scale; // Tiempo mayor al último keyframe
    }

}
void SEVIAN::AnimationSystem::init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) {
}

void SEVIAN::AnimationSystem::update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {

    for (auto entity : entities) {


        auto transform = entity->getComponent<TransformComponent> ();

        auto animation = entity->getComponent<AnimationComponent> ();
        auto skeleton = entity->getComponent<SkeletonComponent> ();


        if (!animation || !skeleton) continue;
       
        animation->currentTime += deltaTime * animation->playbackSpeed * animation->animation.ticksPerSecond;
        if (animation->currentTime > animation->animation.duration) {
            animation->currentTime = fmod ( animation->currentTime, animation->animation.duration );
        }
        /*
        // Actualizar el tiempo de la animación
        animation->currentTime += deltaTime * animation->animation.ticksPerSecond;
        if (animation->currentTime > animation->animation.duration) {
            animation->currentTime = fmod ( animation->currentTime, animation->animation.duration );
        }
        */
        auto& channels = animation->animation.channels;

        // Actualizar transformaciones locales de los huesos
        for (const auto& channel : animation->animation.channels) {
            auto boneIt = skeleton->boneMapping.find ( channel.nodeName );
            if (boneIt == skeleton->boneMapping.end ()) continue;

            int boneIndex = boneIt->second;
            Bone& bone = skeleton->bones[boneIndex];

            glm::vec3 interpolatedPosition = interpolatePosition ( channel.positionKeys, animation->currentTime );
            glm::quat interpolatedRotation = interpolateRotation ( channel.rotationKeys, animation->currentTime );
            glm::vec3 interpolatedScale = interpolateScale ( channel.scaleKeys, animation->currentTime );

            glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), interpolatedPosition );
            glm::mat4 rotation = glm::toMat4 ( interpolatedRotation );
            glm::mat4 scale = glm::scale ( glm::mat4 ( 1.0f ), interpolatedScale );

            bone.localTransform = translation * rotation * scale;
        }

        // Propagar transformaciones globales en la jerarquía
        for (size_t i = 0; i < skeleton->bones.size (); ++i) {
            Bone& bone = skeleton->bones[i];

            if (bone.parentIndex == -1) {
                bone.finalTransform = bone.localTransform;
            }
            else {
                bone.finalTransform = skeleton->bones[bone.parentIndex].finalTransform * bone.localTransform;
            }

            skeleton->boneTransforms[i] = bone.finalTransform * bone.offsetMatrix;
        }


        

/*
       
        entity->addComponent<AnimationComponent> ( aniComponent );

        auto& transform = entityManager.GetComponent<TransformComponent> ( entity );
        auto& animation = entityManager.GetComponent<AnimationComponent> ( entity );

        // Interpolación lineal de posición
        glm::vec3 direction = animation.targetPosition - transform.position;
        float distance = glm::length ( direction );
        glm::vec3 step = glm::normalize ( direction ) * animation.speed * deltaTime;

        if (distance > glm::length ( step )) {
            transform.position += step; // Movimiento incremental
        }
        else {
            transform.position = animation.targetPosition; // Llega al objetivo
        }

        // Rota lentamente
        transform.rotation.y += glm::radians ( 45.0f ) * deltaTime; // 45 grados por segundo
        */
    }
}
