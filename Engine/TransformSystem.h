#pragma once


#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
#include "Component.h"
#include <RendererFactory.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Animation.h"

namespace SEVIAN {
    struct TransformComponent : public Component
    {
        glm::vec3 position = glm::vec3 ( 0.0f );
        glm::vec3 rotation = glm::vec3 ( 0.0f ); // Radians
        glm::vec3 scale = glm::vec3 ( 1.0f );
        glm::mat4 modelMatrix = glm::mat4 ( 1.0f ); // Calculada
        glm::mat4 local;
        glm::mat4 world;

        Entity parent = Entity(-1);
        std::vector<Entity> children;

        void update ( float deltaTime ) override;
    };
    
    struct HierarchyComponent
    {
        Entity parent;
        std::vector<Entity> children;
    };

    class TransformSystem : public System
    {
     
    public:
        TransformSystem (  ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
    };


    struct SkeletonComponent : public Component
    {
        std::unordered_map<std::string, int> boneMapping; // Nombre -> Índice
        std::vector<Bone> bones;                         // Información de cada hueso
        std::vector<glm::mat4> boneTransforms;           // Transformaciones finales para el shader

        void update ( float deltaTime ) override;

        void initializeBoneTransforms ( int boneCount ) {
            boneTransforms.resize ( boneCount, glm::mat4 ( 1.0f ) );
        }

        void updateBoneTransform ( const std::string& boneName, const glm::mat4& transform ) {
            if (boneMapping.find ( boneName ) != boneMapping.end ()) {
                int boneIndex = boneMapping[boneName];
                boneTransforms[boneIndex] = transform;
            }
        }
    };

    struct AnimationComponent : public Component
    {
       
        Animation animation;
        std::vector<Animation> animations;
        float currentTime = 0.0f;                           // Tiempo actual en la animación activa
        bool isPlaying = false;
        float playbackSpeed = 1.0f;     // Velocidad de reproducción (1.0 = normal)
        void update ( float deltaTime ) override;
    };


}
