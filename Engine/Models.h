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

    struct RenderComponent : public Component
    {
        

        std::shared_ptr<RENDERER::IRenderizable> element;
        void update ( float deltaTime ) override;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector <std::string> texture;

    };

    

    struct MeshesComponent : public Component
    {
        std::vector<Mesh> meshes;
        std::shared_ptr<RENDERER::Entity3D> prop;
        void update ( float deltaTime ) override;
       
    };
    struct SpriteComponent : public Component
    {
        glm::vec3 position;
        float width = 1.0f;
        float height = 1.0f;
        std::string texture;
        glm::vec3 color;
        SpriteComponent (const glm::vec3& _pos, float& _width, float & _height, const std::string& _texture, const glm::vec3 & _color )
            : position ( _pos ), width ( _width ), height(_height), texture (_texture), color(_color) {
            // prop = PropertyRender {};
        }

        void update ( float deltaTime ) override;
    };

    struct ModelComponent : public Component
    {

        std::string path;
        std::string texture;
        

        ModelComponent ( const std::string & path, const std::string & texture )
            : path ( path ), texture ( texture ) {
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
        std::shared_ptr<RENDERER::Entity3D> prop;

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
        std::string name;
        std::string path;
        //VkImage textureImage = VK_NULL_HANDLE;
        //VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
        //VkImageView textureImageView = VK_NULL_HANDLE;
        //VkSampler textureSampler = VK_NULL_HANDLE;
        TextureComponent ( const std::string& type, const std::string& path )
            : name ( type ), path ( path ) {
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
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::quat quaternion = glm::quat ( 1.0f, 0.0f, 0.0f, 0.0f );

        void update ( float deltaTime ) override;
        RotationComponent ( const glm::vec3& rotation )
            : rotation ( rotation ) {
        }
        void setEulerAngles ( float pitch, float yaw, float roll ) {
            quaternion = glm::quat ( glm::radians ( glm::vec3 ( pitch, yaw, roll ) ) );
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


    
    
    struct NameComponent : public Component
    {
        Key name = Key::A;
        bool active = false;
        NameComponent ( const Key& _name )
            : name ( _name ) {
        }
        void update ( float deltaTime ) override;
    };

    struct CameraComponent : public Camera, public Component
    {
        void update ( float deltaTime ) override;

        //glm::vec3 position = glm::vec3 ( 0.5f, 3.0f, 5.0f );
        glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
        glm::vec3 up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

        glm::vec3 position = glm::vec3 ( -2.312f, 0.358f, 28.0f );

        float zoomLevel = 45.0f;
        float fov = 45.0f;
        float width = 0.0f;
        float height = 0.0f;
        float yaw = -90.0f;
        float pitch = 0.0f;
        float sensitivity = 0.1f;
        float distanceFromOrigin = 9.0f; // Distancia fija desde el origen
        float angle = 0.0f; // Ángulo de rotación

        float angleX = 0.0f; // Ángulo en el eje X
        float angleY = 0.0f; // Ángulo en el eje Y
        float distance = 5.0f; // Distancia desde el origen (zoom)

        alignas(16) glm::mat4 view {1.0f};
        alignas(16) glm::mat4 proj {1.0f};



    };

    struct Camera2DComponent : public Camera, public Component // Asumiendo que tienes una clase base Component
    {
        void update ( float deltaTime ) override;

        float orthoLeft = 0.0f;
        float orthoRight = 800.0f; // Ancho de pantalla predeterminado
        float orthoBottom = 0.0f;
        float orthoTop = 600.0f;   // Alto de pantalla predeterminado

        glm::mat4 proj2D;
        glm::mat4 view2D; // Opcional, si necesitas vista 2D no estática. Si no, puede ser identidad.
        float width = 0.0f;
        float height = 0.0f;

        void updateOrthographicProjection ( float screenWidth, float screenHeight ) {
            orthoRight = screenWidth;
            orthoTop = screenHeight;
            proj2D = glm::ortho ( orthoLeft, orthoRight, orthoBottom, orthoTop, -1.0f, 1.0f );
        }
        float zoom = 1.0f;                     // Nivel de zoom
        float rotation = 0.0f;
        // Parámetros de proyección (basados en pantalla)
        float left, right, bottom, top;
        float near = -1.0f, far = 1.0f;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
        void updateProjection ( float screenWidth, float screenHeight ) {
            // Ajusta según zoom y posición
            left = -screenWidth / 2.0f / zoom;
            right = screenWidth / 2.0f / zoom;
            bottom = -screenHeight / 2.0f / zoom;
            top = screenHeight / 2.0f / zoom;

            proj = glm::ortho ( left, right, bottom, top, near, far );
            proj[1][1] *= -1; // Inversión Y para Vulkan
        }

        // Opcional: Métodos para zoom 2D, panning, etc., si los necesitas.
    };

    struct LightComponent : public Component
    {
        enum LightType
        {
            Directional,
            Point,
            Spotlight
        };

        LightType type = Directional;
        
        glm::vec3 position = glm::vec3 ( 0.0, 0.0, 5.0 );  // Solo para Point/Spotlight
        glm::vec3 direction = glm::vec3 ( 0.0f, 0.0f, -3.0f );  // Para Directional/Spotlight
        glm::vec3 up = glm::vec3 ( 0.0f, 1.0f, 0.0f );
        glm::vec3 color = glm::vec3 ( 1.0f );  // Color de la luz
        float intensity = 1.0f;  // Intensidad de la luz
        float radius = 1.0f;  // Para Point/Spotlight
        float cutoff = glm::radians ( 12.5f );  // Ángulo para spotlight
        glm::mat4 view;
        glm::mat4 proj;

        float near_plane = 1.0f;// 0.1f;
        float far_plane = 45.0f;// 50.0f;
        float shadowMapSize = 10.0;// 5.0f; // Ajusta según la distancia que quieras cubrir
        void update ( float deltaTime ) override;
    };
}

