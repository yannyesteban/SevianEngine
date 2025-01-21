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

struct Font
{
	std::map<char, AtlasGlyphInfo> characters;
	

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
