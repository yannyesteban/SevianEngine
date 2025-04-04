#include "WidgetRenderer.h"


namespace SEVIAN::VULKAN {
	WidgetDraw::WidgetDraw () {

	}
	void WidgetDraw::render ( VkCommandBuffer commandBuffer, uint32_t swapchainImageIndex, Camera2D camera ) {




		//updateUniformBuffer1 ( element->ubo[frameIndex].buffersMapped, glm::vec3 ( 0.0f ), width, height );
		memcpy ( ubo[swapchainImageIndex].buffersMapped, &camera, sizeof ( camera ) );
		VkBuffer vertexBuffers[] = { vertexBuffer.buffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );
		vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
		//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindIndexBuffer ( commandBuffer, indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[swapchainImageIndex], 0, nullptr );
		vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indicesSizes), 1, 0, 0, 0 );
	}


	WidgetRenderer::WidgetRenderer ( VulkanInfo vulkan ) :device ( vulkan.device ), descriptorPool ( vulkan.descriptorPool ), frames ( vulkan.frames ) {
		/*setDescriptorSetLayout ();
		createPipelineLayout ();
		setGraphPipeline ();*/
	}

	void WidgetRenderer::createUniformBuffer ( RENDERER::DataResource resourceId, std::vector<FrameData>& frames, VkDeviceSize bufferSize ) {

		std::vector<VulkanUBuffer> buffers;
		buffers.resize ( frames.size () );
		for (size_t i = 0; i < frames.size (); i++) {
			device->createBuffer ( bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, frames[i].memoryData[resourceId].buffers, frames[i].memoryData[resourceId].buffersMemory);

			vkMapMemory ( device->device, frames[i].memoryData[resourceId].buffersMemory, 0, bufferSize, 0, &frames[i].memoryData[resourceId].buffersMapped );
		}
		
	}

	std::unique_ptr<RENDERER::IRenderizable> WidgetRenderer::createSprite ( SEVIAN::SpriteInfo info ) {

		auto element = std::make_unique<VKElement> ();

		auto color = info.color;

		auto one = 1.0f;
		auto zero = 0.0f;

		std::vector<QVertex> vertices = {
			{{ zero, one, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, color}, // Bottom-left
			{{ one , one, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, color}, // Bottom-right
			{{ one, zero, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, color}, // Top-right
			{{ zero, zero, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, color}  // Top-left
		};

		std::vector<uint32_t> indices = {
			0, 2, 3,
			0, 1, 2
		};

		element->frames.resize ( MAX_FRAMES_IN_FLIGHT );
		auto texture = device->createTexture ( "textures/aTravel.jpeg" );
		auto texture_ptr = texture.get ();
		for (auto& descriptor : descriptorInfo) {

			std::vector < DescriptorSetDataInfo > dataInfo;
			for (auto& _info : descriptor) {
				if (_info->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { 
					dataInfo.push_back ( { _info->id, _info->binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, sizeof ( UniformBufferObject ), nullptr, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
				}
				else if (_info->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
					dataInfo.push_back ( { _info->id, _info->binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, texture_ptr, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
				}

			}

			createDescriptorSets ( element->frames, descriptorSetLayout, dataInfo );
			
			
			
		}
		element->test = "yanny esteban";
		std::cout << element->test << std::endl;

		//std::vector<::VULKAN::VulkanUBuffer> ubo = vulkan.device->createUniformBuffer ( vulkan.device->frames, sizeof ( UniformBufferObject ) );
		
		element->pipeline = pipeline;
	
		element->pipelineLayout = pipelineLayout;


		element->vertexBuffer = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		element->indicesBuffer = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );

		element->indicesSizes = indices.size ();


		element->pushConstantsInfo = pushConstantsInfo;
		return element;

		//return
	}

	void WidgetRenderer::createDescriptorSets ( std::vector<FrameData>& frames, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetDataInfo> info ) {
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


			frames[i].descriptorSets.push_back(descriptorSets[i]);

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
					frames[i].memoryData[_info.id] = device->createUniform (_info.bufferSize);

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



	VkDescriptorSetLayout WidgetRenderer::createDescriptorSetLayout ( std::vector<DescriptorBinding*> info ) {
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


	void WidgetRenderer::setDescriptorSetLayout () {

		std::vector<DescriptorSetLayoutInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );


	}

	std::vector<VkPushConstantRange> WidgetRenderer::createPushConstantRange ( std::vector<PushConstantInfo*> pushConstantsInfo ) {
		
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

	VkPipelineLayout WidgetRenderer::createPipelineLayout ( std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstantRangesVector ) {

		VkPipelineLayout pipelineLayout{};


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

	void WidgetRenderer::setGraphPipeline () {

		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
		pipelineLayout = device->createPipelineLayout ( layouts );
		pipeline = createGraphPipeline (
			QVertex::getBindingDescription (),
			QVertex::getAttributeDescriptions (),
			pipelineLayout,
			"shaders/WidgetBasic.vert.spv",
			"shaders/WidgetBasic.frag.spv"
		);

	}

	VkPipeline WidgetRenderer::createGraphPipeline (


		VkVertexInputBindingDescription bindingDescription,
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
		VkPipelineLayout pipelineLayout,
		std::string vertSource,
		std::string fragSource ) {


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
	
}






