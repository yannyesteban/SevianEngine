#include "Renderer.h"
//#include "VulkanProperty.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


using namespace ::SEVIAN;

namespace VULKAN {
	
	static void loadModel ( const std::string path, std::vector<Vertex> & vertices, std::vector<uint32_t> & indices ) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj ( &attrib, &shapes, &materials, &warn, &err, path.c_str () )) {
			throw std::runtime_error ( warn + err );
		}

		std::unordered_map<Vertex, uint32_t> uniqueVertices {};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex {};

				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoords = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count ( vertex ) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size ());
					vertices.push_back ( vertex );
				}

				indices.push_back ( uniqueVertices[vertex] );
			}
		}
	}

	void VulkanRenderer::test2 () {
		std::cout << "Yanny esteban nuñnez\n";
	}

	void VulkanRenderer::test3 () {
		std::cout << "Yanny esteban nuñez test 3\n";
	}
	void VulkanRenderer::initialize ( ) {
				
		createInstance ();
		createSurface ();

		physicalDevices = getPhysicalDevices ();

		int position = 0;

		device = physicalDevices.at ( position ).createDevice ();
		commandPool = physicalDevices.at ( position ).createCommandPool ();

		graphicsQueue = physicalDevices.at ( position ).graphicsQueue;
		presentQueue = physicalDevices.at ( position ).presentQueue;

		device->commandPool = commandPool;
		device->createCommandBuffers ( device->frames, commandPool );

		swapChain = physicalDevices.at ( position ).createSwapChain ( window );
		extent = swapChain.extent;
		renderPass = device->createRenderPass ( swapChain.imageFormat );

		depthResources = device->createDepthResources ( extent );

		swapChain.framebuffers = physicalDevices.at ( position ).createFramebuffers ( swapChain, renderPass, { depthResources.imageView } );

		device->renderPass = renderPass;

		descriptorSetLayout = device->createDescriptorSetLayout ();
		descriptorPool = device->createDescriptorPool ();
		device->descriptorSetLayout = descriptorSetLayout;

		textureManager = std::make_shared<TextureManager> ( device );

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

		meshManager = new MeshManager ( device, textureManager.get() );
	}

	void VulkanRenderer::initialize ( GameApp* _app ) {
		app = _app;

		initWindow ();
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



	void VulkanRenderer::renderFrame () {
		// Inicialización específica de Vulkan
	}

	void VulkanRenderer::doLoop () {
		int k = 0;
		while (!glfwWindowShouldClose ( window )) {
			glfwPollEvents ();
			renderFrame ();

		}
	}

	void VulkanRenderer::cleanup () {
		// Inicialización específica de Vulkan
	}

	bool VulkanRenderer::windowShouldClose () {
		return glfwWindowShouldClose ( window );
	}

	std::vector<VkDescriptorSet> VulkanRenderer::createDescriptorSets ( std::vector<Frame>& frames, VkDescriptorPool descriptorPool, 
		VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range ) {

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
		if (vkAllocateDescriptorSets ( device->device, &allocInfo, descriptorSets.data () ) != VK_SUCCESS) {
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

			vkUpdateDescriptorSets ( device->device, static_cast<uint32_t>(descriptorWrites.size ()), descriptorWrites.data (), 0, nullptr );


		}

		return descriptorSets;
	}

	Pipeline VulkanRenderer::createGraphicsPipeline (

		VkVertexInputBindingDescription bindingDescription,
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
		VkDescriptorSetLayout& descriptorSetLayout ) {

		Pipeline pipe {};
		auto vertShaderCode = device->readFile ( "shaders/solid_v.spv" );
		auto fragShaderCode = device->readFile ( "shaders/solid_f.spv" );

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

		if (vkCreatePipelineLayout ( device->device, &pipelineLayoutInfo, nullptr, &pipe.pipelineLayout ) != VK_SUCCESS) {
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
		pipelineInfo.renderPass = device->renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		VkPipeline pipeline;
		if (vkCreateGraphicsPipelines ( device->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipe.pipeline ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create graphics pipeline!" );
		}

		vkDestroyShaderModule ( device->device, fragShaderModule, nullptr );
		vkDestroyShaderModule ( device->device, vertShaderModule, nullptr );

		return pipe;
	}

	void VulkanRenderer::initEntity ( Info3D info ) {

		auto entity = std::make_unique<Entity> ();


		std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );

		auto texture = mTextures[info.texture.c_str ()].get();
		//auto texture = mTextures[info.texture.c_str ()];
		entity->descriptorSets = device->createDescriptorSets ( ubo,  texture->imageView, texture->sampler, sizeof ( UniformBufferObject2 ) );
		auto attributeDescriptions = getAttributeDescriptions ();
		auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

		entity->vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		entity->indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		entity->indicesSizes = info.indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		entity->pipeline = pipeline.pipeline;
		entity->pipelineLayout = pipeline.pipelineLayout;
		entity->ubo = ubo;

		entities[info.entityId] = std::move ( entity );


	}

	void VulkanRenderer::drawEntity ( uint32_t entityId, glm::vec3 position, Camera camera ) {

		auto entity = entities[entityId].get();

		//auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);

		if (entity) {

			Frame frame = frames[currentFrame];
			updateUniformBuffer (entity->ubo[currentFrame].buffersMapped, position, camera );



			VkBuffer vertexBuffers[] = { entity->vertex.buffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipeline );
			vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
			//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindIndexBuffer ( commandBuffer, entity->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

			vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipelineLayout, 0, 1, &entity->descriptorSets[currentFrame], 0, nullptr );
			vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(entity->indicesSizes), 1, 0, 0, 0 );
		}
		else {
			std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
		}
	}

	void VulkanRenderer::initEntity ( Entity3D * unit ) {
	}

	void VulkanRenderer::drawEntity ( Entity3D * unit, glm::vec3 position, Camera ) {
	}


	std::shared_ptr<Entity3D> VulkanRenderer::init ( std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string name ) {
		auto vulkanProp = std::make_shared<VulkanProperty> ();

		std::vector<VulkanUBuffer>  x = device->createUniformBuffer( frames, sizeof ( UniformBufferObject2 ) );


		//auto texture = mTextures[name.c_str ()];
		auto texture = mTextures[name.c_str ()].get ();
		vulkanProp->descriptorSets = device->createDescriptorSets ( x, texture->imageView, texture->sampler, sizeof ( UniformBufferObject2 ) );
		auto attributeDescriptions = getAttributeDescriptions ();
		auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

		vulkanProp->vertex = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		vulkanProp->indices = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		vulkanProp->indicesSizes = indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		vulkanProp->pipeline = pipeline.pipeline;
		vulkanProp->pipelineLayout = pipeline.pipelineLayout;
		vulkanProp->buffers = x;

		return vulkanProp;


	}
	void VulkanRenderer::cleanupSwapChain () {

		vkDestroyImageView ( device->device, depthResources.imageView, nullptr );
		vkDestroyImage ( device->device, depthResources.image, nullptr );
		vkFreeMemory ( device->device, depthResources.imageMemory, nullptr );

		for (auto framebuffer : swapChain.framebuffers) {
			vkDestroyFramebuffer ( device->device, framebuffer, nullptr );
		}

		for (auto imageView : swapChain.imageViews) {
			vkDestroyImageView ( device->device, imageView, nullptr );
		}

		vkDestroySwapchainKHR ( device->device, swapChain.swapchain, nullptr );
	}

	void VulkanRenderer::recreateSwapChain () {

		int width = 0, height = 0;
		glfwGetFramebufferSize ( window, &width, &height );
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize ( window, &width, &height );
			glfwWaitEvents ();
		}

		vkDeviceWaitIdle ( device->device );

		cleanupSwapChain ();


		int position = 0;

		swapChain = physicalDevices.at ( position ).createSwapChain ( window );
		depthResources = device->createDepthResources ( swapChain.extent );
		swapChain.framebuffers = physicalDevices.at ( position ).createFramebuffers ( swapChain, renderPass, { depthResources.imageView } );
	}

	void VulkanRenderer::addTexture ( TextureInfo info ) {

		textureManager->add ( info );
	}

	void VulkanRenderer::addTexture ( std::string name, std::string path ) {
		if (mTextures.find ( name ) != mTextures.end ()) {

			std::cerr << "Warning: Texture with name '" << name << "' already exists. Skipping assignment." << std::endl;
			return;
		}

		mTextures[name] = device->createTexture ( path.c_str () );
	}

	void VulkanRenderer::addShaders ( std::string, std::string ) {
	}
	void VulkanRenderer::updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position, Camera camera ) {
		static auto startTime = std::chrono::high_resolution_clock::now ();

		auto currentTime = std::chrono::high_resolution_clock::now ();
		float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

		UniformBufferObject2 ubo {};
		
		//std::cout << position.x << ":" << position.y << ":" << position.x << "\n";
		glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position );
		glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		ubo.model = /* rotation * */  translation;
		

		// Posición de la cámara arriba en el eje Z
		glm::vec3 cameraPos = camera.position;//glm::vec3 ( 0.0f, 0.0f, -5.0f );

		//glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 5.0f );

		// Punto al que está mirando la cámara (el origen en este caso)
		glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		// Dirección "up" (hacia el eje Y)
		//glm::vec3 up = camera.up; // glm::vec3 ( 0.0f, 1.0f, 0.0f );
		glm::vec3 up = glm::vec3 ( 0.0f, 10.0f, 0.0f );

		ubo.view = glm::lookAt ( cameraPos, target, up );

		//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		ubo.proj = glm::perspective ( glm::radians ( 45.0f ), extent.width / (float) extent.height, 0.1f, 100.0f );

		//ubo.view = glm::mat4 ( 1.0f );
		//ubo.proj = glm::mat4 ( 1.0f );

		ubo.proj[1][1] *= -1;

		memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
	}

	void VulkanRenderer::updateUniformBuffer ( Frame frame ) {
		static auto startTime = std::chrono::high_resolution_clock::now ();

		auto currentTime = std::chrono::high_resolution_clock::now ();
		float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

		UniformBufferObject2 ubo {};
		ubo.model = glm::rotate ( glm::mat4 ( 1.0f ), time * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		ubo.proj = glm::perspective ( glm::radians ( 45.0f ), extent.width / (float) extent.height, 0.1f, 10.0f );
		ubo.proj[1][1] *= -1;

		memcpy ( frame.uniformBuffersMapped, &ubo, sizeof ( ubo ) );
	}

	void VulkanRenderer::beginFrame () {

		Frame frame = device->frames[device->currentFrame];

		vkWaitForFences ( device->device, 1, &frame.inFlightFences, VK_TRUE, UINT64_MAX );

		//uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR ( device->device, swapChain.swapchain, UINT64_MAX, frame.imageAvailableSemaphores, VK_NULL_HANDLE, &imageIndex );

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain ();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error ( "failed to acquire swap chain image!" );
		}

		vkResetFences ( device->device, 1, &frame.inFlightFences );
		vkResetCommandBuffer ( frame.commandBuffers, /*VkCommandBufferResetFlagBits*/ 0 );

		commandBuffer = frame.commandBuffers;

		/* record command buffer */
		VkCommandBufferBeginInfo beginInfo {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer ( frame.commandBuffers, &beginInfo ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to begin recording command buffer!" );
		}

		VkRenderPassBeginInfo renderPassInfo {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = device->renderPass;
		renderPassInfo.framebuffer = swapChain.framebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.extent;

		/*VkClearValue clearColor = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		*/

		std::array<VkClearValue, 2> clearValues {};
		clearValues[0].color = { {0.0f, 0.1f, 0.0f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size ());
		renderPassInfo.pClearValues = clearValues.data ();

		vkCmdBeginRenderPass ( frame.commandBuffers, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );


		VkViewport viewport {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) swapChain.extent.width;
		viewport.height = (float) swapChain.extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport ( frame.commandBuffers, 0, 1, &viewport );

		VkRect2D scissor {};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChain.extent;
		vkCmdSetScissor ( frame.commandBuffers, 0, 1, &scissor );




		//entity.draw ( commandBuffer, frame );
		//entity2.draw ( commandBuffer, frame );

		/*

		VkBuffer vertexBuffers[] = { objects[0].vertex.buffer };
		VkDeviceSize offsets[] = { 0 };

		auto obj = objects[0];

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, objects[0].indices.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,&frame.descriptorSet, 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(obj.item.indices.size()), 1, 0, 0, 0);
		*/



	}
	void VulkanRenderer::draw ( std::shared_ptr<Entity3D> prop, glm::vec3 position, Camera camera ) {

		auto entity = std::dynamic_pointer_cast<Entity>(prop);

		//auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);

		if (entity) {

			Frame frame = frames[currentFrame];
			updateUniformBuffer ( entity->ubo[currentFrame].buffersMapped, position, camera );

			VkBuffer vertexBuffers[] = { entity->vertex.buffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipeline );
			vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
			//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindIndexBuffer ( commandBuffer, entity->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

			vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipelineLayout, 0, 1, &entity->descriptorSets[currentFrame], 0, nullptr );
			vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(entity->indicesSizes), 1, 0, 0, 0 );
		}
		else {
			std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
		}


		/*
		auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);

		if (vulkanProp) {

			Frame frame = frames[currentFrame];
			updateUniformBuffer ( vulkanProp->buffers[currentFrame].buffersMapped, position, camera);

			

			VkBuffer vertexBuffers[] = { vulkanProp->vertex.buffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanProp->pipeline );
			vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
			//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindIndexBuffer ( commandBuffer, vulkanProp->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

			vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanProp->pipelineLayout, 0, 1, &vulkanProp->descriptorSets[currentFrame], 0, nullptr );
			vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(vulkanProp->indicesSizes), 1, 0, 0, 0 );
		}
		else {
			std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
		}
		*/
	}

	void VulkanRenderer::draw ( std::shared_ptr<Entity3D> prop, UniformBufferObject ubo ) {
		auto entity = std::dynamic_pointer_cast<Entity>(prop);

		//auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);
		ve;
		ve->render ( ubo );
		return;

		if (entity) {

			Frame frame = device->frames[currentFrame];
			
			MeUBO me = {};
			me.color = glm::vec3 (  0.99f, 0.026f, 0.011f );
			me.color2 = glm::vec3 ( 0.5f, 0.0f, 1.0f );
			me.intensity = 2.1f - 0.11f;

			
			LightUBO l = { };
			
			l.position = glm::vec3 ( 0.6f, 0.05f, 0.1f );
			l.color = glm::vec3 ( 0.6f, 0.3f, 0.0f );
			l.intensity = 0.5f;


			UniformBufferObject ubo2 {};

			
			glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), l.position );
			//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

			glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), l.color.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
				glm::rotate ( glm::mat4 ( 1.0f ), l.color.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
				glm::rotate ( glm::mat4 ( 1.0f ), l.color.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
			ubo2.model = translation * rotationMat;


			//ubo.model = /* rotation * */  translation;


			// Posición de la cámara arriba en el eje Z
			glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, -5.0f );

			//glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 5.0f );

			// Punto al que está mirando la cámara (el origen en este caso)
			glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
			// Dirección "up" (hacia el eje Y)
			//glm::vec3 up = camera.up; // glm::vec3 ( 0.0f, 1.0f, 0.0f );
			glm::vec3 up = glm::vec3 ( 0.0f, -25.0f, 0.0f );

			ubo2.view = glm::lookAt ( cameraPos, target, up );

			//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
			ubo2.proj = glm::perspective ( glm::radians ( 45.0f ), 1300 / (float) 600, 0.1f, 100.0f );

			
			memcpy ( entity->light[currentFrame].buffersMapped, &l, sizeof ( l ) );
			memcpy ( entity->me[currentFrame].buffersMapped, &me, sizeof(me) );
			memcpy ( entity->ubo[currentFrame].buffersMapped, &ubo, sizeof ( ubo ) );
			//memcpy ( entity->ubo2[currentFrame].buffersMapped, &ubo2, sizeof ( ubo2 ) );
			

			VkBuffer vertexBuffers[] = { entity->vertex.buffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipeline );
			vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
			//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindIndexBuffer ( commandBuffer, entity->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

			vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipelineLayout, 0, 1, &entity->descriptorSets[currentFrame], 0, nullptr );
			vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(entity->indicesSizes), 1, 0, 0, 0 );

			entity->render (ubo);
		}
		else {
			std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
		}

	}

	void VulkanRenderer::drawText ( std::string text, glm::vec3 position, Camera camera ) {

		fontText.draw (
			text, currentFrame, commandBuffer, position, camera, extent.width, extent.height );
	}
	void VulkanRenderer::endFrame () {

		

		Frame frame = device->frames[device->currentFrame];
		auto commandBuffer = frame.commandBuffers;
		vkCmdEndRenderPass ( commandBuffer );

		if (vkEndCommandBuffer ( commandBuffer ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to record command buffer!" );
		}


		VkSubmitInfo submitInfo {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { frame.imageAvailableSemaphores };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frame.commandBuffers;

		VkSemaphore signalSemaphores[] = { frame.renderFinishedSemaphores };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit ( graphicsQueue, 1, &submitInfo, frame.inFlightFences ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to submit draw command buffer!" );
		}

		VkPresentInfoKHR presentInfo {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain.swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;


		VkResult result = vkQueuePresentKHR ( presentQueue, &presentInfo );

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {

			std::cout << "error error \n";
			framebufferResized = false;
			recreateSwapChain ();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error ( "failed to present swap chain image!" );
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	std::unique_ptr<Entity3D> VulkanRenderer::createEntity ( Info3D info ) {
		
		if (0 == 0) {
			auto entity = meshManager->create ( info );
			return std::unique_ptr<Entity3D> ( std::move ( entity ) );
		}
		
		if (1 == 1) {
			//auto entity = std::make_unique<MeshEntity> ( device, textureManager.get (), info );
			//return std::unique_ptr<Entity3D> ( std::move ( entity ) );
		}

		auto entity = std::make_unique<VulkanEntity> ( device, textureManager.get (), info );
		//auto entity = std::make_unique<Entity3D> ();

		return std::unique_ptr<Entity3D> ( std::move ( entity ) );
		/*
		auto x = new VulkanEntity (
			device, textureManager.get(), info
		);

		ve = x;
		auto entity = std::make_unique<Entity3D> ();
		

		std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		std::vector<VulkanUBuffer> ubo2 = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		std::vector<VulkanUBuffer> lightUBO = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );
		std::vector<VulkanUBuffer> meUBO = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );

		auto texture = mTextures[info.texture].get ();
		//auto texture = mTextures[info.texture.c_str ()];
		std::vector<BufferInfo> buffersInfo;
		
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );
		//buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo2, sizeof ( UniformBufferObject ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, lightUBO, sizeof ( LightUBO ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
		//
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ubo, sizeof ( UniformBufferObject ), texture->imageView, texture->sampler, 2 } );
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, meUBO, sizeof ( MeUBO ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 3 } );

		entity->descriptorSets = device->createDescriptorSets ( buffersInfo );
		//entity->descriptorSets = device->createDescriptorSets ( ubo, texture->imageView, texture->sampler, sizeof ( UniformBufferObject ) );
		
		
		auto attributeDescriptions = getAttributeDescriptions ();
		//auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

		if (pipeline.pipeline == VK_NULL_HANDLE) {

			//auto fragShaderCode = device->readFile ( "shaders/solid_f.spv" );
			pipeline = device->createGraphPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout, "shaders/total_v.spv", "shaders/total_f.spv" );
			//pipeline = device->createGraphPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout, "shaders/lights_v.spv", "shaders/lights_f.spv" );
		}

		entity->vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		entity->indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		entity->indicesSizes = info.indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		entity->pipeline = pipeline.pipeline;
		entity->pipelineLayout = pipeline.pipelineLayout;
		entity->ubo = ubo;
		entity->ubo2 = ubo2;
		entity->light = lightUBO;
		entity->me = meUBO;

		//entities[info.entityId] = std::move ( entity );

		//return static_cast<PropertyRender&>(*entity);
		return std::unique_ptr<PropertyRender> ( std::move ( entity ) );
		//return std::shared_ptr<PropertyRender> ( std::move ( entity ) );
		*/
	}

	std::unique_ptr<Entity3D> VulkanRenderer::createSprite ( Sprite3D info ) {

		
		std::vector<Vertex> vertices = {
			{{-info.width / 2.0f, -info.height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, info.color}, // Bottom-left
			{{ info.width / 2.0f, -info.height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, info.color}, // Bottom-right
			{{ info.width / 2.0f,  info.height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, info.color}, // Top-right
			{{-info.width / 2.0f,  info.height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, info.color}  // Top-left
		};

		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0 // Two triangles forming a square
		};

		auto entity = std::make_unique<Entity> ();


		std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );

		auto texture = mTextures[info.texture].get ();
		//auto texture = mTextures[info.texture.c_str ()];
		entity->descriptorSets = device->createDescriptorSets ( ubo, texture->imageView, texture->sampler, sizeof ( UniformBufferObject2 ) );
		auto attributeDescriptions = getAttributeDescriptions ();
		//auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

		if (pipeline.pipeline == VK_NULL_HANDLE) {

			//auto fragShaderCode = device->readFile ( "shaders/solid_f.spv" );
			pipeline = device->createGraphPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout, "shaders/solid_v.spv", "shaders/solid_f.spv" );
		}

		entity->vertex = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		entity->indices = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		entity->indicesSizes = indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		entity->pipeline = pipeline.pipeline;
		entity->pipelineLayout = pipeline.pipelineLayout;
		entity->ubo = ubo;

		//entities[info.entityId] = std::move ( entity );

		//return static_cast<PropertyRender&>(*entity);
		return std::unique_ptr<Entity3D> ( std::move ( entity ) );
	}

	std::unique_ptr<Entity3D> VulkanRenderer::createModel ( Model3D info ) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		loadModel ( info.model, vertices, indices );

		auto entity = std::make_unique<Entity> ();


		std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );

		auto texture = mTextures[info.texture].get ();
		//auto texture = mTextures[info.texture.c_str ()];
		entity->descriptorSets = device->createDescriptorSets ( ubo, texture->imageView, texture->sampler, sizeof ( UniformBufferObject2 ) );
		auto attributeDescriptions = getAttributeDescriptions ();
		//auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

		if (pipeline.pipeline == VK_NULL_HANDLE) {

			//auto fragShaderCode = device->readFile ( "shaders/solid_f.spv" );
			pipeline = device->createGraphPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout, "shaders/solid_v.spv", "shaders/solid_f.spv" );
		}

		entity->vertex = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		entity->indices = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		entity->indicesSizes = indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		entity->pipeline = pipeline.pipeline;
		entity->pipelineLayout = pipeline.pipelineLayout;
		entity->ubo = ubo;

		//entities[info.entityId] = std::move ( entity );

		//return static_cast<PropertyRender&>(*entity);
		return std::unique_ptr<Entity3D> ( std::move ( entity ) );
		//return std::unique_ptr<PropertyRender> ();
	}





	void VulkanRenderer::initWindow () {
		glfwInit ();

		glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );

		window = glfwCreateWindow ( width, height, title.c_str (), nullptr, nullptr );
		glfwSetWindowUserPointer ( window, this );
		//glfwSetFramebufferSizeCallback ( window, this->resizeCallback );

		glfwSetKeyCallback ( window, keyCallback );


	}

	bool VulkanRenderer::checkValidationLayerSupport ()

	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties ( &layerCount, nullptr );

		std::vector<VkLayerProperties> availableLayers ( layerCount );
		vkEnumerateInstanceLayerProperties ( &layerCount, availableLayers.data () );

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp ( layerName, layerProperties.layerName ) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VulkanRenderer::getRequiredExtensions () {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions ( &glfwExtensionCount );

		std::vector<const char*> extensions ( glfwExtensions, glfwExtensions + glfwExtensionCount );

		if (enableValidationLayers) {
			extensions.push_back ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
		}

		return extensions;
	}

	void VulkanRenderer::populateDebugMessengerCreateInfo ( VkDebugUtilsMessengerCreateInfoEXT& createInfo ) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void VulkanRenderer::createInstance () {
		if (enableValidationLayers && !checkValidationLayerSupport ()) {
			throw std::runtime_error ( "validation layers requested, but not available!" );
		}

		VkApplicationInfo appInfo {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION ( 1, 0, 0 );
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION ( 1, 0, 0 );
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions ();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size ());
		createInfo.ppEnabledExtensionNames = extensions.data ();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size ());
			createInfo.ppEnabledLayerNames = validationLayers.data ();

			populateDebugMessengerCreateInfo ( debugCreateInfo );
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance ( &createInfo, nullptr, &instance ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create instance!" );
		}
	}

	VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT ( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger ) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr ( instance, "vkCreateDebugUtilsMessengerEXT" );
		if (func != nullptr) {
			return func ( instance, pCreateInfo, pAllocator, pDebugMessenger );
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanRenderer::setupDebugMessenger () {
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo ( createInfo );

		if (CreateDebugUtilsMessengerEXT ( instance, &createInfo, nullptr, &debugMessenger ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to set up debug messenger!" );
		}
	}

	void VulkanRenderer::createSurface () {

		if (glfwCreateWindowSurface ( instance, window, nullptr, &surface ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create window surface!" );
		}
	}

	QueueFamilyIndices VulkanRenderer::findQueueFamilies ( VkPhysicalDevice physicalDevice ) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties ( physicalDevice, &queueFamilyCount, nullptr );

		std::vector<VkQueueFamilyProperties> queueFamilies ( queueFamilyCount );
		vkGetPhysicalDeviceQueueFamilyProperties ( physicalDevice, &queueFamilyCount, queueFamilies.data () );

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR ( physicalDevice, i, surface, &presentSupport );

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete ()) {
				break;
			}

			i++;
		}

		return indices;
	}

	bool VulkanRenderer::checkDeviceExtensionSupport ( VkPhysicalDevice physicalDevice ) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties ( physicalDevice, nullptr, &extensionCount, nullptr );

		std::vector<VkExtensionProperties> availableExtensions ( extensionCount );
		vkEnumerateDeviceExtensionProperties ( physicalDevice, nullptr, &extensionCount, availableExtensions.data () );

		std::set<std::string> requiredExtensions ( deviceExtensions.begin (), deviceExtensions.end () );

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase ( extension.extensionName );
		}

		return requiredExtensions.empty ();
	}

	SwapChainSupportDetails VulkanRenderer::querySwapChainSupport ( VkPhysicalDevice physicalDevice ) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR ( physicalDevice, surface, &details.capabilities );

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, surface, &formatCount, nullptr );

		if (formatCount != 0) {
			details.formats.resize ( formatCount );
			vkGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, surface, &formatCount, details.formats.data () );
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, surface, &presentModeCount, nullptr );

		if (presentModeCount != 0) {
			details.presentModes.resize ( presentModeCount );
			vkGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, surface, &presentModeCount, details.presentModes.data () );
		}

		return details;
	}

	bool VulkanRenderer::isDeviceSuitable ( VkPhysicalDevice physicalDevice ) {
		QueueFamilyIndices indices = findQueueFamilies ( physicalDevice );

		bool extensionsSupported = checkDeviceExtensionSupport ( physicalDevice );

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport ( physicalDevice );
			swapChainAdequate = !swapChainSupport.formats.empty () && !swapChainSupport.presentModes.empty ();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures ( physicalDevice, &supportedFeatures );

		return indices.isComplete () && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	std::vector <PhysicalDevice> VulkanRenderer::getPhysicalDevices () {

		std::vector <PhysicalDevice>physicalDevices;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices ( instance, &deviceCount, nullptr );

		if (deviceCount == 0) {
			throw std::runtime_error ( "failed to find GPUs with Vulkan support!" );
		}

		std::vector<VkPhysicalDevice> devices ( deviceCount );
		vkEnumeratePhysicalDevices ( instance, &deviceCount, devices.data () );


		for (const auto& device : devices) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties ( device, &deviceProperties );
			std::cout << "deviceProperties:  " << deviceProperties.deviceName << std::endl;
			if (isDeviceSuitable ( device )) {
				std::cout << " isDeviceSuitable -> deviceProperties:  " << deviceProperties.deviceName << std::endl;
				PhysicalDevice ph = PhysicalDevice ( instance, surface, device );
				physicalDevices.push_back ( ph );
				//physicalDevice = device;
				//break;
			}
		}

		if (physicalDevices.size () == 0) {
			throw std::runtime_error ( "failed to find a suitable GPU!" );
		}

		return physicalDevices;
	}


	void VulkanTextureNO::load ( const std::string& path ) {
		// Inicialización específica de Vulkan
	}

	void VulkanShader::load ( const std::string& vertexPath, const std::string& fragmentPath ) {
		// Inicialización específica de Vulkan
	}




	

}