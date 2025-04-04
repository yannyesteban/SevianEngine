#pragma once
#include "Vulkan.h"
#include "VKElement.h"
#include "VulkanProperty.h"
#include "RenderInterface.h"
#include "PhysicalDevice.h"
#include "Entity.h"
#include "VulkanText.h"
#include "VulkanEntity.h"
#include "MeshManager.h"
#include "ShadowWorld.h"

#include "MeshEntity.h"

#include <stdexcept>
//#include <cstddef>
#include <iostream>
#include <optional>

#include <set>
#include <chrono>
#include <map>
#include <algorithm>
#include <unordered_map>
#include "Text.h"
#include "TextEntity.h"


#ifdef NDEBUG
const bool CenableValidationLayers = false;
#else
const bool CenableValidationLayers = true;
#endif

//using ::SEVIAN::Vertex;
using namespace ::SEVIAN;
namespace SEVIAN {
    namespace VULKAN {

        static void loadModel ( const std::string path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices );

        const int MAX_FRAMES_IN_FLIGHT = 2;
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback ( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData ) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

            return VK_FALSE;
        }



        /*

        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions () {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions ( 4, {} );

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof ( SEVIAN::Vertex, position );

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof ( SEVIAN::Vertex, normal );

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof ( SEVIAN::Vertex, texCoords );

            attributeDescriptions[3].binding = 0;
            attributeDescriptions[3].location = 3;
            attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[3].offset = offsetof ( SEVIAN::Vertex, color );

            return attributeDescriptions;
        }

        static VkVertexInputBindingDescription getBindingDescription () {
            VkVertexInputBindingDescription bindingDescription {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof ( SEVIAN::Vertex );
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }
        */
        class VulkanRenderer : public RENDERER::RenderInterface
        {
        public:
            Device* device;
            VkDescriptorPool descriptorPool;
            std::vector<Frame> frames;

            VulkanEntity* ve;

            VulkanRenderer () { }
            VulkanRenderer ( GLFWwindow* window, uint32_t width, uint32_t height ) : window ( window ), width ( width ), height ( height ) {
                setViewport ( width, height );
            }
            //~VulkanRenderer () override {     // Liberar recursos espec�ficos de esta clase            }
            SEVIAN::RENDERER::GameApp* app;

            std::vector<Texture> textures;
            std::vector<Shader> shaders;
            std::vector<Item> items;

            Text* texto;
            ShadowWorld* shadowWorld;

            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };
            const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
            const bool enableValidationLayers = CenableValidationLayers;


            void initialize ();
            void renderFrame () override;
            void cleanup () override;

            bool windowShouldClose () override;
            void test () {


                std::cout << "Hello";
            }
            //std::vector<VkDescriptorSet> createDescriptorSets ( std::vector<Frame>& frames, VkDescriptorPool descriptorPool, VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range );



            void addTexture ( TextureInfo info ) override;
            void addTexture ( std::string name, std::string path ) override;
            void addShaders ( std::string, std::string ) override;

            void initEntity ( Info3D info ) override;
            void drawEntity ( uint32_t entityId, glm::vec3 position, Camera ) override;


            void initEntity ( RENDERER::Entity3D* unit ) override;
            void drawEntity ( RENDERER::Entity3D* unit, glm::vec3 position, Camera ) override;

            void beginFrame () override;
            void endFrame () override;

            void beginRenderPass ( int index ) override;
            void endRenderPass () override;

            std::unique_ptr<RENDERER::Entity3D> createEntity ( Info3D info );
            //std::unique_ptr<Entity3D> createSprite ( Sprite3D info ) override;
            //std::unique_ptr<Entity3D> createModel ( Model3D info ) override;

            void draw ( std::shared_ptr<RENDERER::Entity3D>, glm::vec3 position, Camera ) override;
            void draw ( std::shared_ptr<RENDERER::Entity3D>, UniformBufferObject ubo ) override;
            void drawText ( std::string text, glm::vec3 position, Camera camera ) override;


