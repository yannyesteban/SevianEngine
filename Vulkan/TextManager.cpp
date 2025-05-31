#include "TextManager.h"


namespace SEVIAN {
	namespace VULKAN {
		TextManager::TextManager ( VulkanInfo vulkan ) : vulkan ( vulkan ), device ( vulkan.device ) {

			std::vector<DescriptorSetLayoutInfo> bufDSLInfo;
			bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
			bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

			descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
			//ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

			std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
			pipelineLayout = device->createPipelineLayout ( layouts );

			pipeline = createGraphPipeline (
				VertexText::getBindingDescription (),
				VertexText::getAttributeDescriptions (),
				pipelineLayout,
				"shaders/msdf.vert.spv",
				"shaders/msdf.frag.spv"
			);
		}

		TextManager::TextManager ( std::shared_ptr<VulkanRenderer> renderer ) :device ( renderer->device ), descriptorPool ( renderer->descriptorPool ), frames ( renderer->frames ) {

			resourceData = std::make_shared<ResourceData> ( device );

			resourceData->addResourceData ( RENDERER::DataResource::TRANSFORM, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, VK_SHADER_STAGE_VERTEX_BIT );
			resourceData->addResourceData ( RENDERER::DataResource::TEXTURE, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT );

			//resourceData->addPushConstant ( RENDERER::DataResource::STYLE, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof ( StyleUbo ), 0 );

			resourceData->init ();
			pipeline = createGraphPipeline (
				VertexText::getBindingDescription (),
				VertexText::getAttributeDescriptions (),
				resourceData->pipelineLayout,
				"shaders/msdf.vert.spv",
				"shaders/msdf.frag.spv"
			);
		}

		void TextManager::addFont ( Font font, bool setDefault = false ) {

			fonts.emplace ( font.name, FontData { font, createAtlasTexture ( font.atlas.data, font.atlas.width, font.atlas.height ) } );

			if (setDefault) {
				defaultFont = font.name;


			}

			/*
			fonts.emplace ( font.name, font );
			auto _texture = fontTextures.emplace ( font.name, createAtlasTexture ( font.atlas.data, font.atlas.width, font.atlas.height ) );

			if (setDefault) {
				defaultFont = font.name;
				defaultTexture = _texture.second; // como puedo recuperar la ultima textura que emplacé

			}*/


			//fonts[name] = font;
			//fontTextures[name] = createAtlasTexture (font.atlas.data, font.atlas.width, font.atlas.height);
		}

		Font TextManager::getFont ( std::string name ) {
			auto it = fonts.find ( name );
			if (it != fonts.end ()) {
				return it->second.font;
			}
			else if (!defaultFont.empty ()) {

				return fonts.at ( defaultFont ).font;
			}
			throw std::runtime_error ( "Font not found" );
			//auto it = fonts.find ( name );
			//if (it != fonts.end ()) {
			//	return it->second; // La fuente fue encontrada
			//}
			//else {
			//	// Manejar el caso en el que la fuente no existe
			//	if (defaultFont != "") {
			//		auto it = fonts.find ( defaultFont );
			//		if (it != fonts.end ()) {
			//			return it->second; // La fuente fue encontrada
			//		}
			//	}
			//}

			//throw std::runtime_error ( "Font not found and no default font set." );
		}

		VulkanTexture TextManager::getTexture ( std::string name ) {
			auto it = fonts.find ( name );
			if (it != fonts.end ()) {
				return it->second.texture;
			}
			else if (!defaultFont.empty ()) {

				return fonts.at ( defaultFont ).texture;
			}
			throw std::runtime_error ( "Texture not found" );
		}

		std::unique_ptr<RENDERER::IRenderizable> TextManager::createText ( float x, float y, std::vector<Quad> quads, std::string textureName ) {

			auto element = std::make_unique<VKElement> ();
			uint32_t indexOffset = 0;
			std::vector<VertexText> vertices;
			std::vector<uint32_t> indices;

			for (auto& quad : quads) {

				float xpos = x + quad.xPos;
				float ypos = y + quad.yPos;

				float u0 = quad.u0;
				float v0 = quad.v0;
				float u1 = quad.u1;
				float v1 = quad.v1;

				float w = quad.width;
				float h = quad.height;

				int color = 5;
				
				//vertices.push_back ( { {xpos, ypos}, {u0, v1}, color } );             // Bottom-left
				//vertices.push_back ( { {xpos + w, ypos}, {u1, v1}, color } );         // Bottom-right
				//vertices.push_back ( { {xpos, ypos + h}, {u0, v0}, color } );         // Top-left
				//vertices.push_back ( { {xpos + w, ypos + h}, {u1, v0}, color } );     // Top-right
				//indices.push_back ( indexOffset + 0 );
				//indices.push_back ( indexOffset + 1 );
				//indices.push_back ( indexOffset + 2 );
				//indices.push_back ( indexOffset + 1 );
				//indices.push_back ( indexOffset + 3 );
				//indices.push_back ( indexOffset + 2 );
				
				

				// ¡Usar push_back para añadir los vértices a la lista!
				vertices.push_back ( { { xpos    , ypos     }, { u0 , v0 }, color } ); // Top-left (¡Correcto para sistema Y-abajo, origen arriba!)
				vertices.push_back ( { { xpos + w, ypos     }, { u1 , v0 }, color } ); // Top-right
				vertices.push_back ( { { xpos + w, ypos + h }, { u1 , v1 }, color } ); // Bottom-right
				vertices.push_back ( { { xpos    , ypos + h }, { u0 , v1 }, color } ); // Bottom-left

				// ¡Usar push_back para añadir los índices a la lista!
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );

				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 2 );
				indices.push_back ( indexOffset + 3 );

