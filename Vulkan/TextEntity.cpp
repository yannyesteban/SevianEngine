#include "TextEntity.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <msdfgen.h>
#include <msdfgen-ext.h>



const std::string mainText = "Yanny@A.com";


// Función para guardar el atlas SDF como imagen
void saveSDFAtlas ( const std::vector<uint8_t>& atlasData, int atlasWidth, int atlasHeight, const std::string& filename, const std::string& filename2 ) {
	// Crear una imagen de OpenCV del tamaño del atlas, con 4 canales (RGBA)
	cv::Mat sdfImage ( atlasHeight, atlasWidth, CV_8UC4, (void*) atlasData.data () );

	// Guardar la imagen como PNG
	if (!cv::imwrite ( filename, sdfImage )) {
		std::cerr << "Error al guardar la imagen del atlas: " << filename << std::endl;
	}
	else {
		std::cout << "Atlas guardado correctamente como: " << filename << std::endl;
	}

	cv::flip ( sdfImage, sdfImage, 0 ); // Invertir verticalmente
	cv::imwrite ( filename2, sdfImage );
	
}
const int FONT_SIZE = 32;


using namespace msdfgen;
int msdfgenTest ( std::string src, char c ) {
	int texWidth = 32;
	int texHeight = 32;
	double size = 32;
	if (FreetypeHandle* ft = initializeFreetype ()) {
		if (FontHandle* font = loadFont ( ft, src.c_str () )) {
			Shape shape;
			if (loadGlyph ( shape, font, c, FONT_SCALING_EM_NORMALIZED )) {
				shape.normalize ();
				//                      max. angle
				//edgeColoringSimple ( shape, 3.0 );
				//          output width, height
				Bitmap<float, 3> msdf ( texWidth, texWidth );
				//                            scale, translation (in em's)
				Vector2 scale ( texWidth * 0.90, texWidth * 0.90 );
				SDFTransformation t ( Projection ( scale, Vector2 ( 0.125, 0.125 * 2 ) ), Range ( 0.125 ) );
				generateMSDF ( msdf, shape, t );


				std::string name = "char_test_";
				name += c;
				name += ".png";

				savePng ( msdf, name.c_str () );
			}
			destroyFont ( font );
		}
		deinitializeFreetype ( ft );
	}
	return 0;
}

void printTest ( Bitmap<float, 3> msdf, char c ) {
	if (c == '@' || c == 'A' || c == 'g' || c == 'e' || c == 'y' || c == 'o') {
		std::string name = "test_";
		name += c;
		name += ".png";
		//printf ( "altura: %c es : %f", c, height );
		savePng ( msdf, name.c_str () );
	}
}





namespace VULKAN {

