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
	float w = 0.0f;
	float h = 0.0f;
	int line = 0;
	char c;
	float xPos = 0.0f;
	float yPos = 0.0f;
};


struct Font
{
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
	std::string fontPath;
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
	Font create ( AtlasInfo info );
};
