#include "FontManager.h"
#include <stdexcept>


namespace SEVIAN::FONT {
	std::map<std::string, Font> Manager::fonts;
	void Manager::createFont ( std::string name, std::string path ) {

		AtlasInfo info;

		info.fontPath = path;
		info.size = 32;
		info.cols = 10;
		info.rows = 10;
		info.firstGlyph = 32;
		info.lastGlyph = 128;

		info.border = 0.0;
		info.range = 0.050;
		info.centered = true;
		info.edgeColoring = true;
		info.angleThreshold = 3.0;


		AtlasGenerator atlas;
		auto font = atlas.create ( name, info );
		fonts.emplace ( name, font );
		//fonts[name] = font;
	}
	Font& Manager::getFont ( const std::string& name ) {
		// TODO: Insertar una instrucción "return" aquí
		//return fonts[name];
		auto it = fonts.find ( name );
		if (it != fonts.end ()) {
			return it->second; // La fuente fue encontrada
		}
		else {
			// Manejar el caso en el que la fuente no existe
			throw std::runtime_error ( "Font not found" );
		}
	}
}



