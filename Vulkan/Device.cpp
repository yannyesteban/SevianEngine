#include "Device.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void db ( std::string message ) {
	times++;
	std::cout << times << " : " << message << "\n";
}


Device::Device ( VkPhysicalDevice pPhysicalDevice, VkDevice pDevice, VkQueue pGraphicsQueue ) {
	device = pDevice;
	physicalDevice = pPhysicalDevice;
	graphicsQueue = pGraphicsQueue;
}

std::vector<char> Device::readFile ( const std::string& filename ) {
	std::ifstream file ( filename, std::ios::ate | std::ios::binary );

	if (!file.is_open ()) {
		throw std::runtime_error ( "failed to open file!" );
	}

	size_t fileSize = (size_t) file.tellg ();
	std::vector<char> buffer ( fileSize );

	file.seekg ( 0 );
	file.read ( buffer.data (), fileSize );

	file.close ();

	return buffer;
}

VkShaderModule Device::createShaderModule ( const std::vector<char>& code ) {
	VkShaderModuleCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size ();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data ());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule ( device, &createInfo, nullptr, &shaderModule ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create shader module!" );
	}

	return shaderModule;
}

VkDescriptorSetLayout Device::createDescriptorSetLayout () {

	VkDescriptorSetLayout descriptorSetLayout1;

	VkDescriptorSetLayoutBinding uboLayoutBinding {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


	VkDescriptorSetLayoutBinding uboLayoutBinding2 {};
	uboLayoutBinding2.binding = 1;
	uboLayoutBinding2.descriptorCount = 1;
	uboLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding2.pImmutableSamplers = nullptr;
	uboLayoutBinding2.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;


	VkDescriptorSetLayoutBinding samplerLayoutBinding {};
	samplerLayoutBinding.binding = 2;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding uboLayoutBinding3 {};
	uboLayoutBinding3.binding = 3;
	uboLayoutBinding3.descriptorCount = 1;
	uboLayoutBinding3.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding3.pImmutableSamplers = nullptr;
	uboLayoutBinding3.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	//std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding2, samplerLayoutBinding };
	//VkDescriptorSetLayoutCreateInfo layoutInfo {};
	/*
	VkDescriptorSetLayoutBinding uboLayoutBinding3 {};
	uboLayoutBinding3.binding = 3;
	uboLayoutBinding3.descriptorCount = 1;
	uboLayoutBinding3.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding3.pImmutableSamplers = nullptr;
	uboLayoutBinding3.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	*/

	std::array<VkDescriptorSetLayoutBinding, 4> bindings = { uboLayoutBinding, uboLayoutBinding2, samplerLayoutBinding,uboLayoutBinding3 };
	VkDescriptorSetLayoutCreateInfo layoutInfo {};


	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size ());
	layoutInfo.pBindings = bindings.data ();
	printf ( "que es un device [%p]\n", device );
	if (vkCreateDescriptorSetLayout ( device, &layoutInfo, nullptr, &descriptorSetLayout1 ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create descriptor set layout!" );
	}

	return descriptorSetLayout1;
}
std::unique_ptr<VulkanTexture> Device::createTexture ( const char* src ) {
	//VulkanTexture Device::createTexture ( const char* src ) {

	//VulkanTexture texture;
	auto texture = std::make_unique<VulkanTexture> ();

	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load ( src, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	std::cout << "height " << texWidth << " height " << texHeight << "\n";

	if (!pixels) {
		throw std::runtime_error ( "failed to load texture image!" );
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer ( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

	void* data;
	vkMapMemory ( device, stagingBufferMemory, 0, imageSize, 0, &data );
	memcpy ( data, pixels, static_cast<size_t>(imageSize) );
	vkUnmapMemory ( device, stagingBufferMemory );

	stbi_image_free ( pixels );

	createImage ( texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture->textureImage, texture->textureImageMemory );

	transitionImageLayout ( texture->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
	copyBufferToImage ( stagingBuffer, texture->textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
	transitionImageLayout ( texture->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

	vkDestroyBuffer ( device, stagingBuffer, nullptr );
	vkFreeMemory ( device, stagingBufferMemory, nullptr );
	textureImageView = createImageView ( texture->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
	texture->sampler = createTextureSampler ();
	texture->imageView = textureImageView;

	return texture;
}

VkDescriptorPool Device::createDescriptorPool () {
	//VkDescriptorPool descriptorPool;

	uint32_t MAX_FRAMES_IN_FLIGHT = 2;
	std::array<VkDescriptorPoolSize, 2> poolSizes {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 100;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //1000;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 100;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //1000;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	//poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //1000;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size ());
	poolInfo.pPoolSizes = poolSizes.data ();
	poolInfo.maxSets = 200;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //1000;// static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool ( device, &poolInfo, nullptr, &descriptorPool ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create descriptor pool!" );
	}

	return descriptorPool;
}


void Device::createUniformBuffers ( Frame& frame, VkDeviceSize bufferSize ) {
	//VkDeviceSize bufferSize = sizeof(UniformBufferObject);


	createBuffer ( bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, frame.uniformBuffers, frame.uniformBuffersMemory );

	vkMapMemory ( device, frame.uniformBuffersMemory, 0, bufferSize, 0, &frame.uniformBuffersMapped );

}


std::vector<VulkanUBuffer> Device::createUniformBuffer ( std::vector<Frame>& frames, VkDeviceSize bufferSize ) {

	std::vector<VulkanUBuffer> buffers;
	buffers.resize ( frames.size () );

	for (auto& frame : frames) {

		createBuffer ( bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffers[frame.index].buffers, buffers[frame.index].buffersMemory );

		vkMapMemory ( device, buffers[frame.index].buffersMemory, 0, bufferSize, 0, &buffers[frame.index].buffersMapped );



	}

	return buffers;
}

void Device::createCommandBuffers ( std::vector<Frame>& frames, VkCommandPool commandPool ) {
	std::vector<VkCommandBuffer> commandBuffers;
	commandBuffers.resize ( frames.size () );

	VkCommandBufferAllocateInfo allocInfo {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) commandBuffers.size ();
	frames[0].name = "yanny";
	frames[1].name = "esteban";
	if (vkAllocateCommandBuffers ( device, &allocInfo, commandBuffers.data () ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to allocate command buffers!" );
	}


	for (size_t i = 0; i < frames.size (); i++) {
		frames[i].commandBuffers = commandBuffers[i];
	}
}

void Device::createSyncObjects ( Frame& frame ) {
	//imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	//renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	//inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


	if (vkCreateSemaphore ( device, &semaphoreInfo, nullptr, &frame.imageAvailableSemaphores ) != VK_SUCCESS ||
		vkCreateSemaphore ( device, &semaphoreInfo, nullptr, &frame.renderFinishedSemaphores ) != VK_SUCCESS ||
		vkCreateFence ( device, &fenceInfo, nullptr, &frame.inFlightFences ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create synchronization objects for a frame!" );
	}

}

VkPipeline Device::createGraphicsPipeline ( VkVertexInputBindingDescription bindingDescription,
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkDescriptorSetLayout& descriptorSetLayout ) {
	//auto vertShaderCode = readFile("shaders/vert2.spv");
	//auto fragShaderCode = readFile("shaders/frag2.spv");

	auto vertShaderCode = readFile ( "shaders/solid_vert.spv" );
	auto fragShaderCode = readFile ( "shaders/solid_frag.spv" );


	VkShaderModule vertShaderModule = createShaderModule ( vertShaderCode );
	VkShaderModule fragShaderModule = createShaderModule ( fragShaderCode );

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

	//auto bindingDescription = Vertex::getBindingDescription();



	//auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size ());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data ();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
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
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencil {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

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

	VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout ( device, &pipelineLayoutInfo, nullptr, &pipelineLayout ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create pipeline layout!" );
	}

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
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	VkPipeline pipeline;
	if (vkCreateGraphicsPipelines ( device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create graphics pipeline!" );
	}

	vkDestroyShaderModule ( device, fragShaderModule, nullptr );
	vkDestroyShaderModule ( device, vertShaderModule, nullptr );

	return pipeline;
}

void Device::createTextureImage () {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load ( "textures/a.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error ( "failed to load texture image!" );
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer ( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

	void* data;
	vkMapMemory ( device, stagingBufferMemory, 0, imageSize, 0, &data );
	memcpy ( data, pixels, static_cast<size_t>(imageSize) );
	vkUnmapMemory ( device, stagingBufferMemory );

	stbi_image_free ( pixels );

	createImage ( texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory );

	transitionImageLayout ( textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
	copyBufferToImage ( stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
	transitionImageLayout ( textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

	vkDestroyBuffer ( device, stagingBuffer, nullptr );
	vkFreeMemory ( device, stagingBufferMemory, nullptr );

}
void Device::createBuffer ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory ) {
	VkBufferCreateInfo bufferInfo {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer ( device, &bufferInfo, nullptr, &buffer ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create buffer!" );
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements ( device, buffer, &memRequirements );

	VkMemoryAllocateInfo allocInfo {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType ( memRequirements.memoryTypeBits, properties );

	if (vkAllocateMemory ( device, &allocInfo, nullptr, &bufferMemory ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to allocate buffer memory!" );
	}

	vkBindBufferMemory ( device, buffer, bufferMemory, 0 );
}



std::vector<VkDescriptorSet> Device::createDescriptorSets ( std::vector<VulkanUBuffer>& buffers, VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range ) {
	auto framesCount = MAX_FRAMES_IN_FLIGHT;

	std::vector<VkDescriptorSet> descriptorSets;
	descriptorSets.resize ( framesCount );

	std::vector<VkDescriptorSetLayout> layouts ( framesCount, descriptorSetLayout );

	VkDescriptorSetAllocateInfo allocInfo {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(framesCount);
	//static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data ();

	if (vkAllocateDescriptorSets ( device, &allocInfo, descriptorSets.data () ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to allocate descriptor sets!" );
	}

	for (size_t i = 0; i < framesCount; i++) {
		//frames[i].descriptorSet = descriptorSets[i];

		VkDescriptorBufferInfo bufferInfo {};
		bufferInfo.buffer = buffers[i].buffers;// uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = range;// sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets ( device, static_cast<uint32_t>(descriptorWrites.size ()), descriptorWrites.data (), 0, nullptr );

	}

	return descriptorSets;
}



std::vector<VkDescriptorSet> Device::createDescriptorSets ( std::vector<Frame>& frames, VkDescriptorPool descriptorPool, VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range ) {

	std::vector<VkDescriptorSet> descriptorSets;

	auto framesCount = frames.size ();

	std::vector<VkDescriptorSetLayout> layouts ( framesCount, descriptorSetLayout );

	VkDescriptorSetAllocateInfo allocInfo {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(framesCount);
	//static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data ();

	descriptorSets.resize ( framesCount );
	if (vkAllocateDescriptorSets ( device, &allocInfo, descriptorSets.data () ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to allocate descriptor sets!" );
	}

	for (size_t i = 0; i < framesCount; i++) {
		//frames[i].descriptorSet = descriptorSets[i];

		VkDescriptorBufferInfo bufferInfo {};
		bufferInfo.buffer = frames[i].uniformBuffers;// uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = range;// sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets ( device, static_cast<uint32_t>(descriptorWrites.size ()), descriptorWrites.data (), 0, nullptr );


	}

	return descriptorSets;
}

std::vector<VkDescriptorSet> Device::createDescriptorSets ( std::vector <BufferInfo> info ) {
	auto framesCount = MAX_FRAMES_IN_FLIGHT;

	std::vector<VkDescriptorSet> descriptorSets;
	descriptorSets.resize ( framesCount );

	std::vector<VkDescriptorSetLayout> layouts ( framesCount, descriptorSetLayout );

	VkDescriptorSetAllocateInfo allocInfo {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(framesCount);
	allocInfo.pSetLayouts = layouts.data ();

	if (vkAllocateDescriptorSets ( device, &allocInfo, descriptorSets.data () ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to allocate descriptor sets!" );
	}

	for (size_t i = 0; i < framesCount; i++) {

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

				bufferInfo[j].buffer = _info.buffers[i].buffers;
				bufferInfo[j].offset = 0;
				bufferInfo[j].range = _info.range;
				write.pBufferInfo = &bufferInfo[j];
				j++;

			}
			else if (_info.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {

				imageInfo[k].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo[k].imageView = _info.imageView;
				imageInfo[k].sampler = _info.sampler;
				write.pImageInfo = &imageInfo[k];
				k++;

			}
			descriptorWrites.push_back ( write );

		}
		vkUpdateDescriptorSets ( device, static_cast<uint32_t>(descriptorWrites.size ()), descriptorWrites.data (), 0, nullptr );
	}
	
	return descriptorSets;
}




VkFormat Device::findSupportedFormat ( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features ) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties ( physicalDevice, format, &props );

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error ( "failed to find supported format!" );
}

VkFormat Device::findDepthFormat () {
	return findSupportedFormat (
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

VulkanDepthResources Device::createDepthResources ( VkExtent2D extent ) {
	VulkanDepthResources depthResource {};

	VkFormat depthFormat = findDepthFormat ();

	createImage ( extent.width, extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthResource.image, depthResource.imageMemory );
	depthResource.imageView = createImageView ( depthResource.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT );

	return depthResource;
}

bool Device::hasStencilComponent ( VkFormat format ) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}


Pipeline Device::createGraphPipeline (

	VkVertexInputBindingDescription bindingDescription,
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
	VkDescriptorSetLayout& descriptorSetLayout, std::string vertSource, std::string fragSource ) {

	Pipeline pipe {};

	auto vertShaderCode = readFile ( vertSource );
	auto fragShaderCode = readFile ( fragSource );

	VkShaderModule vertShaderModule = createShaderModule ( vertShaderCode );
	VkShaderModule fragShaderModule = createShaderModule ( fragShaderCode );

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
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	//rasterizer.cullMode = VK_CULL_MODE_NONE;

	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;// VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;


	VkPipelineMultisampleStateCreateInfo multisampling {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencil {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

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

	VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout ( device, &pipelineLayoutInfo, nullptr, &pipe.pipelineLayout ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create pipeline layout!" );
	}

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
	pipelineInfo.layout = pipe.pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline pipeline;
	if (vkCreateGraphicsPipelines ( device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipe.pipeline ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create graphics pipeline!" );
	}

	vkDestroyShaderModule ( device, fragShaderModule, nullptr );
	vkDestroyShaderModule ( device, vertShaderModule, nullptr );

	return pipe;
}