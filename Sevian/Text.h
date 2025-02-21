#pragma once
#include "TextEngine.h"

namespace SEVIAN {
	namespace TEXT {


		enum class TokenType
		{
			WORD,
			SPACE,
			NEWLINE
		};

		struct CharInfo
		{
			char c;
			float advance = 0.0f;
			float scale = 1.0f;
			float lineHeight = 0.0f;
			AtlasGlyphInfo ch;
			
		};

		struct Token
		{
			
			TokenType type;
			Font font;
			
			std::string text;
			float scale = 1.0f;
			float lineHeight = 0.0f;
			float width;  // Se calculará en función de la fuente
			std::vector<Quad> quads;
		};

		struct TokenChar
		{
			
			Font font;
			char c;
			float scale;

		};

		struct Fragment
		{
			Font font;
			std::string text;
			float scale;

		};

		struct Word
		{
			std::vector<Quad> quads;
		};

		class Line
		{
			std::vector<Fragment> fragments;

			//float lineWidth = 0.0f;
			//float lineHeight = 0.0f;
			float scale = 0.0f;
			bool full = false;

		public:
			Line ( std::vector<Quad>&& quads, float y, float lineWidth, float lineHeight, float scale, bool full );
			Line () { };
			std::vector<Quad> quads = {};
			float y = 0.0f;
			float resetPositionY ( float lineHeight );
			void format ( TextAlignment alignment, float maxWidth );
			bool isCompleted ();
			float lineWidth = 0.0f;
			float lineHeight = 0.0f;
		};

		class Box
		{
			//std::vector<Line> lines;
			std::vector<Line> lines;
			float maxWidth;
			float maxHeight;
			float posX = 0.0f;
			float posY = 0.0f;
			Line currentLine;
			float lineHeight = 0.0f;
			float lastScale = 1.0f;

			std::vector<Fragment> fragments;

			void addLine ( float& posY, std::vector<CharInfo> words, bool full );
			void addLine2 ( float& y, std::vector<CharInfo> words, bool full );
		public:
			Box ( float width, float height ) :maxWidth ( width ), maxHeight ( height ) { }
			
			std::vector<Line> getLines ();
			std::vector<Quad> getQuads ();
			void format ( TextAlignment alignment );
			void addText ( const std::string& text, Font font, float scale );
			
			void render ();
			void render2 ();
		};
		class Layout
		{
			std::vector<Box> boxes;
		};
	}


};



