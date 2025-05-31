#include "ResourceData.h"
namespace SEVIAN::VULKAN {
	ResourceData::ResourceData ( Device* device ) : device ( device )
	{
	}
	void SEVIAN::VULKAN::ResourceData::addResourceData ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage )
	{

		if (descriptorInfo.size () <= set) {
			descriptorInfo.resize ( (size_t) set + 1 ); // Redimensionar si es necesario
		}

		// Crear el DescriptorBinding
		auto descriptorBinding = std::make_unique<DescriptorBinding> ( resourceType, type, set, binding, stage );

		// Obtener un puntero crudo del unique_ptr para almacenar en descriptorInfo
		DescriptorBinding* descriptorPtr = descriptorBinding.get ();

		// Almacenar el unique_ptr en _resources (para gestión de memoria)
		resources.emplace ( resourceType, std::move ( descriptorBinding ) );

		// Agregar el puntero crudo al vector correcto dentro de descriptorInfo según el 'set'
		descriptorInfo[set].push_back ( descriptorPtr );
	}

	void ResourceData::addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset )
	{
		auto pushConstant = std::make_unique<PushConstantInfo> ( resourceType, stage, size, offset );
		PushConstantInfo* pushConstantPtr = pushConstant.get ();
		resources.emplace ( resourceType, std::move ( pushConstant ) );
		pushConstantsInfo.push_back ( pushConstantPtr );
	}

	void ResourceData::doDescriptorSets ( std::shared_ptr<VKElement>& element, VkDescriptorPool descriptorPool )
	{
		
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		auto texture = device->createTexture ( "textures/aTravel.jpeg" );
		auto texture_ptr = texture.get ();

		for (auto& descriptor : descriptorInfo) {
			std::vector < DescriptorSetDataInfo > dataInfo;
			for (auto& _info : descriptor) {
				if (_info->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
					dataInfo.push_back ( { _info->id, _info->binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, sizeof ( UniformBufferObject ), nullptr, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL } );
				}
				else if (_info->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
					dataInfo.push_back ( { _info->id, _info->binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, texture_ptr, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL } );
				}
			}
			createDescriptorSets ( element->frames, descriptorPool, descriptorSetLayout, dataInfo );

			
		}
	}

	void ResourceData::init ()
	{
		for (const auto& descriptor : descriptorInfo) {
			descriptorSetLayout = device->createDescriptorSetLayout ( descriptor );
			layouts.push_back ( descriptorSetLayout );

		}
		auto pushConstants = device->createPushConstantRange ( pushConstantsInfo );
		pipelineLayout = device->createPipelineLayout ( layouts, pushConstants );
	}

	void ResourceData::createDescriptorSets ( std::vector<FrameData>& frames, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetDataInfo> info )
	{
		auto framesCount = MAX_FRAMES_IN_FLIGHT;



		std::vector<VkDescriptorSet> descriptorSets;
		descriptorSets.resize ( framesCount );

		std::vector<VkDescriptorSetLayout> layouts ( framesCount, descriptorSetLayout );

		VkDescriptorSetAllocateInfo allocInfo {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(framesCount);
		allocInfo.pSetLayouts = layouts.data ();

		if (vkAllocateDescriptorSets ( device->device, &allocInfo, descriptorSets.data () ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to allocate descriptor sets!" );
		}

		for (size_t i = 0; i < framesCount; i++) {


			frames[i].descriptorSets.push_back ( descriptorSets[i] );

			std::vector<VkWriteDescriptorSet> descriptorWrites;

			std::vector<VkDescriptorBufferInfo> bufferInfo ( info.size () );
			std::vector<VkDescriptorImageInfo> imageInfo ( info.size () );

			size_t j = 0, k = 0;

			for (const auto& _info : info) {



				VkWriteDescriptorSet write {};

				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.dstSet = descriptorSets[i];
				write.dstBinding = _info.binding;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;
				write.descriptorType = _info.descriptorType;
				if (_info.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
					frames[i].memoryData[_info.id] = device->createUniform ( _info.bufferSize );

					bufferInfo[j].buffer = frames[i].memoryData[_info.id].buffers;// _info.buffers[i].buffers;
					bufferInfo[j].offset = 0;
					bufferInfo[j].range = _info.bufferSize;
					write.pBufferInfo = &bufferInfo[j];
					j++;

				}
				else if (_info.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {

					imageInfo[k].imageLayout = _info.imageLayout; //VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo[k].imageView = _info.texture->imageView;
					imageInfo[k].sampler = _info.texture->sampler;
					write.pImageInfo = &imageInfo[k];
					k++;

				}
				descriptorWrites.push_back ( write );

			}
			vkUpdateDescriptorSets ( device->device, static_cast<uint32_t>(descriptorWrites.size ()), descriptorWrites.data (), 0, nullptr );
		}
	}

	

}

