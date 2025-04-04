#include "VulkanRendererManager.h"
namespace SEVIAN::VULKAN {
	
	void VulkanRendererManager::add ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage ) {

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

	void VulkanRendererManager::addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset ) {

		auto pushConstant = std::make_unique<PushConstantInfo> ( resourceType, stage, size, offset );
		PushConstantInfo* pushConstantPtr = pushConstant.get ();
		_resources.emplace ( resourceType, std::move ( pushConstant ) );
		pushConstantsInfo.push_back ( pushConstantPtr );
	}

	VkDescriptorSetLayout VulkanRendererManager::createDescriptorSetLayout ( std::vector<DescriptorBinding*> info ) {
		VkDescriptorSetLayout descriptorSetLayout;

		std::vector<VkDescriptorSetLayoutBinding> bindings = {  };

		for (const auto& _info : info) {
			VkDescriptorSetLayoutBinding layoutBinding {};
			layoutBinding.binding = _info->binding;
			layoutBinding.descriptorCount = 1;
			layoutBinding.descriptorType = _info->type;
			layoutBinding.pImmutableSamplers = nullptr;
			layoutBinding.stageFlags = _info->stage;
			bindings.push_back ( layoutBinding );
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo {};

		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size ());
		layoutInfo.pBindings = bindings.data ();

		if (vkCreateDescriptorSetLayout ( device->device, &layoutInfo, nullptr, &descriptorSetLayout ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create descriptor set layout!" );
		}

		return descriptorSetLayout;
	}

	std::vector<VkPushConstantRange> VulkanRendererManager::createPushConstantRange ( std::vector<PushConstantInfo*> pushConstantsInfo ) {
		std::vector<VkPushConstantRange> pushConstantRangesVector;
		uint32_t currentOffset = 0;

		for (const auto& info : pushConstantsInfo) {
			VkPushConstantRange pushConstantRange {};
			pushConstantRange.stageFlags = info->stage;
			pushConstantRange.offset = currentOffset;
			pushConstantRange.size = info->size;

			pushConstantRangesVector.push_back ( pushConstantRange );
			currentOffset += info->size;
		}
		return pushConstantRangesVector;
	}

	void VulkanRendererManager::createUniformBuffer ( RENDERER::DataResource resourceId, std::vector<FrameData>& frames, VkDeviceSize bufferSize ) {

		std::vector<VulkanUBuffer> buffers;
		buffers.resize ( frames.size () );
		for (size_t i = 0; i < frames.size (); i++) {
			device->createBuffer ( bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, frames[i].memoryData[resourceId].buffers, frames[i].memoryData[resourceId].buffersMemory );

			vkMapMemory ( device->device, frames[i].memoryData[resourceId].buffersMemory, 0, bufferSize, 0, &frames[i].memoryData[resourceId].buffersMapped );
		}
	}

	VkPipelineLayout VulkanRendererManager::createPipelineLayout ( std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstantRangesVector ) {
		VkPipelineLayout pipelineLayout {};


		VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size ());
		pipelineLayoutInfo.pSetLayouts = layouts.data ();

		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRangesVector.size ());
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRangesVector.data ();


		if (vkCreatePipelineLayout ( device->device, &pipelineLayoutInfo, nullptr, &pipelineLayout ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create pipeline layout!" );
		}

		return pipelineLayout;
	}


}

