#include "VulkanText.h"
#include <opencv2/opencv.hpp>

// Visualiza el bitmap SDF usando OpenCV
void visualizeSDFBitmap ( FT_Bitmap& bitmap, const std::string& windowName ) {
	cv::Mat sdfImage ( bitmap.rows, bitmap.width, CV_8UC1, bitmap.buffer );
	cv::imshow ( windowName, sdfImage );
	cv::waitKey ( 0 );
}

// Guarda el bitmap SDF como una imagen usando OpenCV
void saveSDFBitmap ( FT_Bitmap& bitmap, const std::string& filename ) {
	cv::Mat sdfImage ( bitmap.rows, bitmap.width, CV_8UC1, bitmap.buffer );
	cv::imwrite ( filename, sdfImage );
}

// Función para calcular la distancia más cercana al borde
float calculateDistance ( unsigned char* bitmap, int width, int height, int x, int y ) {
	float minDist = std::numeric_limits<float>::max ();

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			if (bitmap[j * width + i] > 0) { // Borde del glifo
				float dist = std::sqrt ( (x - i) * (x - i) + (y - j) * (y - j) );
				if (dist < minDist) {
					minDist = dist;
				}
			}
		}
	}
	return minDist;
}

// Función para generar la textura SDF
std::vector<float> generateSDF ( FT_GlyphSlot slot ) {
	int width = slot->bitmap.width;
	int height = slot->bitmap.rows;
	std::vector<float> sdf ( width * height );

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float dist = calculateDistance ( slot->bitmap.buffer, width, height, x, y );
			sdf[y * width + x] = dist;
		}
	}




	// Normalizar las distancias
	float maxDist = *std::max_element ( sdf.begin (), sdf.end () );
	for (float& value : sdf) {
		value /= maxDist;
	}

	return sdf;
}

// Función para cargar un glifo y generar su SDF
std::vector<float> loadGlyphAndGenerateSDF ( FT_Face face, char character ) {
	if (FT_Load_Char ( face, character, FT_LOAD_RENDER )) {
		throw std::runtime_error ( "Failed to load glyph" );
	}

	return generateSDF ( face->glyph );
}

namespace VULKAN {

	void updateUniformBuffer5 ( void* uniformBuffersMapped, glm::vec3 position, 
		Camera camera, uint32_t width, uint32_t height) {
		static auto startTime = std::chrono::high_resolution_clock::now ();

		auto currentTime = std::chrono::high_resolution_clock::now ();
		float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

		UniformBufferObject2 ubo {};


		ubo.model = glm::rotate ( glm::mat4 ( 1.0f ), time * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

		ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		ubo.proj = glm::perspective ( glm::radians ( 45.0f ), width / (float) height, 0.1f, 10.0f );
		ubo.proj[1][1] *= -1;

		memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
	}
	void updateUniformBuffer50 ( void* uniformBuffersMapped, glm::vec3 position,
		Camera camera, uint32_t width, uint32_t height ) {
		UniformBufferObject2 ubo {};

		// Matriz de modelo: matriz de identidad, sin rotación
		ubo.model = glm::mat4 ( 1.0f );

		// Matriz de vista: observar desde arriba (eje Z positivo)
		ubo.view = glm::lookAt ( glm::vec3 ( 0.0f, 0.0f, 2.0f ),  // posición de la cámara
			glm::vec3 ( 0.0f, 0.0f, 0.0f ),   // punto al que se mira
			glm::vec3 ( 0.0f, 1.0f, 0.0f ) ); // vector hacia arriba

		// Matriz de proyección ortográfica para evitar deformaciones
		float orthoSize = 1.0f;
		float aspectRatio = width / static_cast<float>(height);
		ubo.proj = glm::ortho ( -orthoSize * aspectRatio, orthoSize * aspectRatio,
			-orthoSize, orthoSize, 0.1f, 10.0f );

		// Inversión en el eje Y
		ubo.proj[1][1] *= -1;

		memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
	}

	VkPipeline VulkanText::createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource ) {
		
		
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

		//rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.cullMode = VK_CULL_MODE_NONE;

		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

		
		
		
		
		return pipeline1 ;
	}