	void TextEntity::fontInit ( AtlasInfo info ) {

		// Inicializar msdfgen y cargar la fuente TTF
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype ();
		if (!ft) {
			std::cerr << "No se pudo inicializar FreeType con msdfgen" << std::endl;
			return;
		}

		msdfgen::FontHandle* font = msdfgen::loadFont ( ft, info.fontPath.c_str () );
		if (!font) {
			std::cerr << "No se pudo cargar la fuente: " << info.fontPath << std::endl;
			msdfgen::deinitializeFreetype ( ft );
			return;
		}
		msdfgen::FontMetrics metrics;

		int deltaSize = ceil ( info.border * info.size * 2 );

		int texWidth = info.size + deltaSize;
		int texHeight = info.size + deltaSize;

		float realScale = texWidth / info.size;
		float pixelSize = 1 / info.size;
		float padding = 4 * pixelSize;

		int atlasWidth = texWidth * info.cols;
		int atlasHeight = texHeight * info.rows;

		msdfgen::getFontMetrics ( metrics, font, FONT_SCALING_EM_NORMALIZED );
		std::vector<uint8_t> msdfData ( atlasWidth * atlasHeight * 4 );

		int index = 0;
		//VkDeviceSize imageSize = msdfData.size ();

		for (unsigned char c = info.firstGlyph; c < info.lastGlyph; c++) {

			int col = (index % info.cols);
			int row = info.rows - floor ( index / info.rows ) - 1;
			int row2 = floor ( index / info.rows );
			
			double advance = 0;

			msdfgen::Shape shape;
			msdfgen::Bitmap<float, 3> msdf ( texWidth, texHeight );
			if (!msdfgen::loadGlyph ( shape, font, c, FONT_SCALING_EM_NORMALIZED, &advance )) {
				std::cerr << "No se pudo cargar el glifo para el carácter: " << c << std::endl;
				msdfgen::destroyFont ( font );
				msdfgen::deinitializeFreetype ( ft );
				return;
			}

			shape.normalize ();
			if (info.edgeColoring) {
				edgeColoringSimple ( shape, info.angleThreshold );
			}

			msdfgen::Shape::Bounds bounds = shape.getBounds ( info.border );

			float width = bounds.r - bounds.l;
			float height = bounds.t - bounds.b;

			float left;
			float bottom;

			if (c == 'A') {
				printf ( "" );

			}

			float factor = (float) info.size / texWidth;
			float texel = (float) texWidth / atlasWidth;
			float deltaX;
			float deltaY;

			if (info.centered) {
				left = (realScale - width) / 2.0 - bounds.l;     // Centrar horizontalmente
				bottom = (realScale - bounds.t + bounds.b) / 2.0 - bounds.b;   // Centrar verticalmente

				deltaX = (realScale - width) / 2 * factor;
				deltaY = (realScale - height) / 2 * factor;
			}
			else {
				left = -bounds.l;
				bottom = realScale - bounds.t;

				deltaX = 0.0;
				deltaY = 0.0;

			}

			float u0 = texel * (col + deltaX);
			float u1 = u0 + texel * factor * width;
			float v0 = texel * row2 + deltaY * texel;
			float v1 = v0 + texel * height * factor;

			AtlasGlyphInfo ch = { width, height, bounds.l, bounds.t, u0, v0, u1, v1, advance };
			Characters[c] = ch;

			Vector2 scale ( info.size, info.size );
			Vector2 translation ( left, bottom );
			msdfgen::SDFTransformation tr = msdfgen::SDFTransformation ( msdfgen::Projection ( scale, translation ), msdfgen::Range ( info.range ) );

			generateMSDF ( msdf, shape, tr );

			printTest ( msdf, c );
			
			int pos = row * atlasWidth * texWidth + col * texHeight;
			
			for (int y = 0; y < texHeight; ++y) {
				for (int x = 0; x < texWidth; ++x) {
					int pos0 = (pos + y * atlasWidth + x) * 4;
					//int atlasIndex = ((startY + y) * atlasWidth + (startX + x)) * 4;
					for (int channel = 0; channel < 3; ++channel) {
						msdfData[pos0 + channel] = msdfgen::pixelFloatToByte ( msdf ( x, y )[channel] );
					}
					msdfData[pos0 + 3] = 255; // Canal alfa (opaco)
				}
			}

			index++;

		}

		msdfgen::destroyFont ( font );
		msdfgen::deinitializeFreetype ( ft );

		saveSDFAtlas ( msdfData, atlasWidth, atlasWidth, "atlas_.png", "atlas_c.png" );

		

		//texture = *device->createTexture ( "char_test_g.png" );

		texture = createAtlasTexture ( msdfData, atlasWidth, atlasHeight );

		

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

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


	void TextEntity::draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position, Camera camera, uint32_t width, uint32_t height ) {

		static bool play = false;

		int size = 100;

		if (!play) {
			play = true;
			float x = -size * 2 + 50;
			float y = -size * 2 + 150;
			y = 400 - 200;
			x = -400;
			x = (width / 2.0) * -1;
			y = 500 - size;
			float scale = 1.0f * size;
			uint32_t indexOffset = 0;
			int i = 0;
			text = "BYan";

			if (mainText != "") {
				text = mainText;
			}
			std::vector<VertexTextOld> vertices;
			std::vector<uint32_t> indices;
			int step = 0;
			for (auto c = text.begin (); c != text.end (); c++) {
				AtlasGlyphInfo ch = Characters[*c];
				float xpos = x + ch.bearingX * scale;
				float ypos = y - (ch.height - ch.bearingY) * scale;

				float u0 = ch.u0;
				float v0 = ch.v0;
				float u1 = ch.u1;
				float v1 = ch.v1;

				float w = ch.width * scale;
				float h = ch.height * scale;


				int color = step % 1;
				color = 5;
				vertices.push_back ( { {xpos, ypos}, {u0, v1}, color } );             // Bottom-left
				vertices.push_back ( { {xpos + w, ypos}, {u1, v1}, color } );         // Bottom-right
				vertices.push_back ( { {xpos, ypos + h}, {u0, v0}, color } );         // Top-left
				vertices.push_back ( { {xpos + w, ypos + h}, {u1, v0}, color } );     // Top-right

				// Definir los índices del quad
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 3 );
				indices.push_back ( indexOffset + 2 );

				indexOffset += 4;

				
				x += ch.advance * scale;
				step++;
			}


			prop = init ( vertices, indices, texture );
			tx.push_back ( prop );
		}



		auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);
		if (vulkanProp) {

			Frame frame = frames[currentFrame];
			updateUniformBuffer ( vulkanProp->buffers[currentFrame].buffersMapped, position, camera, width, height );



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

	TextEntity::TextEntity ( FontRenderType type, std::string text, std::string font, Device* device, std::vector<Frame> frames, VkDescriptorPool descriptorPool ) :
		text ( text ),
		font ( font ),
		device ( device ),
		frames ( frames ),
		descriptorPool ( descriptorPool ) {

		
		AtlasInfo info { font };
		
		//fontInit ( info );

		AtlasGenerator atlas;

		f = atlas.create ( { info.fontPath } );

		texture = createAtlasTexture ( f.atlas.data, f.atlas.width, f.atlas.height );

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		Characters = f.characters;
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



	void TextEntity::render ( UniformBufferObject ubo ) {
	}

	

	void TextEntity::ShadowRender ( UniformBufferObject ubo ) {
	}
	

	
	std::shared_ptr<Entity3D> TextEntity::init ( std::vector<VertexTextOld> vertices, std::vector<uint32_t> indices, VulkanTexture texture ) {
		auto vulkanProp = std::make_shared<VulkanProperty> ();

		std::vector<VulkanUBuffer>  x = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );

		//vulkanProp->descriptorSets = device->createDescriptorSets ( x, texture.textureImageView, texture.textureSampler, sizeof ( UniformBufferObject2 ) );

		std::vector<DSInfo> bufDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, x, sizeof ( x ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		//std::vector<DSInfo> texDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, x, sizeof ( x ), texture.imageView, texture.sampler, 1 } );
		static int timer = 0;

		std::cout << " timer ........ " << timer++ << "\n\n";
		vulkanProp->descriptorSets = device->createDescriptorSets ( descriptorSetLayout, bufDSInfo );


		auto attributeDescriptions = VertexText::getAttributeDescriptions ();
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

	

	

	void TextEntity::createImageView ( VkImage image, VkFormat format, VkImageView& imageView ) {
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

	void TextEntity::createTextureSampler ( VkSampler& sampler ) {
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

	VkPipeline TextEntity::createGraphPipeline (
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


	
	VulkanTexture TextEntity::createAtlasTexture ( std::vector<uint8_t> msdfData, int atlasWidth, int atlasHeight ) {
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

	void TextEntity::updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position,
		Camera camera, uint32_t width, uint32_t height ) {
		static auto startTime = std::chrono::high_resolution_clock::now ();

		// Calcular el tiempo transcurrido
		auto currentTime = std::chrono::high_resolution_clock::now ();
		float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

		UniformBufferObject2 ubo {};

		// Calcular el aspecto
		float aspectRatio = width / static_cast<float>(height);

		// Elegir el tipo de proyección (comentar/descomentar según se necesite)
		// Proyección Perspectiva (comentada si usas ortográfica)
		// ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
		// ubo.proj[1][1] *= -1;  // Invertir el eje Y para el espacio de recorte de Vulkan
		auto delta = 300.0f;
		// Proyección Ortográfica (descomentada si usas ortográfica)
		ubo.proj = glm::ortho ( -delta * aspectRatio, delta * aspectRatio, -delta, delta, 1.0f, 5.0f );
		//ubo.proj = glm::ortho ( 0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height) );
		ubo.proj = glm::ortho ( -static_cast<float>(width), static_cast<float>(width), -static_cast<float>(height), static_cast<float>(height), 5.0f, -5.0f );
		ubo.proj = glm::ortho ( -static_cast<float>(width), static_cast<float>(width), -static_cast<float>(height), static_cast<float>(height) );

		auto deltaX = static_cast<float>(width) / 2;
		auto deltaY = static_cast<float>(height) / 2;
		ubo.proj = glm::ortho ( -deltaX, deltaX, -deltaY, deltaY, 1.0f, 25.0f );

		ubo.proj = glm::ortho ( -deltaX, deltaX, -deltaY, deltaY );
		// Sin invertir el eje Y para una proyección ortográfica
		ubo.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o OpenGL con un espacio de recorte diferente

		// Modelo y Vista (sin rotación en este caso)
		ubo.model = glm::mat4 ( 1.0f );  // No hay rotación aplicada
		ubo.view = glm::lookAt ( glm::vec3 ( 0.0f, 0.0f, 2.0f ),  // Cámara desde (0, 0, 2)
			glm::vec3 ( 0.0f, 0.0f, 0.0f ),  // Mira al origen
			glm::vec3 ( 0.0f, 1.0f, 0.0f ) ); // "Arriba" en la dirección Y

		// Copiar los datos del UBO en la memoria mapeada
		memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
	}

}