#pragma once
#include <string>
#include <map>
#include <vector>
struct AtlasGlyphInfo
{
public:
	float width;
	float height;
	float bearingX;
	float bearingY;

	float u0;
	float v0;
	float u1;
	float v1;

	float advance;


};
struct QuadTexture
{
	float u0 = 0.0f;
	float v0 = 0.0f;
	float u1 = 0.0f;
	float v1 = 0.0f;
};
struct Quad
{
	AtlasGlyphInfo ch;
	float x = 0.0f;
	float y = 0.0f;
	float width = 0.0f;
	float height = 0.0f;
	
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float advance = 0.0f;
	
	float xPos = 0.0f;
	float yPos = 0.0f;
	float scale = 1.0f;
	float lineHeight = 0.0f;
	float u0 = 0.0f;
	float u1 = 0.0f;
	float v0 = 0.0f;
	float v1 = 0.0f;
	char c;
};


struct Font
{
	std::string name = "";
	std::map<char, AtlasGlyphInfo> characters;
	double lineHeight;

	struct Atlas
	{
		
		std::vector<uint8_t> data;
		int width = 0;
		int height = 0;
	} atlas;
};




struct AtlasInfo
{
	
	std::string fontPath = "";
	double size = 32;
	int cols = 10;
	int rows = 10;
	unsigned char firstGlyph = 32;
	unsigned char lastGlyph = 128;

	double border = 0.0;
	float range = 0.050;
	bool centered = true;
	bool edgeColoring = true;
	double angleThreshold = 3.0;
};

class AtlasGenerator
{
private:
	
public:
	Font create ( std::string name, AtlasInfo info );
};

enum TextAlignment
{
	LEFT,
	RIGHT,
	CENTER,
	JUSTIFY

};