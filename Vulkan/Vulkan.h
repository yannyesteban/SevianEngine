#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>



namespace VULKAN {

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

    struct VulkanUBuffer
    {
        VkBuffer buffers;
        VkDeviceMemory buffersMemory;
        void* buffersMapped;
    };

    struct DSLInfo
    {
        VkDescriptorType descriptorType; // Tipo de descriptor
        uint32_t binding;               // Índice de binding
    };

    struct DSInfo
    {
        VkDescriptorType descriptorType; // Tipo de descriptor
        std::vector<VulkanUBuffer>& buffers;
        VkDeviceSize range = 0;             // Tamaño del rango del buffer

        VkImageView imageView = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        uint32_t binding;               // Índice de binding
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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

    struct Vertex2
    {
        glm::vec2 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

    };


    struct VertexBuffer
    {

        VkBuffer buffer;
        VkDeviceMemory memory;
    };

    struct DataPipeline
    {

        VkPipelineLayout layout;

        VkPipeline pipeline;
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

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };


}
