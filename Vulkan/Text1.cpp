#include "Text1.h"
#include "ElapsedTimer.h"



namespace VULKAN {

	void Text::paint ( float x, float y, std::vector<Quad> quads ) {

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

			vertices.push_back ( { {xpos, ypos}, {u0, v1}, color } );             // Bottom-left
			vertices.push_back ( { {xpos + w, ypos}, {u1, v1}, color } );         // Bottom-right
			vertices.push_back ( { {xpos, ypos + h}, {u0, v0}, color } );         // Top-left
			vertices.push_back ( { {xpos + w, ypos + h}, {u1, v0}, color } );     // Top-right

			indices.push_back ( indexOffset + 0 );
			indices.push_back ( indexOffset + 1 );
			indices.push_back ( indexOffset + 2 );
			indices.push_back ( indexOffset + 1 );
			indices.push_back ( indexOffset + 3 );
			indices.push_back ( indexOffset + 2 );

			indexOffset += 4;
		}

		elements.push_back ( init ( vertices, indices, texture ) );
	}


	Text::Text ( Device* device, Propertys propertys, VulkanTexture texture, Font font, std::string text ) :
		device ( device ), propertys ( propertys ), texture ( texture ), font ( font ), text ( text ) {

		auto timer = ElapsedTimer ();
		timer.start ();
		
		SEVIAN::TEXT::Box box { 800, 500 };
		box.addText ( "En lugar de crear un objeto temporal y luego copiarlo o moverlo al contenedor, emplace_back construye el objeto directamente dentro del contenedor, utilizando los argumentos que se le pasan.  Esto puede ser significativamente mas eficiente, ", font, 36 );

		box.addText ( "especialmente para tipos de datos complejos o cuando se trabaja con grandes contenedores.", font, 36 );
		box.render ();
		paint ( -400, -0, box.getQuads () );
		box.format ( JUSTIFY );
		timer.mark ();

	}

	void Text::render ( uint32_t width, uint32_t height ) {
		uint32_t frameIndex = device->currentIndex ();
		for (const auto& elemento : elements) {
			//std::cout << elemento << " ";
			auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(elemento);
			if (vulkanProp) {

				Frame frame = device->getFrame ();
				updateUniformBuffer ( vulkanProp->buffers[frameIndex].buffersMapped, glm::vec3 ( 0.0f ), width, height );



				VkBuffer vertexBuffers[] = { vulkanProp->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, propertys.pipeline );
				vkCmdBindVertexBuffers ( frame.commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( frame.commandBuffer, vulkanProp->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, propertys.pipelineLayout, 0, 1, &vulkanProp->descriptorSets[frameIndex], 0, nullptr );
				vkCmdDrawIndexed ( frame.commandBuffer, static_cast<uint32_t>(vulkanProp->indicesSizes), 1, 0, 0, 0 );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}
		}

	}

	std::shared_ptr<Entity3D> Text::init ( std::vector<VertexText> vertices, std::vector<uint32_t> indices, VulkanTexture texture ) {
		auto vulkanProp = std::make_shared<VulkanProperty> ();

		std::vector<VulkanUBuffer>  vulkanBuffer = device->createUniformBuffer ( sizeof ( UniformBufferObject2 ) );

		//vulkanProp->descriptorSets = device->createDescriptorSets ( x, texture.textureImageView, texture.textureSampler, sizeof ( UniformBufferObject2 ) );

		std::vector<DSInfo> bufDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vulkanBuffer, sizeof ( UniformBufferObject2 ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		//std::vector<DSInfo> texDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, vulkanBuffer, sizeof ( UniformBufferObject2 ), texture.imageView, texture.sampler, 1 } );
		
		//auto g = device->createDescriptorSets ( propertys.bufDescriptorSetLayout, bufDSInfo );


		vulkanProp->descriptorSets = device->createDescriptorSets ( propertys.bufDescriptorSetLayout, bufDSInfo );


		auto attributeDescriptions = VertexText::getAttributeDescriptions ();
		//auto pipeline = createGraphicsPipeline3 ( getBindingDescription2 (), attributeDescriptions, device->descriptorSetLayout );

		vulkanProp->vertex = device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		vulkanProp->indices = device->createDataBuffer ( (void*) indices.data (), sizeof ( indices[0] ) * indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		vulkanProp->indicesSizes = indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

		//vulkanProp->pipeline = pipeline.pipeline;
		//vulkanProp->pipelineLayout = pipeline.pipelineLayout;
		vulkanProp->buffers = vulkanBuffer;

		return vulkanProp;


	}

	void Text::updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position, uint32_t width, uint32_t height ) {
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


