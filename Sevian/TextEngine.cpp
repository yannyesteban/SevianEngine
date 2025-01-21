#include "TextEngine.h"
#include <opencv2/opencv.hpp>
#include <msdfgen.h>
#include <msdfgen-ext.h>

#include <iostream>
using namespace msdfgen;

void printTest2 ( Bitmap<float, 3> msdf, char c ) {
	if (c == '@' || c == 'A' || c == 'g' || c == 'e' || c == 'y' || c == 'o') {
		std::string name = "test_";
		name += c;
		name += ".png";
		//printf ( "altura: %c es : %f", c, height );
		savePng ( msdf, name.c_str () );
	}
}

// Función para guardar el atlas SDF como imagen
void saveSDFAtlas2 ( const std::vector<uint8_t>& atlasData, int atlasWidth, int atlasHeight, const std::string& filename, const std::string& filename2 ) {
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




Font AtlasGenerator::create ( AtlasInfo info ) {
	// Inicializar msdfgen y cargar la fuente TTF
	msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype ();
	if (!ft) {
		std::cerr << "No se pudo inicializar FreeType con msdfgen" << std::endl;
		return Font {};
	}

	msdfgen::FontHandle* font = msdfgen::loadFont ( ft, info.fontPath.c_str () );
	if (!font) {
		std::cerr << "No se pudo cargar la fuente: " << info.fontPath << std::endl;
		msdfgen::deinitializeFreetype ( ft );
		return Font {};
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
	
	std::map<char, AtlasGlyphInfo> characters;
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
			return Font {};
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
		characters[c] = ch;

		Vector2 scale ( info.size, info.size );
		Vector2 translation ( left, bottom );
		msdfgen::SDFTransformation tr = msdfgen::SDFTransformation ( msdfgen::Projection ( scale, translation ), msdfgen::Range ( info.range ) );

		generateMSDF ( msdf, shape, tr );

		printTest2 ( msdf, c );

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

	saveSDFAtlas2 ( msdfData, atlasWidth, atlasWidth, "atlas_2025.png", "atlas_2025c.png" );

	Font font1 ( { characters, { msdfData, atlasWidth, atlasHeight } } );
	
	return font1;

}
