#include "RenderEngine.h"

void SEVIAN::VULKAN::RenderEngine::add ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage )
{
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

void SEVIAN::VULKAN::RenderEngine::addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset )
{
	auto pushConstant = std::make_unique<PushConstantInfo> ( resourceType, stage, size, offset );
	PushConstantInfo* pushConstantPtr = pushConstant.get ();
	_resources.emplace ( resourceType, std::move ( pushConstant ) );
	pushConstantsInfo.push_back ( pushConstantPtr );
}

VkDescriptorSetLayout SEVIAN::VULKAN::RenderEngine::createDescriptorSetLayout ( std::vector<DescriptorBinding*> info )
{
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

std::vector<VkPushConstantRange> SEVIAN::VULKAN::RenderEngine::createPushConstantRange ( std::vector<PushConstantInfo*> pushConstantsInfo )
{
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

VkPipelineLayout SEVIAN::VULKAN::RenderEngine::createPipelineLayout ( std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstantRangesVector )
{
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

VkPipeline SEVIAN::VULKAN::RenderEngine::createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource )
{
	Pipeline pipe {};
	auto vertShaderCode = device->readFile ( vertSource );
	auto fragShaderCode = device->readFile ( fragSource );

	VkShaderModule vertShaderModule = device->createShaderModule ( vertShaderCode );
	VkShaderModule fragShaderModule = device->createShaderModule ( fragShaderCode );

	VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size ());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data ();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	//VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

	// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;

	/*Ojo aqui*/

	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	//rasterizer.cullMode = VK_CULL_MODE_NONE;

	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;//VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;




	VkPipelineMultisampleStateCreateInfo multisampling {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencil {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment {};
	//colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	//colorBlendAttachment.blendEnable = VK_FALSE;

	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;


	VkPipelineColorBlendStateCreateInfo colorBlending {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size ());
	dynamicState.pDynamicStates = dynamicStates.data ();


	VkGraphicsPipelineCreateInfo pipelineInfo {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = device->renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline pipeline1;
	if (vkCreateGraphicsPipelines ( device->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline1 ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create graphics pipeline!" );
	}

	vkDestroyShaderModule ( device->device, fragShaderModule, nullptr );
	vkDestroyShaderModule ( device->device, vertShaderModule, nullptr );





	return pipeline1;
}

void SEVIAN::VULKAN::RenderEngine::createDescriptorSets ( std::vector<FrameData>& frames, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetDataInfo> info )
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