	void VulkanText::initialize () {
		auto error = FT_Init_FreeType ( &library );
		if (error) {
			std::cout << "error";
		}
		else {
			std::cout << "todo bien";
		}

		error = FT_New_Face ( library,
			"C:\\source\\2024\\Sevian\\Engine\\fonts\\arial.ttf",
			0,
			&face );

		if (error == FT_Err_Unknown_File_Format) {
			std::cout << "todo bien 2";
		}
		else if (error) {
			std::cout << "error 2";
		}

		FT_Set_Pixel_Sizes ( face, 0, 48 );

		FT_GlyphSlot slot = face->glyph;
		
		for (unsigned char c = 33; c < 128; c++) {
			if (FT_Load_Char ( face, c, FT_LOAD_RENDER )) {
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			
			//FT_Render_Glyph ( slot, FT_RENDER_MODE_SDF );
			
			if (FT_Render_Glyph ( slot, FT_RENDER_MODE_SDF )) {
				std::cerr << "ERROR::FREETYTPE: Failed to render Glyph " << c << std::endl;
				continue;
			}

			//generateSDF ( slot );

			//std::string filename = "sdf_glyph_" + std::to_string ( c ) + ".png";
			//saveSDFBitmap ( slot->bitmap, filename );

			createTextureImage ( face, c, Characters[c] );

			createImageView ( Characters[c].textureImage, VK_FORMAT_R8_UNORM/*VK_FORMAT_R32_SFLOAT*/, Characters[c].textureImageView);
			createTextureSampler ( Characters[c].textureSampler );


			std::cout << "FREETYTPE:  load Glyph" << Characters[c].width << std::endl;


		}

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );

		
		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
		pipelineLayout = device->createPipelineLayout ( layouts );

		pipeline = createGraphPipeline (
			getBindingDescription2(),
			getAttributeDescriptions2 (),
			pipelineLayout,
			"shaders/text_vert.spv",
			"shaders/text_frag.spv"
			//"shaders/FinalVert.spv",
			//"shaders/FinalFrag.spv"
			//"shaders/sceneVert.spv",
			//"shaders/sceneFrag.spv"
			//"shaders/MeshEntityVert.spv",
			//"shaders/MeshEntityFrag.spv"
		);

		

		
	}

	void VulkanText::start () {

		if (!play) {
			play = true;
		}
	}