            void draw ( std::vector<RENDERER::iElement> components, RENDERER::GlobalInfo info ) override;
            void draw ( std::shared_ptr<RENDERER::iElement> components, RENDERER::GlobalInfo info ) override;
            void draw ( std::shared_ptr<RENDERER::RenderObject> object, RENDERER::GlobalInfo info ) override;
            void draw ( std::shared_ptr<RENDERER::IRenderizable> object, RENDERER::GlobalInfo info ) override;
            void draw ( std::shared_ptr<RENDERER::IRenderizable> object, Camera2D camera ) override;
            void draw ( std::shared_ptr<RENDERER::IRenderizable> object ) override;

            // Heredado v�a RenderInterface
            void setViewport ( float width, float height ) override;

            RENDERER::Viewport getViewport () override;

            static void keyCallback ( GLFWwindow* window, int key, int scancode, int action, int mods ) {
                VulkanRenderer* app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer ( window ));

                if (action == GLFW_PRESS) {
                    app->app->onKeyPress ( key );
                }


                else if (action == GLFW_RELEASE) {
                    app->app->onKeyRelease ( key );
                }

            }



        private:
            GLFWwindow* window;
            uint32_t width = 800;
            uint32_t height = 800;
            RENDERER::Viewport viewport ;
            std::string title = "Vulkan 1.0";
            std::shared_ptr<TextureManager> textureManager;

            VkInstance instance;
            VkDebugUtilsMessengerEXT debugMessenger;

            VkSurfaceKHR surface;
            std::vector <PhysicalDevice>physicalDevices;
            PhysicalDevice* physicalDevice;
            SwapChain  swapChain;
            //Device* device;
            VkCommandPool commandPool;

            VkQueue graphicsQueue;
            VkQueue presentQueue;

            VkExtent2D extent;
            VkRenderPass renderPass;
            VkRenderPass shadowRenderPass;
            VulkanDepthResources depthResources;
            VulkanDepthResources depthResources2;
            VkFramebuffer shadowFrameBuffer;

            VkDescriptorSetLayout descriptorSetLayout;
            //VkDescriptorPool descriptorPool;

            VkCommandBuffer commandBuffer;

           
            //uint32_t currentFrame = 0;
            uint32_t imageIndex;
            bool framebufferResized = false;
            VulkanText fontText;

            TextEntity* textManager;
            VkExtent2D shadowExtent = { 2048, 2048 };
            std::unordered_map<uint32_t, std::unique_ptr<Entity>> entities;

            // methods
            void initWindow ();

            bool checkValidationLayerSupport ();
            std::vector<const char*> getRequiredExtensions ();
            void populateDebugMessengerCreateInfo ( VkDebugUtilsMessengerCreateInfoEXT& createInfo );
            void createInstance ();

            VkResult CreateDebugUtilsMessengerEXT ( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger );
            void setupDebugMessenger ();

            void createSurface ();
            void cleanupSwapChain ();
            void recreateSwapChain ();
            void updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position, Camera camera );
            void updateUniformBuffer ( Frame frame );

            /*Pipeline createGraphicsPipeline (
                VkVertexInputBindingDescription bindingDescription,
                std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
                VkDescriptorSetLayout& descriptorSetLayout );*/
                //std::map<std::string, VulkanTexture> mTextures;



            std::map<std::string, std::shared_ptr<VulkanTexture>> mTextures;




            Pipeline pipeline;

            MeshManager* meshManager;
            //std::unique_ptr<MeshManager> meshManager;
            std::unordered_map<std::type_index, std::unique_ptr<RENDERER::iRenderManager>> managers;


           

};

        class VulkanTextureNO : public RENDERER::TextureInterface
        {
        public:
            const char* id;
            VkImageView imageView;
            VkImage textureImage;
            VkDeviceMemory textureImageMemory;
            VkSampler sampler;
            void load ( const std::string& path ) override;

        };

        class VulkanShader : public RENDERER::ShaderInterface
        {
        public:
            void load ( const std::string& vertexPath, const std::string& fragmentPath ) override;
        };
    }


}