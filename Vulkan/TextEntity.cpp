#include "TextEntity.h"
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

// Función para guardar el atlas SDF como imagen
void saveSDFAtlas ( const std::vector<uint8_t>& atlasData, int atlasWidth, int atlasHeight, const std::string& filename ) {
	// Crear una imagen de OpenCV del tamaño del atlas, con 4 canales (RGBA)
	cv::Mat sdfImage ( atlasHeight, atlasWidth, CV_8UC4, (void*) atlasData.data () );

	// Guardar la imagen como PNG
	if (!cv::imwrite ( filename, sdfImage )) {
		std::cerr << "Error al guardar la imagen del atlas: " << filename << std::endl;
	}
	else {
		std::cout << "Atlas guardado correctamente como: " << filename << std::endl;
	}
}
const int FONT_SIZE = 32;
#include <msdfgen.h>
#include <msdfgen-ext.h>
const std::string mainText = "Barrio";
using namespace msdfgen;
int main1 ( std::string src, char c ) {
	if (FreetypeHandle* ft = initializeFreetype ()) {
		if (FontHandle* font = loadFont ( ft, src.c_str () )) {
			Shape shape;
			if (loadGlyph ( shape, font, c, FONT_SCALING_EM_NORMALIZED )) {
				shape.normalize ();
				//                      max. angle
				edgeColoringSimple ( shape, 3.0 );
				//          output width, height
				Bitmap<float, 3> msdf ( 64, 64 );
				//                            scale, translation (in em's)
				SDFTransformation t ( Projection ( 64.0, Vector2 ( 0.125 * 2, 0.125 * 2 ) ), Range ( 0.125 * 2 ) );
				generateMSDF ( msdf, shape, t );

				std::string name = "char_o_";
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

void updateUniformBuffer5 ( void* uniformBuffersMapped, glm::vec3 position,
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


void updateUniformBuffer51 ( void* uniformBuffersMapped, glm::vec3 position,
	Camera camera, uint32_t width, uint32_t height ) {
	static auto startTime = std::chrono::high_resolution_clock::now ();

	auto currentTime = std::chrono::high_resolution_clock::now ();
	float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

	UniformBufferObject2 ubo {};

	// Calcular el ratio de aspecto
	float aspectRatio = width / static_cast<float>(height);

	// Usar una proyección ortográfica
	glm::mat4 projection = glm::ortho ( -aspectRatio, aspectRatio, -1.0f, 1.0f, 0.0f, 1.0f );

	// Modelo de rotación (si es necesario)
	ubo.model = glm::rotate ( glm::mat4 ( 1.0f ), time * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

	// Vista de la cámara
	//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
	ubo.view = glm::lookAt ( glm::vec3 ( 0.0f, 0.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ) );

	// Asignar la proyección ortográfica calculada
	ubo.proj = projection;

	// Copiar el UBO a la memoria
	memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
}


void updateUniformBuffer5no ( void* uniformBuffersMapped, glm::vec3 position,
	Camera camera, uint32_t width, uint32_t height ) {
	static auto startTime = std::chrono::high_resolution_clock::now ();

	auto currentTime = std::chrono::high_resolution_clock::now ();
	float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

	UniformBufferObject2 ubo {};

	float aspectRatio = width / static_cast<float>(height);
	glm::mat4 projection;

	if (aspectRatio > 1.0f) {
		projection = glm::ortho ( -1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.0f, 1.0f );
	}
	else {
		projection = glm::ortho ( -1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, 0.0f, 1.0f );
	}

	ubo.model = glm::rotate ( glm::mat4 ( 1.0f ), time * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
	ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
	ubo.proj = projection;
	ubo.proj[1][1] *= -1;

	memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
}

void updateUniformBuffer5si ( void* uniformBuffersMapped, glm::vec3 position,
	Camera camera, uint32_t width, uint32_t height ) {
	static auto startTime = std::chrono::high_resolution_clock::now ();
	auto currentTime = std::chrono::high_resolution_clock::now ();
	float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

	UniformBufferObject2 ubo {};

	// Aspect ratio
	float aspectRatio = width / static_cast<float>(height);

	// Choose the projection type (uncomment the one you need)
	// Perspective projection
	ubo.proj = glm::perspective ( glm::radians ( 45.0f ), aspectRatio, 0.1f, 10.0f );
	ubo.proj[1][1] *= -1; // Invert Y-axis for Vulkan clip space

	// Ortho projection (if needed)
	// ubo.proj = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.0f, 1.0f);

	// View and Model
	ubo.model = glm::mat4 ( 1.0f ); // No rotation
	ubo.view = glm::lookAt ( glm::vec3 ( 0.0f, 0.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ) );

	// Copy the UBO to the mapped memory
	memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
}


void updateUniformBuffer511 ( void* uniformBuffersMapped, glm::vec3 position,
	Camera camera, uint32_t width, uint32_t height ) {
	static auto startTime = std::chrono::high_resolution_clock::now ();

	auto currentTime = std::chrono::high_resolution_clock::now ();
	float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

	UniformBufferObject2 ubo {};


	float aspectRatio = width / static_cast<float>(height);
	glm::mat4 projection = glm::ortho ( -1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.0f, 1.0f );



	ubo.model = glm::rotate ( glm::mat4 ( 1.0f ), time * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

	ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
	ubo.proj = glm::perspective ( glm::radians ( 45.0f ), width / (float) height, 0.1f, 10.0f );
	//ubo.proj = projection;
	ubo.proj[1][1] *= -1;




	memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
}



namespace VULKAN {
	void TextEntity::fontInit2 ( std::string fontPath ) {
		// Inicializar msdfgen y cargar la fuente TTF
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype ();
		if (!ft) {
			std::cerr << "No se pudo inicializar FreeType con msdfgen" << std::endl;
			return;
		}

		msdfgen::FontHandle* font = msdfgen::loadFont ( ft, fontPath.c_str () );
		if (!font) {
			std::cerr << "No se pudo cargar la fuente: " << fontPath << std::endl;
			msdfgen::deinitializeFreetype ( ft );
			return;
		}

		msdfgen::FontMetrics metrics;
		msdfgen::getFontMetrics ( metrics, font, FONT_SCALING_EM_NORMALIZED );

		int texWidth = 32;
		int texHeight = 32;
		int atlasWidth = 512;
		int atlasHeight = 512;

		std::vector<uint8_t> msdfData ( atlasWidth * atlasHeight * 4, 0 );
		int offsetX = 0;
		int offsetY = 0;
		int maxRowHeight = 0;

		for (unsigned char c = 33; c < 128; c++) {
			// Cargar el glifo del carácter
			msdfgen::Shape shape;
			double advance = 0;
			if (!msdfgen::loadGlyph ( shape, font, c, FONT_SCALING_EM_NORMALIZED, &advance )) {
				std::cerr << "No se pudo cargar el glifo para el carácter: " << c << std::endl;
				continue;
			}

			shape.normalize ();
			edgeColoringSimple ( shape, 3.0 );

			// Obtener los límites del glifo
			msdfgen::Shape::Bounds bounds = shape.getBounds ();
			int width = static_cast<int>(ceil ( bounds.r - bounds.l ));
			int height = static_cast<int>(ceil ( bounds.t - bounds.b ));

			if (offsetX + width >= atlasWidth) {
				offsetX = 0;
				offsetY += maxRowHeight;
				maxRowHeight = 0;
			}
			if (offsetY + height >= atlasHeight) {
				std::cerr << "El atlas es demasiado pequeño para contener todos los glifos." << std::endl;
				break;
			}

			maxRowHeight = std::max ( maxRowHeight, height );

			msdfgen::Bitmap<float, 3> msdf ( texWidth, texHeight );
			//msdfgen::generateMSDF ( msdf, shape, 4.0, 1.0, msdfgen::Vector2 ( -bounds.l, -bounds.b ) );


			msdfgen::SDFTransformation tr ( msdfgen::Projection ( 32.0, msdfgen::Vector2 ( 0.125, 0.125 ) ), msdfgen::Range ( 0.125 ) );
			generateMSDF ( msdf, shape, tr );
			// Copiar los datos del glifo al atlas
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					for (int channel = 0; channel < 3; ++channel) {
						msdfData[((offsetY + y) * atlasWidth + (offsetX + x)) * 4 + channel] =
							msdfgen::pixelFloatToByte ( msdf ( x, y )[channel] );
					}
					msdfData[((offsetY + y) * atlasWidth + (offsetX + x)) * 4 + 3] = 255; // Canal alfa
				}
			}

			Glyph glyph;
			glyph.width = width;
			glyph.height = height;
			glyph.bearingX = static_cast<int>(bounds.l);
			glyph.bearingY = static_cast<int>(bounds.t);
			glyph.advance = static_cast<long>(advance);
			Characters[c] = glyph;

			offsetX += width;
		}

		VkDeviceSize imageSize = msdfData.size ();

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
		device->createImage (
			atlasWidth, atlasHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

		// Transiciones de layouts y copia de buffer a imagen
		device->transitionImageLayout ( character.textureImage, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
		device->copyBufferToImage ( stagingBuffer, character.textureImage,
			static_cast<uint32_t>(atlasWidth), static_cast<uint32_t>(atlasHeight) );
		device->transitionImageLayout ( character.textureImage, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );

		createImageView ( character.textureImage, VK_FORMAT_R8G8B8A8_UNORM, character.textureImageView );
		createTextureSampler ( character.textureSampler );

		msdfgen::destroyFont ( font );
		msdfgen::deinitializeFreetype ( ft );

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
		pipelineLayout = device->createPipelineLayout ( layouts );

		pipeline = createGraphPipeline (
			getBindingDescription2 (),
			getAttributeDescriptions2 (),
			pipelineLayout,
			"shaders/msdf.vert.spv",
			"shaders/msdf.frag.spv"
		);
	}


	void TextEntity::fontInit ( std::string fontPath ) {
		// Inicializar msdfgen y cargar la fuente TTF
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype ();
		if (!ft) {
			std::cerr << "No se pudo inicializar FreeType con msdfgen" << std::endl;
			return;
		}

		msdfgen::FontHandle* font = msdfgen::loadFont ( ft, fontPath.c_str () );
		if (!font) {
			std::cerr << "No se pudo cargar la fuente: " << fontPath << std::endl;
			msdfgen::deinitializeFreetype ( ft );
			return;
		}
		//std::map<char, Glyph> Characters;
		msdfgen::FontMetrics metrics;
		int texWidth = 32;
		int texHeight = 32;
		int atlasWidth = 320;
		int atlasHeight = 320;

		msdfgen::getFontMetrics ( metrics, font, FONT_SCALING_EM_NORMALIZED );
		std::vector<uint8_t> msdfData ( texWidth * texHeight * 4 * 256 );
		int index = 0;
		VkDeviceSize imageSize = msdfData.size ();
		for (unsigned char c = 66; c < 128; c++) {
			double border = 0.0;
			// Cargar el glifo del carácter
			msdfgen::Shape shape;
			double advance = 0;
			msdfgen::FontMetrics metrics;
			msdfgen::getFontMetrics ( metrics, font, FONT_SCALING_EM_NORMALIZED );
			msdfgen::Shape::Bounds bounds;

			if (!msdfgen::loadGlyph ( shape, font, c, FONT_SCALING_EM_NORMALIZED, &advance )) {
				std::cerr << "No se pudo cargar el glifo para el carácter: " << c << std::endl;
				msdfgen::destroyFont ( font );
				msdfgen::deinitializeFreetype ( ft );
				return;
			}
			shape.normalize ();
			edgeColoringSimple ( shape, 3.0 );
			bounds = shape.getBounds ( border );
			char test = 66;

			// Obtener el bounding box del Shape
			double l, b, r, t;
			shape.bound ( l, b, r, t ); // Devuelve los límites izquierdo, inferior, derecho y superior del glifo en unidades EM

			// Calcular propiedades del glifo en píxeles
			int width = (int) ceil ( bounds.r - bounds.l );        // Ancho del glifo
			int height = (int) ceil ( bounds.t - bounds.b );       // Alto del glifo
			int bearingX = (int) floor ( bounds.l );        // Desplazamiento horizontal desde el origen hasta el inicio del glifo
			int bearingY = (int) ceil ( bounds.t );         // Desplazamiento vertical desde el origen hasta la línea de base



			// Asignar propiedades al objeto Glyph
			Glyph glyph;
			glyph.width = width * 32;
			glyph.height = height * 32;
			glyph.bearingX = bounds.l * 32;
			glyph.bearingY = bounds.t * 32;
			glyph.advance = (long) advance * 32;


			if (c == 103) {
				std::cout << "";
				Characters[c].width = ceil ( bounds.r * 32 - bounds.l * 32 );
			}

			Characters[c].width = ceil ( bounds.r * 32 - bounds.l * 32 );
			Characters[c].height = ceil ( bounds.t * 32 - bounds.b * 32 );
			Characters[c].bearingX = bounds.l * 32;
			Characters[c].bearingY = bounds.t * 32;

			Characters[c].advance = Characters[c].advance >> 6;// advance * 20;



			msdfgen::Bitmap<float, 3> msdf ( texWidth, texHeight );
			//msdfgen::generateMSDF(msdf, shape, border_width, 1.0, msdfgen::Vector2(-bounds.l, -bounds.b));
			msdfgen::SDFTransformation tr ( msdfgen::Projection ( 32.0, msdfgen::Vector2 ( 0.125, 0.125 ) ), msdfgen::Range ( 0.125 ) );
			generateMSDF ( msdf, shape, tr );

			/*std::string name = "char2_";
			name += c;
			name += ".png";
			main1 ( fontPath, test );
			savePng ( msdf, name.c_str () );*/


			for (int y = 0; y < texHeight; ++y) {
				for (int x = 0; x < texWidth; ++x) {
					for (int channel = 0; channel < 3; ++channel) {

						//msdfData[(x + texWidth * y) * 4 + channel] = msdfgen::pixelFloatToByte ( msdf ( x, y )[channel] );
						msdfData[index++] = msdfgen::pixelFloatToByte ( msdf ( x, y )[channel] );
					}
					//msdfData[(y * texWidth + x) * 4 + 3] = 255; // Canal alfa (completamente opaco)
					msdfData[index++] = 255; // Canal alfa (completamente opaco)
				}
			}
		}

		cv::Mat sdfImage ( atlasHeight, atlasWidth, CV_8UC4, msdfData.data () );
		cv::imwrite ( "atlas.png", sdfImage );
		//saveSDFAtlas ( msdfData, atlasWidth, atlasWidth, "atlas.png" );
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
		device->createImage ( texWidth, texHeight, format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

		// Transiciones de layouts y copia de buffer a imagen
		device->transitionImageLayout ( character.textureImage, format,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
		device->copyBufferToImage ( stagingBuffer, character.textureImage,
			static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
		device->transitionImageLayout ( character.textureImage, format,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );

		createImageView ( character.textureImage, format, character.textureImageView );
		createTextureSampler ( character.textureSampler );

		msdfgen::destroyFont ( font );
		msdfgen::deinitializeFreetype ( ft );

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
		pipelineLayout = device->createPipelineLayout ( layouts );

		pipeline = createGraphPipeline (
			getBindingDescription2 (),
			getAttributeDescriptions2 (),
			pipelineLayout,
			"shaders/msdf.vert.spv",
			"shaders/msdf.frag.spv"
		);
	}

	TextEntity::TextEntity ( FontRenderType type, std::string text, std::string font, Device* device, std::vector<Frame> frames, VkDescriptorPool descriptorPool ) :
		text ( text ),
		font ( font ),
		device ( device ),
		frames ( frames ),
		descriptorPool ( descriptorPool ) {
		fontInit ( font );

		return;
		auto error = FT_Init_FreeType ( &library );
		if (error) {
			std::cout << "error";
		}
		else {
			std::cout << "todo bien";
		}

		error = FT_New_Face ( library, font.c_str (), 0, &face );

		if (error == FT_Err_Unknown_File_Format) {
			std::cout << "todo bien 2";
		}
		else if (error) {
			std::cout << "error 2";
		}

		FT_Set_Pixel_Sizes ( face, 0, FONT_SIZE );

		FT_GlyphSlot slot = face->glyph;

		for (unsigned char c = 33; c < 128; c++) {
			if (FT_Load_Char ( face, c, FT_LOAD_RENDER )) {
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			if (FT_Render_Glyph ( slot, FT_RENDER_MODE_SDF )) {
				std::cerr << "ERROR::FREETYTPE: Failed to render Glyph " << c << std::endl;
				continue;
			}

			initGlyph ( face, c, Characters[c] );

			createTextureImage2 ( font, c, Characters[c] );
		}

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
		pipelineLayout = device->createPipelineLayout ( layouts );

		pipeline = createGraphPipeline (
			getBindingDescription2 (),
			getAttributeDescriptions2 (),
			pipelineLayout,
			"shaders/msdf.vert.spv",
			"shaders/msdf.frag.spv"
		);
	}


	TextEntity::TextEntity ( std::string text, std::string font, Device* device, std::vector<Frame> frames, VkDescriptorPool descriptorPool ) :
		text ( text ),
		font ( font ),
		device ( device ),
		frames ( frames ),
		descriptorPool ( descriptorPool ) {

		auto error = FT_Init_FreeType ( &library );
		if (error) {
			std::cout << "error";
		}
		else {
			std::cout << "todo bien";
		}

		error = FT_New_Face ( library, font.c_str (), 0, &face );

		if (error == FT_Err_Unknown_File_Format) {
			std::cout << "todo bien 2";
		}
		else if (error) {
			std::cout << "error 2";
		}

		FT_Set_Pixel_Sizes ( face, 0, FONT_SIZE );

		FT_GlyphSlot slot = face->glyph;

		for (unsigned char c = 33; c < 128; c++) {
			if (FT_Load_Char ( face, c, FT_LOAD_RENDER )) {
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			if (FT_Render_Glyph ( slot, FT_RENDER_MODE_SDF )) {
				std::cerr << "ERROR::FREETYTPE: Failed to render Glyph " << c << std::endl;
				continue;
			}

			initGlyph ( face, c, Characters[c] );

			createTextureImage2 ( font, c, Characters[c] );
		}

		std::vector<DSLInfo> bufDSLInfo;
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 } );
		bufDSLInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } );

		descriptorSetLayout = device->createDescriptorSetLayout ( bufDSLInfo );
		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

		std::vector<VkDescriptorSetLayout> layouts = { descriptorSetLayout };
		pipelineLayout = device->createPipelineLayout ( layouts );

		pipeline = createGraphPipeline (
			getBindingDescription2 (),
			getAttributeDescriptions2 (),
			pipelineLayout,
			"shaders/msdf.vert.spv",
			"shaders/msdf.frag.spv"
		);
	}


	void TextEntity::createTextureImage ( const std::string& fontPath, char c, Glyph& character ) {
		// Inicializar msdfgen y cargar la fuente TTF
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype ();
		if (!ft) {
			std::cerr << "No se pudo inicializar FreeType con msdfgen" << std::endl;
			return;
		}

		msdfgen::FontHandle* font = msdfgen::loadFont ( ft, fontPath.c_str () );
		if (!font) {
			std::cerr << "No se pudo cargar la fuente: " << fontPath << std::endl;
			msdfgen::deinitializeFreetype ( ft );
			return;
		}

		// Cargar el glifo del carácter
		msdfgen::Shape shape;
		double advance = 0;
		msdfgen::FontMetrics metrics;
		msdfgen::getFontMetrics ( metrics, font, FONT_SCALING_EM_NORMALIZED );
		msdfgen::Shape::Bounds bounds;

		if (!msdfgen::loadGlyph ( shape, font, c,
			//msdfgen::FontCoordinateScaling::FONT_SCALING_EM_NORMALIZED,
			&advance )) {
			std::cerr << "No se pudo cargar el glifo para el carácter: " << c << std::endl;
			msdfgen::destroyFont ( font );
			msdfgen::deinitializeFreetype ( ft );
			return;
		}
		shape.normalize ();
		edgeColoringSimple ( shape, 3.0 );
		bounds = shape.getBounds ( 0 );
		if (!msdfgen::loadGlyph ( shape, font, c,
			msdfgen::FontCoordinateScaling::FONT_SCALING_EM_NORMALIZED )) {
			std::cerr << "No se pudo cargar el glifo para el carácter: " << c << std::endl;
			msdfgen::destroyFont ( font );
			msdfgen::deinitializeFreetype ( ft );
			return;
		}
		shape.normalize ();
		edgeColoringSimple ( shape, 3.0 );

		char test = 66;


		character.width = 32;
		character.height = 32;
		character.advance = 32 * 100;


		character.width = ceil ( bounds.r - bounds.l );
		character.height = ceil ( bounds.t - bounds.b );
		character.advance = advance * 100;


		int texWidth = 32; // Ajusta el tamaño de textura según sea necesario
		int texHeight = 32;

		//edgeColoringSimple ( shape, 0.0 );
		msdfgen::Bitmap<float, 3> msdf ( texWidth, texHeight );
		//msdfgen::Bitmap<float, 3> msdf ( 32, 32 );
		float scale = 4.0 / texHeight;
		//msdfgen::SDFTransformation t ( msdfgen::Projection ( texHeight, msdfgen::Vector2 ( scale, scale ) ), msdfgen::Range ( scale ) );


		msdfgen::SDFTransformation t ( msdfgen::Projection ( 32.0, msdfgen::Vector2 ( 0.125, 0.125 ) ), msdfgen::Range ( 0.125 ) );
		//generateMSDF ( msdf, shape, t );


		//msdfgen::generateMSDF ( msdf, shape, 6.0, 1.0, msdfgen::Vector2 ( -0.0, 0.0 ) );
		msdfgen::generateMSDF ( msdf, shape, t );
		// Convertir el Bitmap de msdfgen a un buffer lineal


		if (c == test) {
			std::string name = "char5_";
			name += c;
			name += ".png";
			main1 ( fontPath, test );
			savePng ( msdf, name.c_str () );
		}


		VkDeviceSize imageSize = texWidth * texHeight * 3 * sizeof ( float ); // 3 canales (R, G, B) de 32 bits flotantes
		std::vector<float> msdfData ( texWidth * texHeight * 3 );
		for (int y = 0; y < texHeight; ++y) {
			for (int x = 0; x < texWidth; ++x) {
				for (int channel = 0; channel < 3; ++channel) {
					msdfData[(y * texWidth + x) * 3 + channel] = msdf ( x, y )[channel];
					if (c == test) {
						std::cout << " msdf ( x, y )[channel] " << msdf ( x, y )[channel] << "\n";
					}
				}
			}

		}

		// Crear el staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		device->createBuffer ( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory );

		// Mapear los datos de msdf al staging buffer
		void* data;
		vkMapMemory ( device->device, stagingBufferMemory, 0, imageSize, 0, &data );
		memcpy ( data, msdfData.data (), static_cast<size_t>(imageSize) );
		vkUnmapMemory ( device->device, stagingBufferMemory );

		// Crear la textura Vulkan
		device->createImage ( texWidth, texHeight, VK_FORMAT_R32G32B32_SFLOAT, VK_IMAGE_TILING_LINEAR/* VK_IMAGE_TILING_OPTIMAL*/,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

		// Transiciones de layouts y copia de buffer a imagen
		device->transitionImageLayout ( character.textureImage, VK_FORMAT_R32G32B32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
		device->copyBufferToImage ( stagingBuffer, character.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
		device->transitionImageLayout ( character.textureImage, VK_FORMAT_R32G32B32_SFLOAT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		// Liberar el staging buffer
		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );


		createImageView ( character.textureImage, VK_FORMAT_R32G32B32_SFLOAT, character.textureImageView );
		createTextureSampler ( character.textureSampler );


		// Liberar recursos de msdfgen
		msdfgen::destroyFont ( font );
		msdfgen::deinitializeFreetype ( ft );
	}
	/*
	FT_Library ft_library;
FT_Face face;

FT_Init_FreeType(&ft_library);
FT_New_Memory_Face(ft_library, data, (FT_Long)size, 0, &face);

unsigned int tex_width = 1024;
unsigned int tex_height = tex_width;
unsigned int buffer_size = tex_width * tex_height * 4;

// Create image buffer
uint8_t* pixels = new uint8_t[buffer_size]{0};
unsigned int pen_x = 0;
unsigned int pen_y = 0;
int max_height = 0;

msdfgen::FontHandle* font_handle = msdfgen::adoptFreetypeFont(face);
msdfgen::FontMetrics font_metrics;
msdfgen::getFontMetrics(font_metrics, font_handle);
_line_height = font_metrics.lineHeight;

FT_UInt gindex = 0;
FT_ULong charcode = FT_Get_First_Char(face, &gindex);

	// Go through all available characters in the font
while (gindex != 0)
{
	float glyph_width = 0;
	float glyph_height = 0;
	double advance = 0;
	float border_width = 4;
	// Next
	charcode = FT_Get_Next_Char(face, charcode, &gindex);
}


	*/


	void TextEntity::createTextureImage2 ( const std::string& fontPath, char c, Glyph& character ) {
		// Inicializar msdfgen y cargar la fuente TTF
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype ();
		if (!ft) {
			std::cerr << "No se pudo inicializar FreeType con msdfgen" << std::endl;
			return;
		}

		msdfgen::FontHandle* font = msdfgen::loadFont ( ft, fontPath.c_str () );
		if (!font) {
			std::cerr << "No se pudo cargar la fuente: " << fontPath << std::endl;
			msdfgen::deinitializeFreetype ( ft );
			return;
		}
		double border = 0.0;
		// Cargar el glifo del carácter
		msdfgen::Shape shape;
		double advance = 0;
		msdfgen::FontMetrics metrics;
		msdfgen::getFontMetrics ( metrics, font, FONT_SCALING_EM_NORMALIZED );
		msdfgen::Shape::Bounds bounds;

		if (!msdfgen::loadGlyph ( shape, font, c, FONT_SCALING_EM_NORMALIZED, &advance )) {
			std::cerr << "No se pudo cargar el glifo para el carácter: " << c << std::endl;
			msdfgen::destroyFont ( font );
			msdfgen::deinitializeFreetype ( ft );
			return;
		}
		shape.normalize ();
		edgeColoringSimple ( shape, 3.0 );
		bounds = shape.getBounds ( border );
		char test = 66;

		// Obtener el bounding box del Shape
		double l, b, r, t;
		shape.bound ( l, b, r, t ); // Devuelve los límites izquierdo, inferior, derecho y superior del glifo en unidades EM

		// Calcular propiedades del glifo en píxeles
		int width = (int) ceil ( bounds.r - bounds.l );        // Ancho del glifo
		int height = (int) ceil ( bounds.t - bounds.b );       // Alto del glifo
		int bearingX = (int) floor ( bounds.l );        // Desplazamiento horizontal desde el origen hasta el inicio del glifo
		int bearingY = (int) ceil ( bounds.t );         // Desplazamiento vertical desde el origen hasta la línea de base



		// Asignar propiedades al objeto Glyph
		Glyph glyph;
		glyph.width = width * 32;
		glyph.height = height * 32;
		glyph.bearingX = bounds.l * 32;
		glyph.bearingY = bounds.t * 32;
		glyph.advance = (long) advance * 32;


		if (c == 103) {
			std::cout << "";
			character.width = ceil ( bounds.r * 32 - bounds.l * 32 );
		}

		//character.width = ceil ( bounds.r - bounds.l );
		//character.height = ceil ( bounds.t - bounds.b );
		character.advance = character.advance >> 6;// advance * 20;

		// Normalizar el glifo y generar el mapa MSDF


		int texWidth = 32;
		int texHeight = 32;

		msdfgen::Bitmap<float, 3> msdf ( texWidth, texHeight );
		//msdfgen::generateMSDF(msdf, shape, border_width, 1.0, msdfgen::Vector2(-bounds.l, -bounds.b));
		msdfgen::SDFTransformation tr ( msdfgen::Projection ( 32.0, msdfgen::Vector2 ( 0.125, 0.125 ) ), msdfgen::Range ( 0.125 ) );
		generateMSDF ( msdf, shape, tr );
		//msdfgen::generateMSDF ( msdf, shape, 2, 1.0, msdfgen::Vector2 ( -16, -16 ) );
		if (c == test) {
			std::string name = "char2_";
			name += c;
			name += ".png";
			main1 ( fontPath, test );
			savePng ( msdf, name.c_str () );
		}

		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
		std::vector<uint8_t> msdfData ( texWidth * texHeight * 4 );
		/*

		for (int y = 0; y < texHeight; ++y) {
			for (int x = 0; x < texWidth; ++x) {
				int flippedY = texHeight - 1 - y;
				for (int channel = 0; channel < 3; ++channel) {
					msdfData[(y * texWidth + x) * 4 + channel] = static_cast<uint8_t>(msdf ( x, flippedY )[channel] * 255.0f);
				}
				msdfData[(y * texWidth + x) * 4 + 3] = 255;  // Alpha (completamente opaco)
			}
		}
		for (int y = 0; y < texHeight; ++y) {
			for (int x = 0; x < texWidth; ++x) {
				int flippedY = texHeight - 1 - y;
				for (int channel = 0; channel < 3; ++channel) {
					// Invertir el eje Y

					msdfData[(y * texWidth + x) * 3 + channel] = msdf ( x, flippedY )[channel];
				}
			}
		}
		*/
		//std::vector<uint8_t> msdfData ( texWidth * texHeight * 4 ); // 4 canales: RGBA
		VkDeviceSize imageSize = 0;

		auto img1 = msdfData.size ();
		auto img2 = texWidth * texHeight * 4 * sizeof ( uint8_t );

		imageSize = msdfData.size ();
		imageSize = texWidth * texHeight * 4 * sizeof ( uint8_t );
		//imageSize = msdfData.size ();
		for (int y = 0; y < texHeight; ++y) {
			for (int x = 0; x < texWidth; ++x) {
				for (int channel = 0; channel < 3; ++channel) {
					float floatValue = msdf ( x, y )[channel]; // Valor flotante original
					//float normalizedValue = (floatValue + 1.0f) / 2.0f; // Normalizar a rango [0, 1]
					float normalizedValue = (floatValue / 0.125f) * 0.5f + 0.5f;
					uint8_t unormValue = static_cast<uint8_t>(normalizedValue * 255.0f); // Convertir a 8 bits
					msdfData[(y * texWidth + x) * 4 + channel] = unormValue;
					//msdfData[(y * texWidth + x) * 4 + channel] = static_cast<uint8_t>(msdf ( x, y )[channel] * 255.0f);


					//msdfData[(y * texWidth + x) * 4 + channel] = static_cast<uint8_t>(std::clamp ( msdf ( x, y )[channel] * 255.0f, 0.0f, 255.0f ));

					msdfData[(x + texWidth * y) * 4 + channel] = msdfgen::pixelFloatToByte ( msdf ( x, y )[channel] );
				}
				msdfData[(y * texWidth + x) * 4 + 3] = 255; // Canal alfa (completamente opaco)
			}
		}


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
		device->createImage ( texWidth, texHeight, format, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

		// Transiciones de layouts y copia de buffer a imagen
		device->transitionImageLayout ( character.textureImage, format,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
		device->copyBufferToImage ( stagingBuffer, character.textureImage,
			static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
		device->transitionImageLayout ( character.textureImage, format,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );

		createImageView ( character.textureImage, format, character.textureImageView );
		createTextureSampler ( character.textureSampler );

		msdfgen::destroyFont ( font );
		msdfgen::deinitializeFreetype ( ft );
	}



	VkVertexInputBindingDescription TextEntity::getBindingDescription2 () {
		VkVertexInputBindingDescription bindingDescription {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof ( SEVIAN::VertexText );
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> TextEntity::getAttributeDescriptions2 () {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions ( 2, {} );
		/*    glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 color;*/
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof ( SEVIAN::VertexText, pos );

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof ( SEVIAN::VertexText, texCoord );



		return attributeDescriptions;
	}





	void TextEntity::render ( UniformBufferObject ubo ) {
	}

	void TextEntity::render2 ( UniformBufferObject ubo ) {
	}

	void TextEntity::ShadowRender ( UniformBufferObject ubo ) {
	}
	/*
	void TextEntity::draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position,
		Camera camera,
		uint32_t width,
		uint32_t height ) {

		static bool play = false;

		if (!play) {
			play = true;
			float x = 0;
			float y = 0;
			float scale = 0.006f;
			uint32_t indexOffset = 0;
			int i = 0;

			text = "A"; // Aquí puedes pasar el texto dinámico
			for (auto c = text.begin (); c != text.end (); c++) {

				std::vector<VertexText> vertices;
				std::vector<uint32_t> indices;

				std::cout << " - - - i " << i << "\n";
				Glyph ch = Characters[*c];

				// Ajustamos la posición Y, invirtiéndola para Vulkan
				float xpos = x + ch.bearingX * scale;
				float ypos = y - (ch.height - ch.bearingY) * scale;

				// Aseguramos que las coordenadas de las esquinas están bien ajustadas
				float w = ch.width * scale;
				float h = ch.height * scale;

				// Invertir la posición Y (Vulkan tiene el origen en la esquina inferior izquierda)
				float invertedYPos = y - ypos - h; // Inversión del eje Y

				// Definir los vértices del carácter, usando las coordenadas invertidas para Y
				vertices.push_back ( { {xpos, invertedYPos }, {0.0f, 0.0f} } ); // Bottom-left
				vertices.push_back ( { {xpos + w, invertedYPos}, {1.0f, 0.0f} } ); // Bottom-right
				vertices.push_back ( { {xpos, invertedYPos + h}, {0.0f, 1.0f} } ); // Top-left
				vertices.push_back ( { {xpos + w, invertedYPos + h}, {1.0f, 1.0f} } ); // Top-right

				indexOffset = 0;
				// Definir los índices del carácter
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 3 );
				indices.push_back ( indexOffset + 2 );

				indexOffset += 4;

				// Avanzamos en la posición X para el siguiente carácter
				x += (ch.advance) * scale;

				auto prop = init ( vertices, indices, ch );
				tx.push_back ( prop );

				i++;
			}
		}

		// Procesar las propiedades de los vértices e índices
		for (const auto& elemento : tx) {
			auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(elemento);
			if (vulkanProp) {
				Frame frame = frames[currentFrame];
				updateUniformBuffer5 ( vulkanProp->buffers[currentFrame].buffersMapped, position, camera, width, height );

				VkBuffer vertexBuffers[] = { vulkanProp->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );
				vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
				vkCmdBindIndexBuffer ( commandBuffer, vulkanProp->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &vulkanProp->descriptorSets[currentFrame], 0, nullptr );
				vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(vulkanProp->indicesSizes), 1, 0, 0, 0 );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}
		}
	}*/

	void TextEntity::draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position,
		Camera camera,
		uint32_t width,
		uint32_t height ) {

		static bool play = false;



		if (!play) {
			play = true;
			float x = 0.0;
			float y = 0.0;
			float scale = 1.0f * 4;
			uint32_t indexOffset = 0;
			int i = 0;
			text = "BYan";

			if (mainText != "") {
				text = mainText;
			}

			for (auto c = text.begin (); c != text.end (); c++) {

				std::vector<VertexText> vertices;
				std::vector<uint32_t> indices;


				std::cout << " - - - i " << i << "\n";
				Glyph ch = Characters[*c];

				float xpos = x + ch.bearingX * scale;


				float ypos = y - (ch.height - ch.bearingY) * scale;



				float w = ch.width * scale;
				float h = ch.height * scale;

				// Define los vértices para el carácter
				vertices.push_back ( { {xpos,     ypos    }, {0.0f, 0.0f} } ); // Bottom-left
				vertices.push_back ( { {xpos + w, ypos    }, {1.0f, 0.0f} } ); // Bottom-right
				vertices.push_back ( { {xpos,     ypos + h}, {0.0f, 1.0f} } ); // Top-left
				vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } ); // Top-right

				//vertices.push_back ( { {xpos,     ypos   }, {0.0f, 0.0f} } ); // Bottom-left
				//vertices.push_back ( { {xpos + w, ypos    }, {1.0f, 0.0f} } ); // Bottom-right
				//vertices.push_back ( { {xpos,     ypos + h}, {0.0f, 1.0f} } ); // Top-left
				//vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } ); // Top-right

				//vertices.push_back ( { {-0.1f,     -0.1f   }, {0.0f, 1.0f} } ); // Bottom-left
				//vertices.push_back ( { {0.1f, -0.1f    }, {1.0f, 1.0f} } ); // Bottom-right
				//vertices.push_back ( { {0.1f,     0.1f}, {1.0f, 0.0f} } ); // Top-left
				//vertices.push_back ( { {-0.1f, 0.1f}, {0.0f, 0.0f} } ); // Top-right



				//vertices.push_back ( { {-0.1f,     -0.1f   }, {0.0f, 0.0f} } );
				//vertices.push_back ( { {0.1f,     -0.1f   }, {1.0f, 0.0f} } );
				//vertices.push_back ( { {0.1f,     0.1f   }, {1.0f, 1.0f} } );
				//vertices.push_back ( { {-0.1f,     0.1f   }, {0.0f, 1.0f} } );

				indexOffset = 0;
				// Define los índices para el carácter
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );

				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 3 );
				indices.push_back ( indexOffset + 2 );

				indexOffset += 4;

				x += (ch.advance) * scale; // advance.x is in 1/64th pixels
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

	void TextEntity::draw2 ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position, Camera camera, uint32_t width, uint32_t height ) {

		static bool play = false;



		if (!play) {
			play = true;
			float x = -0.0;
			float y = 0.0;
			float scale = 1.0f * 4;
			uint32_t indexOffset = 0;
			int i = 0;
			text = "BYan";

			if (mainText != "") {
				text = mainText;
			}
			std::vector<VertexText> vertices;
			std::vector<uint32_t> indices;
			for (auto c = text.begin (); c != text.end (); c++) {





				std::cout << " - - - i " << i << "\n";
				Glyph ch = Characters[*c];
				ch.advance = 10;

				float xpos = x + ch.bearingX * scale;


				float ypos = y - (ch.height - ch.bearingY) * scale;



				float w = ch.width * scale;
				float h = ch.height * scale;

				// Define los vértices para el carácter
				vertices.push_back ( { {xpos,     ypos    }, {0.0f, 0.0f} } ); // Bottom-left
				vertices.push_back ( { {xpos + w, ypos    }, {1.0f, 0.0f} } ); // Bottom-right
				vertices.push_back ( { {xpos,     ypos + h}, {0.0f, 1.0f} } ); // Top-left
				vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } ); // Top-right

				//vertices.push_back ( { {xpos,     ypos   }, {0.0f, 0.0f} } ); // Bottom-left
				//vertices.push_back ( { {xpos + w, ypos    }, {1.0f, 0.0f} } ); // Bottom-right
				//vertices.push_back ( { {xpos,     ypos + h}, {0.0f, 1.0f} } ); // Top-left
				//vertices.push_back ( { {xpos + w, ypos + h}, {1.0f, 1.0f} } ); // Top-right

				//vertices.push_back ( { {-0.1f,     -0.1f   }, {0.0f, 1.0f} } ); // Bottom-left
				//vertices.push_back ( { {0.1f, -0.1f    }, {1.0f, 1.0f} } ); // Bottom-right
				//vertices.push_back ( { {0.1f,     0.1f}, {1.0f, 0.0f} } ); // Top-left
				//vertices.push_back ( { {-0.1f, 0.1f}, {0.0f, 0.0f} } ); // Top-right



				//vertices.push_back ( { {-0.1f,     -0.1f   }, {0.0f, 0.0f} } );
				//vertices.push_back ( { {0.1f,     -0.1f   }, {1.0f, 0.0f} } );
				//vertices.push_back ( { {0.1f,     0.1f   }, {1.0f, 1.0f} } );
				//vertices.push_back ( { {-0.1f,     0.1f   }, {0.0f, 1.0f} } );

				//indexOffset = 0;
				// Define los índices para el carácter
				indices.push_back ( indexOffset + 0 );
				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 2 );

				indices.push_back ( indexOffset + 1 );
				indices.push_back ( indexOffset + 3 );
				indices.push_back ( indexOffset + 2 );

				indexOffset += 4;

				x += (ch.advance) * scale; // advance.x is in 1/64th pixels
				//x += 0.20;


				i++;
			}

			prop = init ( vertices, indices, character );
			tx.push_back ( prop );
		}



		auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);
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


	std::shared_ptr<Entity3D> TextEntity::init ( std::vector<VertexText> vertices, std::vector<uint32_t> indices, Glyph texture ) {
		auto vulkanProp = std::make_shared<VulkanProperty> ();

		std::vector<VulkanUBuffer>  x = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );



		//vulkanProp->descriptorSets = device->createDescriptorSets ( x, texture.textureImageView, texture.textureSampler, sizeof ( UniformBufferObject2 ) );


		std::vector<DSInfo> bufDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, x, sizeof ( x ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		//std::vector<DSInfo> texDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, x, sizeof ( x ), texture.textureImageView, texture.textureSampler, 1 } );
		static int timer = 0;

		std::cout << " timer ........ " << timer++ << "\n\n";
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

	std::shared_ptr<Entity3D> TextEntity::init2 ( std::vector<VertexText> vertices, std::vector<uint32_t> indices ) {
		return std::shared_ptr<Entity3D> ();
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





		return pipeline1;
	}


	void TextEntity::initGlyph ( FT_Face face, char c, Glyph& character ) {
		if (FT_Load_Char ( face, c, FT_LOAD_RENDER )) {
			std::cerr << "No se pudo cargar el glifo" << std::endl;
			return;
		}
		//std::vector<float> sdf = loadGlyphAndGenerateSDF ( face, c );
		int texWidth = face->glyph->bitmap.width;
		int texHeight = face->glyph->bitmap.rows;

		character.width = face->glyph->bitmap.width;
		character.height = face->glyph->bitmap.rows;
		character.bearingX = face->glyph->bitmap_left;
		character.bearingY = face->glyph->bitmap_top;
		character.advance = face->glyph->advance.x;
		character.bitmap = face->glyph->bitmap.buffer;


	}

	void TextEntity::createTextureImage ( Glyph& character ) {

		int texWidth = character.width;
		int texHeight = character.height;




		VkDeviceSize imageSize = texWidth * texHeight;
		//VkDeviceSize imageSize = texWidth * texHeight * sizeof ( float );

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		device->createBuffer ( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );



		void* data;
		vkMapMemory ( device->device, stagingBufferMemory, 0, imageSize, 0, &data );
		memcpy ( data, character.bitmap, static_cast<size_t>(imageSize) );
		vkUnmapMemory ( device->device, stagingBufferMemory );


		device->createImage ( texWidth, texHeight, VK_FORMAT_R32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory );

		device->transitionImageLayout ( character.textureImage, VK_FORMAT_R32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
		device->copyBufferToImage ( stagingBuffer, character.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight) );
		device->transitionImageLayout ( character.textureImage, VK_FORMAT_R32_SFLOAT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );



		vkDestroyBuffer ( device->device, stagingBuffer, nullptr );
		vkFreeMemory ( device->device, stagingBufferMemory, nullptr );


	}



}