	void VulkanText::draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position, Camera camera, uint32_t width, uint32_t height) {
		
		

		if (!play) {
			play = true;
			float x = 0;
			float y = 0;
			float scale = 0.006f;
			uint32_t indexOffset = 0;
			int i = 0;
			

			for (auto c = text.begin (); c != text.end (); c++) {

				std::vector<VertexText> vertices;
				std::vector<uint32_t> indices;

				
				std::cout << " - - - i " << i << "\n";
				Character ch = Characters[*c];

				float xpos = x + ch.bearingX * scale;
				float ypos = y - (ch.height - ch.bearingY) * scale;

				float w = ch.width * scale;
				float h = ch.height * scale;

				// Define los vértices para el carácter
				//vertices.push_back ( { {xpos,     ypos    }, {0.0f, 0.0f} } ); // Bottom-left
				//vertices.push_back ( { {xpos + w, ypos    }, {1.0f, 0.0f} } ); // Bottom-right
				//vertices.push_back ( { {xpos,     ypos + h}, {0.0f, 1.0f} } ); // Top-left
				//vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } ); // Top-right

				vertices.push_back ( { {xpos,     ypos   }, {0.0f, 0.0f} } ); // Bottom-left
				vertices.push_back ( { {xpos + w, ypos    }, {1.0f, 0.0f} } ); // Bottom-right
				vertices.push_back ( { {xpos,     ypos + h}, {0.0f, 1.0f} } ); // Top-left
				vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } ); // Top-right



				indexOffset = 0;
				// Define los índices para el carácter
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );

				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 3 );
				indices.push_back ( indexOffset + 2 );
				/*
				// Define the vertices for the character
				vertices.push_back ( { {xpos,     ypos}, {0.0f, 0.0f} } );
				vertices.push_back ( { {xpos + w,     ypos},     {1.0f, 0.0f} } );
				vertices.push_back ( { {xpos , ypos+h},     {0.0f, 1.0f} } );
				vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } );

				// Define the indices for the character
				indices.push_back ( indexOffset + 2 );
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 3 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );
				*/
				indexOffset += 4;

				x += (ch.advance ) * scale; // advance.x is in 1/64th pixels
				//x += 0.20;
				auto prop = init ( vertices, indices, ch );
				tx.push_back ( prop );

				i++;
			}
		}


		for (const auto& elemento : tx) {
			//std::cout << elemento << " ";
			auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(elemento);
			if (vulkanProp) {

				Frame frame = frames[currentFrame];
				updateUniformBuffer5 ( vulkanProp->buffers[currentFrame].buffersMapped, position, camera, width, height );



				VkBuffer vertexBuffers[] = { vulkanProp->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );
				vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( commandBuffer, vulkanProp->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &vulkanProp->descriptorSets[currentFrame], 0, nullptr );
				vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(vulkanProp->indicesSizes), 1, 0, 0, 0 );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}
		}
	}

	void VulkanText::createTextureImage ( FT_Face face, char c, Character& character ) {
		if (FT_Load_Char (face, c, FT_LOAD_RENDER)) {
			std::cerr << "No se pudo cargar el glifo" << std::endl;
			return;
		}
		//std::vector<float> sdf = loadGlyphAndGenerateSDF ( face, c );
		int texWidth = face->glyph->bitmap.width;
		int texHeight = face->glyph->bitmap.rows;

		

		if (texWidth == 0) {
			texWidth = 48;
			texHeight = 61;
		}
		VkDeviceSize imageSize = texWidth * texHeight;
		//VkDeviceSize imageSize = texWidth * texHeight * sizeof ( float );

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		device->createBuffer ( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );



		void* data;
		vkMapMemory ( device->device, stagingBufferMemory, 0, imageSize, 0, &data );
		memcpy ( data, /*sdf.data ()*/ face->glyph->bitmap.buffer, static_cast<size_t>(imageSize));
		vkUnmapMemory ( device->device, stagingBufferMemory );

		if (true) {
			device->createImage ( texWidth, texHeight, VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

			device->transitionImageLayout ( character.textureImage, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
			device->copyBufferToImage ( stagingBuffer, character.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
			device->transitionImageLayout ( character.textureImage, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		}
		else {
			device->createImage ( texWidth, texHeight, VK_FORMAT_R32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

			device->transitionImageLayout ( character.textureImage, VK_FORMAT_R32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
			device->copyBufferToImage ( stagingBuffer, character.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
			device->transitionImageLayout ( character.textureImage, VK_FORMAT_R32_SFLOAT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		}
		
		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );

		character.width = face->glyph->bitmap.width;
		character.height = face->glyph->bitmap.rows;
		character.bearingX = face->glyph->bitmap_left;
		character.bearingY = face->glyph->bitmap_top;
		character.advance = face->glyph->advance.x >> 6;
	}


	void VulkanText::createImageView ( VkImage image, VkFormat format, VkImageView& imageView ) {
		VkImageViewCreateInfo viewInfo {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView ( device->device, &viewInfo, nullptr, &imageView ) != VK_SUCCESS) {
			throw std::runtime_error ( "No se pudo crear la vista de la imagen de la textura!" );
		}
	}

	void VulkanText::createTextureSampler ( VkSampler& sampler ) {
		VkSamplerCreateInfo samplerInfo {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;// VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;// VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;// VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;// VK_FALSE;// VK_TRUE;
		samplerInfo.maxAnisotropy = 16;// 1.0f;// 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler ( device->device, &samplerInfo, nullptr, &sampler ) != VK_SUCCESS) {
			throw std::runtime_error ( "No se pudo crear el sampler de la textura!" );
		}
	}

	std::shared_ptr<Entity3D> VulkanText::init ( std::vector<VertexText> vertices, std::vector<uint32_t> indices, Character texture ) {
		auto vulkanProp = std::make_shared<VulkanProperty> ();
		
		std::vector<VulkanUBuffer>  x = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );

		
		
		//vulkanProp->descriptorSets = device->createDescriptorSets ( x, texture.textureImageView, texture.textureSampler, sizeof ( UniformBufferObject2 ) );


		std::vector<DSInfo> bufDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, x, sizeof ( x ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		//std::vector<DSInfo> texDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, x, sizeof ( x ), texture.textureImageView, texture.textureSampler, 1 } );

		vulkanProp->descriptorSets = device->createDescriptorSets ( descriptorSetLayout, bufDSInfo );
		



		auto attributeDescriptions = getAttributeDescriptions2 ();
		//auto pipeline = createGraphicsPipeline3 ( getBindingDescription2 (), attributeDescriptions, device->descriptorSetLayout );

		vulkanProp->vertex = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		vulkanProp->indices = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		vulkanProp->indicesSizes = indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		//vulkanProp->pipeline = pipeline.pipeline;
		//vulkanProp->pipelineLayout = pipeline.pipelineLayout;
		vulkanProp->buffers = x;
		
		return vulkanProp;


	}
	
	Pipeline VulkanText::createGraphicsPipeline3 (

		VkVertexInputBindingDescription bindingDescription,
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
		VkDescriptorSetLayout& descriptorSetLayout ) {

		Pipeline pipe {};
		auto vertShaderCode = device->readFile ( "shaders/text_vert.spv" );
		auto fragShaderCode = device->readFile ( "shaders/text_frag.spv" );

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
		
		//rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.cullMode = VK_CULL_MODE_NONE;

		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
	
	

}