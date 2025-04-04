#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>

#include "RenderInterface.h"


namespace SEVIAN {
    namespace VULKAN {
        enum class ResourceType
        {
            Descriptor,
            Pushconstant
        };
        struct Resource
        {
            RENDERER::DataResource id = RENDERER::DataResource::NONE;

            virtual ~Resource () = default;
            ResourceType resourceType;

            ResourceType getType () {
                return resourceType;
            }
        };


        struct DescriptorBinding : Resource
        {
            RENDERER::DataResource id = RENDERER::DataResource::NONE;
            VkDescriptorType type;
            uint32_t set;
            uint32_t binding;
            VkShaderStageFlagBits stage;


            DescriptorBinding ( RENDERER::DataResource id, VkDescriptorType type, uint32_t set, uint32_t binding, VkShaderStageFlagBits stage ) :
                id ( id ), type ( type ), set ( set ), binding ( binding ), stage ( stage ) {
            };

        private:
            ResourceType resourceType = ResourceType::Descriptor;
        };
        struct PushConstantInfo : Resource
        {
            RENDERER::DataResource id = RENDERER::DataResource::NONE;
            VkShaderStageFlags stage;
            uint32_t size;
            uint32_t offset;

            PushConstantInfo ( RENDERER::DataResource id, VkShaderStageFlags stage, uint32_t size, uint32_t offset ) :id ( id ), stage ( stage ), size ( size ), offset ( offset ) { };

        private:
            ResourceType resourceType = ResourceType::Pushconstant;
        };

        struct DescriptorRecord
        {
            std::vector<DescriptorBinding> bindings;

            void add ( RENDERER::DataResource id, VkDescriptorType type, uint32_t set, uint32_t binding, VkShaderStageFlagBits stage ) {
                bindings.push_back ( { id, type, set, binding, stage } );
            }
        };


        enum class BindType { UniformBuffer, PushConstant };

        enum class RenderStage : uint32_t
        { // Especificar el tipo subyacente
            Vertex = 1 << 0,  // 1
            Fragment = 1 << 1, // 2
            Geometry = 1 << 2, // 4
            Compute = 1 << 3,  // 8
            // ... otros stages ...
        };
        inline RenderStage operator|( RenderStage a, RenderStage b ) {
            return static_cast<RenderStage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
        }
        

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct VertexBuffer
        {

            VkBuffer buffer = VK_NULL_HANDLE;
            VkDeviceMemory memory = VK_NULL_HANDLE;
        };

        struct VulkanUBuffer
        {
            VkBuffer buffers;
            VkDeviceMemory buffersMemory;
            void* buffersMapped;
        };

        struct FrameData
        {
            std::unordered_map<RENDERER::DataResource, VulkanUBuffer> memoryData;
            std::vector<VkDescriptorSet> descriptorSets;
        };

        struct Element1 : public SEVIAN::RENDERER::iElement
        {
            void* handle;
            bool visible = true;
            bool shadow = true;

            VkPipeline pipeline = VK_NULL_HANDLE;
            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;



            std::vector<VkDescriptorSet> descriptorSets;

            size_t indicesSizes;

            //VkBuffer vertexBuffer = VK_NULL_HANDLE;
            //VkBuffer indexBuffer = VK_NULL_HANDLE;
            VertexBuffer vertexBuffer;
            VertexBuffer indicesBuffer;


            VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;


            VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

            std::vector<VulkanUBuffer> ubo;





        };

        struct Character
        {
            VkImage        textureImage;
            VkDeviceMemory textureImageMemory;
            VkImageView    textureImageView;
            VkSampler      textureSampler;
            int            width;
            int            height;
            int            bearingX;
            int            bearingY;
            long           advance;


        };
        struct Pipeline
        {
            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
            VkPipeline pipeline = VK_NULL_HANDLE;
        };

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete () {
                return graphicsFamily.has_value () && presentFamily.has_value ();
            }
        };

        enum TypeLayout { UNIFORM_BUFFER, IMAGE_SAMPLER };
        
        struct Frame
        {
            VkDescriptorSet descriptorSet;
            VkFence inFlightFences;
            VkSemaphore imageAvailableSemaphores;
            VkSemaphore renderFinishedSemaphores;
            VkCommandBuffer commandBuffer;

            VkBuffer uniformBuffers;
            VkDeviceMemory uniformBuffersMemory;
            void* uniformBuffersMapped;
            const char* name;

            uint32_t index;
        };


        struct VulkanDepthResources
        {
            const char* id;
            VkImageView imageView;
            VkImage image;
            VkDeviceMemory imageMemory;


        };



