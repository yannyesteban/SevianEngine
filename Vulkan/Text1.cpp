#include "Text1.h"




namespace VULKAN {

	void Text::Text2 ( std::vector<std::vector<Quad>> lines ) {

		int size = 100;



		
		float scale = 1.0f * size;
		uint32_t indexOffset = 0;
		float x = 0.0f;
		float y = 0.0f;
		std::vector<VertexText> vertices;
		std::vector<uint32_t> indices;
		for (auto& line : lines) {
			
			for (auto& quad : line) {


				AtlasGlyphInfo ch = font.characters[quad.c];
				float xpos = quad.xPos ;
				float ypos = quad.yPos ;

				float u0 = ch.u0;
				float v0 = ch.v0;
				float u1 = ch.u1;
				float v1 = ch.v1;

				float w = quad.w;
				float h = quad.h;


				int color =5;
			
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
			}
		}
		elements.push_back ( init ( vertices, indices, texture ) );


		



	}

	Text::Text ( Device* device, Propertys propertys, VulkanTexture texture, Font font, std::string text ) :
		device ( device ), propertys ( propertys ), texture ( texture ), font ( font ), text ( text ) {




		TextLayout layout ( font, 800, 800 );

		auto word = layout.addText ( "yanny esteban nunez jimenez hoy es un gran dia" );

		SEVIAN::TEXT::Box box { 500, 600 };
		//Text2 ( word );
		box.addText ( "ahora que tengo terminado mi render text, mi nueva caja esta demasiado pequena ", font, 32 );
		box.addText ( "ahora mi old caja es demasiado grande.", font, 48 );
		
		Text2 ( box.getLines () );

		return;
		int size = 100;



		float x = 0.0f;
		float y = 0.0f;

		float scale = 1.0f * size;
		uint32_t indexOffset = 0;
		int i = 0;
		//text = "BYan";


		std::vector<VertexText> vertices;
		std::vector<uint32_t> indices;
		int step = 0;
		for (auto c = text.begin (); c != text.end (); c++) {
			AtlasGlyphInfo ch = font.characters[*c];
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

		elements.push_back ( init ( vertices, indices, texture ) );
		//prop = init ( vertices, indices, texture );
		//tx.push_back ( prop );



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
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vulkanBuffer, sizeof ( vulkanBuffer ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		//std::vector<DSInfo> texDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, vulkanBuffer, sizeof ( vulkanBuffer ), texture.imageView, texture.sampler, 1 } );
		static int timer = 0;

		std::cout << " timer ........ " << timer++ << "\n\n";
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

static std::vector<std::string> split ( const std::string& s ) {
	std::vector<std::string> words;
	std::string word;

	for (char c : s) {



		if (std::isspace ( c )) {
			if (!word.empty ()) {
				words.push_back ( word );
				word.clear ();
			}
		}
		else {
			word += c;
		}
	}

	if (!word.empty ()) {
		words.push_back ( word );
	}

	return words;
}

std::vector<std::vector<Quad>> TextLayout::addText ( std::string text ) {


	float width = 500.0f;

	float lineWidth = 0.0f;
	
	int line = 0;
	float x = 0.0f;
	float y = 0.0f;

	float w = 0.0f;
	std::vector<Quad> quads {};

	std::vector<std::vector<Quad>> lines {};

	int size = 100;




	float scale = 1.0f * size;

	for (char c : text) {

		if (c == '\n') { // Si es un carácter de nueva línea
			// Almacenar la línea actual en la lista de líneas
			lines.push_back ( quads );
			quads.clear (); // Limpiar los quads para la nueva línea

			// Reiniciar la posición horizontal y mover la posición vertical hacia abajo
			x = 0.0f;
			y -= font.lineHeight * scale; // Mover hacia abajo (ajusta según la altura de la línea)
			line++; // Incrementar el número de línea
			continue; // Saltar al siguiente carácter
		}

		Quad q;
		AtlasGlyphInfo ch = font.characters[c];
		q.c = c;
		q.ch = ch;
		q.line = line;
		float xpos = x + ch.bearingX * scale;
		float ypos = y - (ch.height - ch.bearingY) * scale;

		q.xPos = xpos;
		q.yPos = ypos;



		x += ch.advance * scale;
		w += x;

		if (std::isspace ( c ) && x > width) {
			lines.push_back ( quads );
			quads.clear ();
			y -= font.lineHeight * scale;
			x = 0.0;
			line++;



		}

		quads.push_back ( q );

	}

	if (!quads.empty ()) {
		lines.push_back ( quads );
	}

	std::vector words = split ( text );

	for (auto& word : words) {
		std::cout << "palabra " << word << " tiene: " << wordSize ( word ) << "\n";

	}

	return lines;
}

struct Palabra
{
	std::vector<Quad> quads {};
};

float TextLayout::wordSize ( std::string text ) {

	float x = 0.0f;
	float y = 0.0f;
	float scale = 1.0f;

	float width = 0.0f;


	for (auto c = text.begin (); c != text.end (); c++) {

		Quad q;
		AtlasGlyphInfo ch = font.characters[*c];
		float xpos = x + ch.bearingX * scale;
		float ypos = y - (ch.height - ch.bearingY) * scale;

		float u0 = ch.u0;
		float v0 = ch.v0;
		float u1 = ch.u1;
		float v1 = ch.v1;

		float w = ch.width * scale;
		float h = ch.height * scale;




		x += ch.advance * scale;
		width += x;



	}

	return width;
}
