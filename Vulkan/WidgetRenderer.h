#pragma once
#include "RenderInterface.h"
#include "Device.h"
//#include "VKElement.h"
#include "Renderer.h"
#include "VulkanRendererManager.h"


namespace SEVIAN {

	struct Zoom
	{
		int value = 0;
	};



	struct PushData : RENDERER::IMemoryData
	{
		void* data = nullptr;
	};
	struct UboData : RENDERER::IMemoryData
	{

		VkBuffer buffers;
		VkDeviceMemory buffersMemory;
		void* buffersMapped;
	};
	namespace VULKAN {

		




		class WidgetDraw : public VKElement
		{
			// Heredado vía iSpriteManager
		public:

			VkPipeline pipeline = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;



			std::vector<VkDescriptorSet> descriptorSets;

			size_t indicesSizes = 0;

			//VkBuffer vertexBuffer = VK_NULL_HANDLE;
			//VkBuffer indexBuffer = VK_NULL_HANDLE;
			VertexBuffer vertexBuffer {};
			VertexBuffer indicesBuffer {};


			VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;


			VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

			std::vector<VulkanUBuffer> ubo;
		public:
			WidgetDraw ();
			void render ( VkCommandBuffer commandBuffer, uint32_t swapchainImageIndex, Camera2D camera );

		};


		class WidgetRenderer : public RENDERER::iWidgetManager, public VulkanRendererManager
		{
			// Heredado vía iSpriteManager

			using RenderResource = std::unordered_map<RENDERER::DataResource, std::shared_ptr<DescriptorBinding>>;
		private:
			//VulkanInfo vulkan;
			Device* device;
			std::vector<Frame> frames;
			VkDescriptorPool descriptorPool;
			std::vector<VkDescriptorSetLayout> layouts;
			std::shared_ptr<VulkanRenderer> renderer;


			std::unordered_map<RENDERER::DataResource, std::shared_ptr<DescriptorBinding>> resourcesData;

			std::vector<RenderResource> resources;

			VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkPipeline pipeline = VK_NULL_HANDLE;

			//std::vector<VulkanUBuffer> ubo;
			std::vector<VulkanUBuffer> ubo;

			//VertexBuffer vertexBuffer;
			//VertexBuffer indicesBuffer;
			size_t indicesSizes;

			std::vector<VkDescriptorSet> descriptorSets;
			void setDescriptorSetLayout ();
			std::vector<VkPushConstantRange> createPushConstantRange ( std::vector<PushConstantInfo*> pushConstantsInfo );
			VkPipelineLayout createPipelineLayout ( std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstantRangesVector );

			void setGraphPipeline ();


			VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );

			std::vector<DescriptorBinding*> descriptorBindingInfo;
			std::vector<PushConstantInfo*> pushConstantsInfo;


			std::vector<std::vector<DescriptorBinding*>> descriptorInfo;

			std::unordered_map<RENDERER::DataResource, std::unique_ptr<Resource>> _resources;


			std::vector< std::unordered_map< RENDERER::DataResource, std::unique_ptr<DescriptorBinding> > > descriptors;

			std::vector<std::unique_ptr<DescriptorRecord>> descriptorRecords;
		public:
			void addDescriptor ( std::vector<std::pair<RENDERER::DataResource, std::unique_ptr<Resource>>> resources ) {
				for (auto& resource : resources) {
					_resources.emplace ( resource.first, std::move ( resource.second ) );
				}
			}
			void addResourceInfo ( RENDERER::DataResource resource, std::shared_ptr<DescriptorBinding> dataInfo ) {

				resourcesData[resource] = dataInfo;
			}

			DescriptorBinding* getResourceInfo ( RENDERER::DataResource resource ) {
				auto it = resourcesData.find ( resource );
				return (it != resourcesData.end ()) ? it->second.get () : nullptr;
			}

			void add ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage ) {

				if (descriptorInfo.size () <= set) {
					descriptorInfo.resize ( (size_t) set + 1 ); // Redimensionar si es necesario
				}

				// Crear el DescriptorBinding
				auto descriptorBinding = std::make_unique<DescriptorBinding> ( resourceType, type, set, binding, stage );

				// Obtener un puntero crudo del unique_ptr para almacenar en descriptorInfo
				DescriptorBinding* descriptorPtr = descriptorBinding.get ();

				// Almacenar el unique_ptr en _resources (para gestión de memoria)
				_resources.emplace ( resourceType, std::move ( descriptorBinding ) );

				// Agregar el puntero crudo al vector correcto dentro de descriptorInfo según el 'set'
				descriptorInfo[set].push_back ( descriptorPtr );

			}

			void addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset ) {

				auto pushConstant = std::make_unique<PushConstantInfo> ( resourceType, stage, size, offset );
				PushConstantInfo* pushConstantPtr = pushConstant.get ();
				_resources.emplace ( resourceType, std::move ( pushConstant ) );
				pushConstantsInfo.push_back ( pushConstantPtr );
			}


			
			void createUniformBuffer ( RENDERER::DataResource resourceId, std::vector<FrameData>& frames, VkDeviceSize bufferSize );
			
			WidgetRenderer ( VulkanInfo vulkan );

			WidgetRenderer ( std::shared_ptr<VulkanRenderer> renderer ) :device ( renderer->device ), descriptorPool ( renderer->descriptorPool ), frames ( renderer->frames ) {


				add ( RENDERER::DataResource::TRANSFORM, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, VK_SHADER_STAGE_VERTEX_BIT );
				add ( RENDERER::DataResource::TEXTURE, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT );
				addPushConstant ( RENDERER::DataResource::STYLE, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof ( StyleUbo ), 0 );
				//addPushConstant ( RENDERER::DataResource::STYLE, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof ( glm::vec4 ), 0 );
				//addPushConstant ( RENDERER::DataResource::USE_TEXTURE,  VK_SHADER_STAGE_VERTEX_BIT, sizeof ( int ), sizeof ( glm::vec4 ) );

				for (const auto& descriptor : descriptorInfo) {
					descriptorSetLayout = createDescriptorSetLayout ( descriptor );
					layouts.push_back ( descriptorSetLayout );

				}
				auto pushConstants = createPushConstantRange ( pushConstantsInfo );
				pipelineLayout = createPipelineLayout ( layouts, pushConstants );

				pipeline = createGraphPipeline (
					QVertex::getBindingDescription (),
					QVertex::getAttributeDescriptions (),
					pipelineLayout,
					"shaders/WidgetBasic.vert.spv",
					"shaders/WidgetBasic.frag.spv"
				);


			};

			std::unique_ptr<RENDERER::IRenderizable> createSprite ( SEVIAN::SpriteInfo info ) override;

			void createDescriptorSets ( std::vector<FrameData>& frames, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetDataInfo> info );




			VkDescriptorSetLayout createDescriptorSetLayout ( std::vector<DescriptorBinding*> info );

		};
	}

}



