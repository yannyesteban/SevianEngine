#pragma once
#include <string>
#include <map>
#include "TextEngine.h"

namespace SEVIAN::FONT {
	class Manager
	{
	private:
		static std::map<std::string, Font> fonts;
	public:
		static void createFont ( std::string name, std::string path );
		static Font& getFont ( const std::string& name );
	};
	/*
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
		auto font = atlas.create ( info );
		fonts[name] = font;
	}
	*/
	/*
	Font& Manager::getFont ( const std::string& name ) {
		return fonts[name];
	}
	*/
}