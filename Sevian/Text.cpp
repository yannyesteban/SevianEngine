#include "Text.h"
#include <iostream>
#include <sstream>

namespace SEVIAN {
	namespace TEXT {

		void Box::addText ( const std::string& text, Font font, float scale ) {

			fragments.push_back ( { font, text, scale } );
		}

		
		void Box::addLine ( float& y, std::vector<CharInfo> words, bool full ) {

			float lineHeight = 0.0f;
			float lineWidth = 0.0f;
			for (auto& info : words) {
				lineWidth += info.advance;
				lineHeight = std::max ( info.lineHeight, lineHeight );
			}

			y -= lineHeight;
			float x = 0.0f;

			std::vector <Quad> quads;
			for (auto& info : words) {
				AtlasGlyphInfo ch = info.ch;
				Quad q;
				q.c = info.c;
				q.u0 = ch.u0;
				q.u1 = ch.u1;
				q.v0 = ch.v0;
				q.v1 = ch.v1;
				q.ch = ch;

				q.width = ch.width * info.scale;
				q.height = ch.height * info.scale;
				//lineWidth += ch.advance * info.scale;
				q.offsetX = ch.bearingX * info.scale;
				q.offsetY = (ch.height - ch.bearingY) * info.scale;

				q.xPos = x + q.offsetX;
				q.yPos = y - q.offsetY;

				q.advance = ch.advance * info.scale;
				x += ch.advance * info.scale;
				quads.push_back ( q );
			}

			lines.emplace_back ( std::move ( quads ), y, lineWidth, lineHeight, 1.0f, full );
		}

		
		void Box::render () {

			Font font;

			lines.clear ();

			std::vector<CharInfo> word;
			std::vector<CharInfo> lineWords;

			float x = 0.0f;
			float y = 0.0f;
			float wordWidth = 0.0f;
			float maxLineHeight = 0.0f;
			float lineHeight = 0.0f;
			float scale = 0.0f;

			for (Fragment& fragment : fragments) {

				font = fragment.font;
				scale = fragment.scale;
				lineHeight = font.lineHeight * scale;

				for (auto& c : fragment.text) {

					if (lineHeight > maxLineHeight) {
						maxLineHeight = lineHeight;
					}

					if (x > maxWidth || c == '\n') {

						bool isNewLine = std::isspace ( c ) || c == '\n';

						if (isNewLine && wordWidth > maxWidth) {
							lineWords.insert ( lineWords.end (), word.begin (), word.end () );
						}

						if (isNewLine || wordWidth < maxWidth) {

							addLine ( y, lineWords, true );

							maxLineHeight = 0.0f;
							lineWords.clear ();

							if (isNewLine) {
								word.clear ();
								wordWidth = 0.0f;
								x = 0.0f;
							}
							else {
								if (!word.empty () && std::isspace ( word[0].c )) {
									word.erase ( word.begin () );
								}

								x = wordWidth;
							}
						}

						if (isNewLine) {
							continue;
						}
					}
					else if (std::isspace ( c )) {

						lineWords.insert ( lineWords.end (), word.begin (), word.end () );
						word.clear ();
						wordWidth = 0.0f;

					}

					AtlasGlyphInfo ch = font.characters[c];
					CharInfo q;

					//AtlasGlyphInfo ch = font.characters[c];
					q.c = c;
					q.ch = ch;
					q.scale = scale;;
					q.lineHeight = lineHeight;
					q.advance = ch.advance * scale;

					x += ch.advance * scale;
					wordWidth += ch.advance * scale;
					word.emplace_back ( q );
				}
			}
			if (!word.empty ()) {
				lineWords.insert ( lineWords.end (), word.begin (), word.end () );
			}

			if (!lineWords.empty ()) {
				addLine ( y, lineWords, false );
			}
		}

		
		std::vector<Line> Box::getLines () {
			return lines;
		}

		
		std::vector<Quad> Box::getQuads () {

			std::vector<Quad> quads;
			//quads.reserve ( lines.size () * 10 ); // Estimate size based on average quads per line

			for (const auto& line : lines) {

				quads.insert ( quads.end (), line.quads.begin (), line.quads.end () );
			}
			return quads;
		}

		
		void Box::format ( TextAlignment alignment ) {

			for (auto& line : lines) {
				line.format ( alignment, maxWidth );

			}
		}

		
		Line::Line ( std::vector<Quad>&& quads, float y, float lineWidth, float lineHeight, float scale, bool full )
			: quads ( std::move ( quads ) ), y ( y ), lineWidth ( lineWidth ), lineHeight ( lineHeight ), scale ( scale ), full ( full ) { 	}

		
		void Line::format ( TextAlignment alignment, float maxWidth ) {
			if (quads.empty ()) return;

			float offsetX = 0.0f;
			float totalExtraSpace = maxWidth - lineWidth;
			int spaceCount = 0;
			switch (alignment) {
			case TextAlignment::CENTER:
				offsetX = totalExtraSpace / 2.0f;
				break;
			case TextAlignment::RIGHT:
				offsetX = totalExtraSpace;
				break;
			case TextAlignment::JUSTIFY:
				// Calcular el espacio total que se necesita agregar

				if (!full || lineWidth > maxWidth) {
					break;
				}

				// Contar cuántos espacios hay en la línea
				for (size_t i = 0; i < quads.size () - 1; ++i) {

					if (std::isspace ( quads[i].c )) {
						spaceCount++;
					}
				}

				if (spaceCount > 0) {
					// Distribuir el espacio extra entre los espacios
					float spaceIncrement = totalExtraSpace / spaceCount;
					float accumulatedOffset = 0.0f;
					offsetX = 0.0f;

					for (size_t i = 0; i < quads.size (); ++i) {

						quads[i].xPos = offsetX + accumulatedOffset + quads[i].offsetX;
						offsetX += quads[i].advance;
						if (std::isspace ( quads[i].c ) && i < quads.size () - 1) {
							accumulatedOffset += spaceIncrement;
						}
					}
				}
				break;
			case TextAlignment::LEFT:
			default:
				break;
			}

			if (alignment != TextAlignment::JUSTIFY || !full) {

				for (auto& quad : quads) {

					quad.xPos = offsetX + quad.offsetX;
					offsetX += quad.advance;
				}
			}
		}

		
		bool Line::isCompleted () {
			return full;
		}

		
		float Line::resetPositionY ( float newLineHeight ) {

			y += lineHeight - newLineHeight;
			lineHeight = newLineHeight;

			for (auto& q : quads) {
				float ypos = y - (q.ch.height - q.ch.bearingY) * scale;
				q.yPos = ypos;
			}

			return y;
		}
	}
}