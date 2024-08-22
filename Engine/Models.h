#pragma once

#include "Tools.h"
#include "Component.h"
#include "Vertex.h"
#include "RenderInterface.h"

#include <glm/glm.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <bitset>


namespace SEVIAN {


    struct SpriteComponent : public Component
    {
        glm::vec3 position;
        float width = 1.0f;
        float height = 1.0f;
        std::string texture;
        glm::vec4 color;
        SpriteComponent (const glm::vec3& _pos, float& _width, float & _height, const std::string& _texture, const glm::vec4 & _color )
            : position ( _pos ), width ( _width ), height(_height), texture (_texture), color(_color) {
            // prop = PropertyRender {};
        }

        void update ( float deltaTime ) override;
    };


    struct MeshComponent : public Component
    {

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkBuffer indexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
        VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        //PropertyRender* prop = nullptr;
        std::shared_ptr<PropertyRender> prop;

        MeshComponent ( const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds )
            : vertices ( verts ), indices ( inds ) {
            // prop = PropertyRender {};
        }

        void update ( float deltaTime ) override;
    };

    struct MaterialComponent : public Component
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;

        MaterialComponent ( const glm::vec4& amb, const glm::vec4& diff, const glm::vec4& spec, float shin )
            : ambient ( amb ), diffuse ( diff ), specular ( spec ), shininess ( shin ) {
        }

        void update ( float deltaTime ) override;
    };

    struct TextureComponent : public Component
    {
        std::string type;
        std::string path;
        //VkImage textureImage = VK_NULL_HANDLE;
        //VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
        //VkImageView textureImageView = VK_NULL_HANDLE;
        //VkSampler textureSampler = VK_NULL_HANDLE;
        TextureComponent ( const std::string& type, const std::string& path )
            : type ( type ), path ( path ) {
        }
        void update ( float deltaTime ) override;
    };

    struct PositionComponent : public Component
    {
        glm::vec3 position;
        void update ( float deltaTime ) override;
        PositionComponent ( const glm::vec3& position )
            : position ( position ) {
        }
    };

    struct RotationComponent : public Component
    {
        glm::vec3 rotation;
        void update ( float deltaTime ) override;
        RotationComponent ( const glm::vec3& rotation )
            : rotation ( rotation ) {
        }
    };

    struct ScaleComponent : public Component
    {
        glm::vec3 scale;
        void update ( float deltaTime ) override;
        ScaleComponent ( const glm::vec3& scale )
            : scale ( scale ) {
        }
    };

    struct InputComponent : public Component
    {
        void update ( float deltaTime ) override;
        std::bitset<static_cast<size_t>(Key::MAX_KEYS)> keyStates;
    };


    struct CameraComponent : public Camera, public Component
    {
        void update ( float deltaTime ) override;
        
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;

        float zoomLevel = 45.0f;

        float yaw = -90.0f;
        float pitch = 0.0f;
        float sensitivity = 0.1f;
        

       
    };

    struct NameComponent : public Component
    {
        Key name = Key::A;
        bool active = false;
        NameComponent ( const Key& _name )
            : name ( _name ) {
        }
        void update ( float deltaTime ) override;
    };
}

