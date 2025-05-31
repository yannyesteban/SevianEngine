#pragma once
#include <string>

#include <vector>
#include <memory>
#include "Vertex.h"
#include "TextEngine.h"
#include "iObjectManager.h"
#include "Text.h"
#include <typeindex>


namespace SEVIAN {

   
 

    namespace RENDERER {

        struct Viewport
        {
            float width;
            float height;
        };

        struct IMemoryData
        {
            ~IMemoryData () = default;
        };


        enum class ShaderResourceType
        {
            UniformBuffer,
            StorageBuffer,
            CombinedImageSampler,
            PushConstant
            // Puedes agregar otros tipos si lo necesitas
        };

        enum class DataResource
        {
            NONE,
            TRANSFORM,
			TEXTURE,
			MATERIAL,
			LIGHT,
			VERTEX,
			INDEX,
			UBO,
			TEXT,
			SPRITE,
			MESH,
			ENTITY,
			BONE,
			ANIMATION,
            STYLE,
            USE_TEXTURE
			
        };

      


        struct PushConstantData
        {
            //glm::mat4 projection; // Matriz de proyección (16 floats = 64 bytes)
            glm::vec4 color;      // Color del widget (4 floats = 16 bytes)

            // Total: 80 bytes, cabe en los 128 bytes mínimos
        };
        struct Transform
        {
            glm::vec3 position { 0.0f };
            glm::quat rotation { 1.0f, 0.0f, 0.0f, 0.0f };
            glm::vec3 scale { 1.0f };
            mutable glm::mat4 worldMatrix { 1.0f };
            mutable bool dirty = true; // Flag para optimización

            void updateWorldMatrix ( const glm::mat4& parentMatrix ) const {
                if (dirty) {
                    glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position );
                    glm::mat4 rotationM = glm::mat4_cast ( rotation );
                    glm::mat4 scaleM = glm::scale ( glm::mat4 ( 1.0f ), scale );

                    worldMatrix = parentMatrix * translation * rotationM * scaleM;
                    dirty = false;
                }
            }
        };
        class IRenderizable
        {
        public:
            virtual ~IRenderizable () = default;

            // 1. Actualización de transformaciones
            virtual void updateTransform ( const Transform& parentTransform ) = 0;

            // 2. Envío de datos al renderizador
            //virtual void submitRenderData ( RenderQueue& queue ) const = 0;

            // 3. Jerarquía
            virtual void addChild ( std::shared_ptr<IRenderizable> child ) = 0;

            // 4. Materiales/Recursos
            //virtual MaterialHandle getMaterial () const = 0;
            //virtual TextureHandle getBaseColorTexture () const = 0;

            /*template<typename T>
			void setData ( DataResource type, T data ) {
				resourcesData[type] = std::make_unique<T> ( data );
			}*/
            // 5. Tipos de renderizable
            enum class Type { MESH, SPRITE, TEXT, LIGHT };

			/*std::unordered_map<DataResource, std::unique_ptr<IMemoryData>> memoryData;
			std::unordered_map<DataResource, std::unique_ptr<IMemoryData>> resourcesData;*/

            std::string test = "hello";
            //virtual Type getType () const = 0;

            virtual void addData ( DataResource , void * data) = 0;
            virtual std::unordered_map < DataResource, void *> getData (  ) = 0;
            virtual void* getData ( DataResource ) = 0;
        };

        struct iElement
        {
            void* handle;
            bool visible = true;
            bool shadow = true;
            virtual ~iElement () = default;
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

        struct RenderObject
        {
            //std::vector<std::unique_ptr<iElement>> elements;
            std::vector<std::shared_ptr<iElement>> elements;
            //std::vector<iElement> elements;

        };

        class iRenderText
        {
        public:
            virtual ~iRenderText () = default;
            virtual std::unique_ptr<IRenderizable> createText ( float x, float y, std::vector<Quad> quads, std::string textureName ) = 0;
            virtual void addFont ( Font font, bool setDefault = false ) = 0;
            virtual Font getFont ( std::string name ) = 0;

        };

        struct GlobalInfo
        {
            UniformBufferObject ubo;
        };

        

        

        class iMeshManager
        {
            
            RenderObject createMesh ( SEVIAN::Info3D info );

        };

        class iTextManager : public ObjectManager
        {
        public:
            //virtual ~iTextManager () = default;
            virtual std::unique_ptr<IRenderizable> createText ( float x, float y, std::vector<Quad> quads, std::string textureName ) = 0;
            virtual std::unique_ptr<IRenderizable> createText ( TEXT::Box box ) = 0;
            virtual void addFont ( Font font, bool setDefault = false ) = 0;
            virtual Font getFont ( std::string name ) = 0;
           

        };

        class iSpriteManager : public ObjectManager
        {
        public:
            virtual std::unique_ptr<IRenderizable> createSprite ( SEVIAN::SpriteInfo info ) = 0;

        };