        struct DescriptorSetLayoutInfo
        {
            VkDescriptorType descriptorType; // Tipo de descriptor
            uint32_t binding;               // Índice de binding
        };
        struct VulkanTexture
        {
            const char* id;
            VkImageView imageView;
            VkImage textureImage;
            VkDeviceMemory textureImageMemory;
            VkSampler sampler;

            int texWidth;
            int texHeight;
            int texChannels;

        };


        struct DescriptorSetDataInfo
        {
            RENDERER::DataResource id = RENDERER::DataResource::NONE;
            uint32_t binding;               // Índice de binding
            VkDescriptorType descriptorType; // Tipo de descriptor
           
            VkDeviceSize bufferSize = 0;             // Tamaño del rango del buffer
            VulkanTexture*  texture;
            //VkImageView imageView = VK_NULL_HANDLE;
            //VkSampler sampler = VK_NULL_HANDLE;
            
            VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        };

        struct DescriptorSetInfo
        {
		
            VkDescriptorType descriptorType; // Tipo de descriptor
            std::vector<VulkanUBuffer>& buffers;
            VkDeviceSize range = 0;             // Tamaño del rango del buffer

            VkImageView imageView = VK_NULL_HANDLE;
            VkSampler sampler = VK_NULL_HANDLE;
            uint32_t binding;               // Índice de binding
            VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        };

        

        struct Vertex2
        {
            glm::vec2 pos;
            glm::vec3 color;
            glm::vec2 texCoord;

        };




        struct DataPipeline
        {

            VkPipelineLayout layout;

            VkPipeline pipeline;
        };

        struct VertexText
        {
            glm::vec2 pos;
            glm::vec2 texCoord;
            int color = 1;

            static VkVertexInputBindingDescription getBindingDescription () {
                VkVertexInputBindingDescription bindingDescription {};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof ( VertexText );
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions () {

                std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};

                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[0].offset = offsetof ( VertexText, pos );

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[1].offset = offsetof ( VertexText, texCoord );

                attributeDescriptions[2].binding = 0;
                attributeDescriptions[2].location = 2;
                attributeDescriptions[2].format = VK_FORMAT_R8_SINT;
                attributeDescriptions[2].offset = offsetof ( VertexText, color );

                return attributeDescriptions;

            }
        };


        struct Propertys
        {
            VertexBuffer vertex;
            VertexBuffer indices;

            VkPipeline pipeline = VK_NULL_HANDLE;
            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

            VkPipeline shadowPipeline = VK_NULL_HANDLE;
            VkPipelineLayout shadowPipelineLayout = VK_NULL_HANDLE;

            std::vector<VkDescriptorSet> descriptorSets;

            size_t indicesSizes;

            VkBuffer vertexBuffer = VK_NULL_HANDLE;
            VkBuffer indexBuffer = VK_NULL_HANDLE;
            VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
            VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
            std::vector<VulkanUBuffer> buffers;


            VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;
            VkDescriptorSetLayout texDescriptorSetLayout = VK_NULL_HANDLE;
            VkDescriptorSetLayout shadowDescriptorSetLayout = VK_NULL_HANDLE;
            //VkPipelineLayout shadowPipelineLayout = VK_NULL_HANDLE;
        };




        struct QVertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
            glm::vec3 color;




            static VkVertexInputBindingDescription getBindingDescription () {
                VkVertexInputBindingDescription bindingDescription {};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof ( QVertex );
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return bindingDescription;
            }

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions () {

                std::vector<VkVertexInputAttributeDescription> attributeDescriptions {};
                attributeDescriptions.resize ( 4 );
                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof ( QVertex, position );

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof ( QVertex, normal );

                attributeDescriptions[2].binding = 0;
                attributeDescriptions[2].location = 2;
                attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[2].offset = offsetof ( QVertex, texCoords );

                attributeDescriptions[3].binding = 0;
                attributeDescriptions[3].location = 3;
                attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[3].offset = offsetof ( QVertex, color );


                return attributeDescriptions;

            }

            bool operator==( const QVertex& other ) const {
                return position == other.position && color == other.color && texCoords == other.texCoords;
            }

        };




    }
}
namespace SEVIAN {
    struct UniformBufferObject2
    {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class Texture
    {
    public:
        const char* id;
        const char* source;
        const char* format;
    };

    class Shader
    {
    public:
        const char* id;
        const char* source;
        const char* type;
        const char* entry;
        std::vector<int> layout;
    };

    class Item
    {
    public:
        const char* id;
        //std::vector<Data3D> data;
        std::vector<uint32_t> indices;
        const char* texture;

        std::vector<const char*> shaders;
    };

    


}