				indexOffset += 4;
				//x += quad.advance;
			}

			element->frames.resize ( MAX_FRAMES_IN_FLIGHT );
			texture = getTexture ( textureName );

			for (auto& descriptor : resourceData->descriptorInfo) {

				std::vector < DescriptorSetDataInfo > dataInfo;
				for (auto& _info : descriptor) {
					if (_info->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
						dataInfo.push_back ( { _info->id, _info->binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, sizeof ( UniformBufferObject ), nullptr, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL } );
					}
					else if (_info->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
						dataInfo.push_back ( { _info->id, _info->binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &texture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL } );
					}

				}

				resourceData->createDescriptorSets ( element->frames, descriptorPool, resourceData->descriptorSetLayout, dataInfo );
			}
			element->pipeline = pipeline;
			element->pipelineLayout = resourceData->pipelineLayout;
			element->pushConstantsInfo = resourceData->pushConstantsInfo;
			element->vertexBuffer = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
			element->indicesBuffer = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
			element->indicesSizes = indices.size ();

			
			
			return element;


		}

		std::unique_ptr<RENDERER::IRenderizable> TextManager::createText ( TEXT::Box box ) {
			return std::unique_ptr<RENDERER::IRenderizable> ();
		}

		VulkanTexture TextManager::createAtlasTexture ( std::vector<uint8_t> msdfData, int atlasWidth, int atlasHeight ) {
			VulkanTexture texture;
			VkDeviceSize imageSize = msdfData.size ();
			VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

			// Crear el staging buffer
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			device->createBuffer (
				imageSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingBufferMemory );

			void* data;
			vkMapMemory ( device->device, stagingBufferMemory, 0, imageSize, 0, &data );
			memcpy ( data, msdfData.data (), static_cast<size_t>(imageSize) );
			vkUnmapMemory ( device->device, stagingBufferMemory );

			// Crear la textura Vulkan
			device->createImage ( atlasWidth, atlasHeight, format, VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture.textureImage, texture.textureImageMemory );

			// Transiciones de layouts y copia de buffer a imagen
			device->transitionImageLayout ( texture.textureImage, format,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
			device->copyBufferToImage ( stagingBuffer, texture.textureImage,
				static_cast<uint32_t>(atlasWidth), static_cast<uint32_t>(atlasHeight) );
			device->transitionImageLayout ( texture.textureImage, format,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

			vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
			vkFreeMemory ( device->device, stagingBufferMemory, nullptr );

			createImageView ( texture.textureImage, format, texture.imageView );
			createTextureSampler ( texture.sampler );
			return texture;
		}

		void TextManager::createImageView ( VkImage image, VkFormat format, VkImageView& imageView ) {
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

		void TextManager::createTextureSampler ( VkSampler& sampler ) {
			VkSamplerCreateInfo samplerInfo {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_NEAREST; //VK_FILTER_NEAREST;// VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_NEAREST; // VK_FILTER_NEAREST;// VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;// VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;// VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;// VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_TRUE;// VK_FALSE;// VK_TRUE;
			samplerInfo.maxAnisotropy = 16;// 1.0f;// 16;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;// VK_SAMPLER_MIPMAP_MODE_NEAREST;// VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;


			samplerInfo.magFilter = VK_FILTER_LINEAR; // Usar filtrado lineal para interpolación
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; // Permitir mipmaps lineales
			//samplerInfo.maxLod = FLT_MAX;
			//samplerInfo.unnormalizedCoordinates = VK_TRUE;

			if (vkCreateSampler ( device->device, &samplerInfo, nullptr, &sampler ) != VK_SUCCESS) {
				throw std::runtime_error ( "No se pudo crear el sampler de la textura!" );
			}
		}

		VkPipeline TextManager::createGraphPipeline (
			VkVertexInputBindingDescription bindingDescription,
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions,
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
			rasterizer.cullMode = VK_CULL_MODE_NONE;

			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;// VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
}