        class iWidgetManager : public ObjectManager
        {
        public:
            virtual std::unique_ptr<IRenderizable> createSprite ( SEVIAN::SpriteInfo info ) = 0;
           // virtual void setPushValues ( PushConstantData pushData ) = 0;

        };


        class iParticleManager
        {
        public:
            RenderObject createParticleSystem ( const glm::vec3& position, int maxParticles );
        };

        class iSkyboxManager
        {
        public:
            RenderObject createSkybox ( const std::vector<std::string>& texturePaths );
        };

        class iPostProcessingManager
        {
        public:
            RenderObject createPostProcessingEffect ( const std::string& effectName );
        };

        class iUIManager
        {
        public:
            RenderObject createButton ( const std::string& label, const glm::vec2& position, const glm::vec2& size );
            RenderObject createText ( const std::string& text, const glm::vec2& position );
        };


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
        private:
            std::unordered_map<std::type_index, std::unique_ptr<RENDERER::ObjectManager>> managers;



            std::string testValue = "Esteban";
        public:

            void printTest () {
                std::cout << testValue << "\n";
            }

            // virtual void initialize () = 0;
            virtual ~RenderInterface () = default;
            virtual void initialize () = 0;
            virtual void renderFrame () = 0;
            virtual void cleanup () = 0;
            virtual bool windowShouldClose () = 0;

            virtual void addTexture ( TextureInfo info ) = 0;

            virtual void addTexture ( std::string, std::string ) = 0;
            virtual void addShaders ( std::string, std::string ) = 0;


            virtual void beginFrame () = 0;
            virtual void endFrame () = 0;

            virtual void beginRenderPass ( int index ) = 0;
            virtual void endRenderPass () = 0;

            virtual void draw ( std::shared_ptr<Entity3D>, glm::vec3, Camera ) = 0;
            virtual void draw ( std::shared_ptr<Entity3D>, UniformBufferObject ubo ) = 0;
            virtual void drawText ( std::string text, glm::vec3 position, Camera camera ) = 0;

            virtual void initEntity ( Info3D info ) = 0;
            virtual void initEntity ( Entity3D* unit ) = 0;


            virtual void drawEntity ( uint32_t entityId, glm::vec3 position, Camera ) = 0;


            virtual void drawEntity ( Entity3D* unit, glm::vec3 position, Camera ) = 0;

            //virtual std::unique_ptr<Entity3D> createEntity ( Info3D info ) = 0;
            //virtual std::unique_ptr<Entity3D> createSprite ( Sprite3D info ) = 0;

            //virtual std::unique_ptr<Entity3D> createModel ( Model3D info ) = 0;

            //virtual void draw ( std::shared_ptr<Entity3D>, UniformBufferObject ubo ) override;

            virtual void draw ( std::vector<RENDERER::iElement> components, RENDERER::GlobalInfo info ) = 0;
            virtual void draw ( std::shared_ptr<RENDERER::iElement> components, RENDERER::GlobalInfo info ) = 0;
            virtual void draw ( std::shared_ptr<RENDERER::RenderObject> object, RENDERER::GlobalInfo info ) = 0;
            virtual void draw ( std::shared_ptr<RENDERER::IRenderizable> object, RENDERER::GlobalInfo info ) = 0;
            virtual void draw ( std::shared_ptr<RENDERER::IRenderizable> object, Camera2D camera ) = 0;
            virtual void draw ( std::shared_ptr<RENDERER::IRenderizable> object) = 0;

            virtual void setViewport ( float width, float height ) = 0;
            virtual Viewport getViewport () = 0;

            template<typename T, typename T2, typename... Args>
            void registerManager2 ( Args&&... args ) {
                auto type = std::type_index ( typeid(T) );
                /*
                 std::unordered_map<std::type_index, std::unique_ptr<RENDERER::iRenderManager>> managers;
                 */
                managers[type] = std::make_unique<T2> ( std::forward<Args> ( args )... );
            }

            template<typename T, typename T2, typename... Args>
            void registerManager ( Args&&... args ) {
                auto type = std::type_index ( typeid(T) );
                managers[type] = std::make_unique<T2> ( std::forward<Args> ( args )... );
            }
            

            template<typename T>
            T* getManager () {
                auto it = managers.find ( std::type_index ( typeid(T) ) );
                if (it != managers.end ()) {
                    return static_cast<T*>(it->second.get ());
                }
                throw std::runtime_error ( "Manager not registered" );
            }

            template<typename T>
            T* getManager2 () {
                auto it = managers.find ( std::type_index ( typeid(T) ) );
                if (it != managers.end ()) {
                    return dynamic_cast<T*>(it->second.get ());
                }
                throw std::runtime_error ( "Manager not registered" );
            }
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

    }




    