#pragma once
#include "Vulkan.h"
#include "VulkanProperty.h"
#include "RenderInterface.h"
#include "PhysicalDevice.h"
#include "VulkanText.h"

#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <optional>

#include <set>
#include <chrono>
#include <map>
#include <algorithm>
#include <unordered_map>








#ifdef NDEBUG
const bool CenableValidationLayers = false;
#else
const bool CenableValidationLayers = true;
#endif





namespace SEVIAN {
    /*namespace std {
        template<> struct hash<Vertex>
        {
            size_t operator()( Vertex const& vertex ) const {
                return ((hash<glm::vec3> ()(vertex.pos) ^ (hash<glm::vec3> ()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2> ()(vertex.texCoord) << 1);
            }
        };
    }
    */
    static void loadModel ( const std::string path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices );
   

    struct Entity3D: PropertyRender
    {

        VertexBuffer vertex;
        VertexBuffer indices;

        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        std::vector<VkDescriptorSet> descriptorSets;

        size_t indicesSizes;

        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkBuffer indexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
        VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
        std::vector<VulkanUBuffer> ubo;
    };

    const int MAX_FRAMES_IN_FLIGHT = 2;
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback ( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData ) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    

    

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions () {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions ( 4, {} );
        /*    glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 color;*/
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

    class VulkanRenderer : public RenderInterface
    {
    public:

        
        VulkanRenderer ( ) { }
        VulkanRenderer ( GLFWwindow* window, uint32_t width, uint32_t height ) : window ( window ), width ( width ), height ( height ) { }

        GameApp* app;

        std::vector<Texture> textures;
        std::vector<Shader> shaders;
        std::vector<Item> items;

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        const bool enableValidationLayers = CenableValidationLayers;

        void test2 () override;
        void test3 ();
        void initialize ();
        void initialize ( GameApp* _app ) override;
        void renderFrame () override;
        void doLoop () override;
        void cleanup () override;

        //void keyCallback ( GLFWwindow* window, int key, int scancode, int action, int mods )
        bool windowShouldClose () override;
        void test () {
            std::cout << "imprimeindo un Test !\n";
            createInstance ();
            createSurface ();

            physicalDevices = getPhysicalDevices ();



            int position = 0;

            device = physicalDevices.at ( position ).createDevice ();
            commandPool = physicalDevices.at ( position ).createCommandPool ();

            graphicsQueue = physicalDevices.at ( position ).graphicsQueue;
            presentQueue = physicalDevices.at ( position ).presentQueue;

            device->commandPool = commandPool;


            swapChain = physicalDevices.at ( position ).createSwapChain ( window );
            extent = swapChain.extent;
            renderPass = device->createRenderPass ( swapChain.imageFormat );

            depthResources = device->createDepthResources ( extent );

            swapChain.framebuffers = physicalDevices.at ( position ).createFramebuffers ( swapChain, renderPass, { depthResources.imageView } );


            device->renderPass = renderPass;

            descriptorSetLayout = device->createDescriptorSetLayout ();
            descriptorPool = device->createDescriptorPool ();
            device->descriptorSetLayout = descriptorSetLayout;

            std::map<std::string, VulkanTexture> mTextures;

            for (auto& t : textures) {

                auto vt = device->createTexture ( t.source );
                vt->id = "yanny";
                //vkTextures.push_back ( vt );
                //mTextures[t.id] = vt;
            }

            frames.resize ( 2 );

            uint32_t index = 0;
            for (auto& frame : frames) {
                db ( "creando un frame" );
                frame.index = index;
                //device->createUniformBuffers ( frame, sizeof ( UniformBufferObject2 ) );
                device->createSyncObjects ( frame );
                index++;
            }


            device->createCommandBuffers ( frames, device->commandPool );
            fontText.device = device;
            fontText.frames = frames;
            fontText.descriptorPool = descriptorPool;
            fontText.initialize ();

            std::cout << "Hello";
        }
        std::vector<VkDescriptorSet> createDescriptorSets ( std::vector<Frame>& frames, VkDescriptorPool descriptorPool, VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range );
        
        
        std::shared_ptr<PropertyRender> init ( std::vector<Vertex>, std::vector<uint32_t> indices, std::string ) override;
        void addTexture ( std::string name, std::string path) override;
        void addShaders ( std::string, std::string ) override;

        void initEntity ( Info3D info ) override;
        void drawEntity ( uint32_t entityId, glm::vec3 position, Camera ) override;


        void initEntity ( PropertyRender unit ) override;
        void drawEntity ( PropertyRender unit, glm::vec3 position, Camera ) override;

        void beginFrame () override;
        void endFrame () override;

        std::unique_ptr<PropertyRender> createEntity ( Info3D info ) override;
        std::unique_ptr<PropertyRender> createSprite ( Sprite3D info ) override;
        std::unique_ptr<PropertyRender> createModel ( Model3D info ) override;

        void draw ( std::shared_ptr<PropertyRender>, glm::vec3 position, Camera ) override;
        void drawText ( std::string text, glm::vec3 position, Camera camera ) override;
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
        std::string title = "Vulkan 1.0";
        
        
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;

        VkSurfaceKHR surface;
        std::vector <PhysicalDevice>physicalDevices;
        PhysicalDevice* physicalDevice;
        SwapChain  swapChain;
        Device* device;
        VkCommandPool commandPool;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkExtent2D extent;
        VkRenderPass renderPass;
        VulkanDepthResources depthResources;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;

        VkCommandBuffer commandBuffer;

        std::vector<Frame> frames;
        uint32_t currentFrame = 0;
        uint32_t imageIndex;
        bool framebufferResized = false;
        VulkanText fontText;

        std::unordered_map<uint32_t, std::unique_ptr<Entity3D>> entities;

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

        QueueFamilyIndices findQueueFamilies ( VkPhysicalDevice physicalDevice );
        bool checkDeviceExtensionSupport ( VkPhysicalDevice physicalDevice );
        SwapChainSupportDetails querySwapChainSupport ( VkPhysicalDevice physicalDevice );
        bool isDeviceSuitable ( VkPhysicalDevice physicalDevice );
        std::vector <PhysicalDevice> getPhysicalDevices ();
        Pipeline createGraphicsPipeline (
            VkVertexInputBindingDescription bindingDescription,
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
            VkDescriptorSetLayout& descriptorSetLayout );
        //std::map<std::string, VulkanTexture> mTextures;
        std::map<std::string, std::shared_ptr<VulkanTexture>> mTextures;
        
        Pipeline pipeline;
    };

    class VulkanTexture : public TextureInterface
    {
    public:
        const char* id;
        VkImageView imageView;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkSampler sampler;
        void load ( const std::string& path ) override;
        
    };

    class VulkanShader : public ShaderInterface
    {
    public:
        void load ( const std::string& vertexPath, const std::string& fragmentPath ) override;
    };
}


