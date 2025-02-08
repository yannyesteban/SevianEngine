#pragma once
#include <string>
#include "Vertex.h"
#include <vector>
#include <memory>

namespace SEVIAN {

    namespace RENDERER {

        struct iElement
        {
            void* handle;
            bool visible = true;
            bool shadow = true;
            /*
            implementacion en Vulkan
            VkPipeline pipeline = VK_NULL_HANDLE;
            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        

            std::vector<VkDescriptorSet> descriptorSets;

            size_t indicesSizes;

            VkBuffer vertexBuffer = VK_NULL_HANDLE;
            VkBuffer indexBuffer = VK_NULL_HANDLE;
        


            VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;
        
            */
        };

        class iObject
        {
        private:
            std::vector<iElement> elements;
        public:
            void addElement ( iElement element );

            void render ();
        };

        class iMeshManager
        {
            
            iObject createMesh ( SEVIAN::Info3D info );

        };

        class iTextManager
        {

            iObject createText ( SEVIAN::Info3D info );

        };

        class iSpriteManager
        {

            virtual iObject createSprite ( SEVIAN::SpriteInfo info ) = 0;

        };
        class iParticleManager
        {
        public:
            iObject createParticleSystem ( const glm::vec3& position, int maxParticles );
        };

        class iSkyboxManager
        {
        public:
            iObject createSkybox ( const std::vector<std::string>& texturePaths );
        };

        class iPostProcessingManager
        {
        public:
            iObject createPostProcessingEffect ( const std::string& effectName );
        };

        class iUIManager
        {
        public:
            iObject createButton ( const std::string& label, const glm::vec2& position, const glm::vec2& size );
            iObject createText ( const std::string& text, const glm::vec2& position );
        };

    }




    class GameApp
    {
    public:
        virtual ~GameApp () = default;
        virtual void onKeyPress ( int key ) = 0;
        virtual void onKeyRelease ( int key ) = 0;


        

    };

    class UnitRender
    {
    public:
        virtual ~UnitRender () = default;

    };

    class Entity3D
    {
    public:
        virtual ~Entity3D () = default;
        virtual void render ( UniformBufferObject ubo ) = 0;
        virtual void ShadowRender ( UniformBufferObject ubo ) = 0;
        
    
    };
    
    class RenderInterface
    {
    public:
        // virtual void initialize () = 0;
        virtual ~RenderInterface () = default;
        virtual void initialize (  ) = 0;
        virtual void initialize ( GameApp * ) = 0;
        virtual void doLoop () = 0;
        virtual void renderFrame () = 0;
        virtual void cleanup () = 0;
        virtual bool windowShouldClose () = 0;

        virtual void addTexture ( TextureInfo info ) = 0;

        virtual void addTexture ( std::string, std::string) = 0;
        virtual void addShaders ( std::string, std::string ) = 0;
        //virtual PropertyRender elementInit (Vertex, uint32_t) = 0;
        virtual std::shared_ptr<Entity3D> init ( std::vector<Vertex>, std::vector<uint32_t>, std::string) = 0;
        virtual void test () = 0;
        virtual void test2 () = 0;

        virtual void beginFrame () = 0;
        virtual void endFrame () = 0;

        virtual void beginRenderPass (int index) = 0;
        virtual void endRenderPass () = 0;

        virtual void draw ( std::shared_ptr<Entity3D>, glm::vec3, Camera ) = 0;
        virtual void draw ( std::shared_ptr<Entity3D>, UniformBufferObject ubo ) = 0;
        virtual void drawText ( std::string text, glm::vec3 position, Camera camera ) = 0;

        virtual void initEntity ( Info3D info ) = 0;
        virtual void drawEntity ( uint32_t entityId, glm::vec3 position, Camera ) = 0;

        virtual void initEntity ( Entity3D * unit ) = 0;
        virtual void drawEntity ( Entity3D * unit, glm::vec3 position, Camera ) = 0;

        virtual std::unique_ptr<Entity3D> createEntity ( Info3D info ) = 0;
        virtual std::unique_ptr<Entity3D> createSprite ( Sprite3D info ) = 0;

        virtual std::unique_ptr<Entity3D> createModel ( Model3D info ) = 0;

        //virtual void draw ( std::vector<RENDERER::iObject> components ) = 0;
    };

    // TextureInterface.h
    class TextureInterface
    {
    public:
        virtual ~TextureInterface () = default;
        virtual void load ( const std::string& path ) = 0;
    };

    // ShaderInterface.h
    class ShaderInterface
    {
    public:
        virtual ~ShaderInterface () = default;
        virtual void load ( const std::string& vertexPath, const std::string& fragmentPath ) = 0;
    };
}