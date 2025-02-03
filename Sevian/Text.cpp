#include "Text.h"
#include <iostream>
#include <sstream>

namespace SEVIAN {
	namespace TEXT {
		void Box::addWord ( std::vector<Quad>, float wordWidth ) {
		
		
		}

		void Box::addText2 ( const std::string& text, Font font, float scale ) {
			fragments.push_back ( { font, text, scale } );

		}
		/*
		void Box::render2 () {
			Font font;
			float scale = 0.0f;
			float lineHeight = 0.0f;

			std::vector<Quad> quads;
			std::vector<Quad> lineQuads;

			float x = 0.0f;
			float y = 0.0f;

			for (Fragment& fragment : fragments) {
				font = fragment.font;
				scale = fragment.scale;
				lineHeight = font.lineHeight * scale;

				for (size_t i = 0; i < fragment.text.size (); ++i) {
					char c = fragment.text[i];
					AtlasGlyphInfo ch = font.characters[c];

					if (x > maxWidth || c == '\n') {
						// Si el carácter es un espacio o un salto de línea, procesar la línea actual
						if (!lineQuads.empty ()) {
							y -= lineHeight;
							float lineWidth = 0.0f;

							for (auto& q : lineQuads) {
								q.yPos = y - q.offsetY;
								lineWidth += q.ch.advance * scale;
							}

							lines.emplace_back ( std::move ( lineQuads ), y, lineWidth, lineHeight, scale, true );
							lineQuads.clear ();
						}

						// Reiniciar posición x
						x = 0.0f;

						// Si el carácter era un salto de línea, continuar con el siguiente
						if (c == '\n') continue;
					}

					if (std::isspace ( c )) {
						// Si es un espacio, agregar las palabras acumuladas a la línea
						if (!quads.empty ()) {
							lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );
							quads.clear ();
						}
						x += ch.advance * scale;
						continue;
					}

					// Crear un Quad para el carácter actual
					Quad q;
					q.c = c;
					q.ch = ch;
					q.xPos = x + ch.bearingX * scale;
					q.yPos = y - (ch.height - ch.bearingY) * scale;
					q.width = ch.width * scale;
					q.height = ch.height * scale;

					x += ch.advance * scale;
					quads.emplace_back ( q );
				}
			}

			// Agregar la última línea si hay caracteres pendientes
			if (!quads.empty ()) {
				lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );
				y -= lineHeight;
				float lineWidth = 0.0f;

				for (auto& q : lineQuads) {
					q.yPos = y - q.offsetY;
					lineWidth += q.ch.advance * scale;
				}

				lines.emplace_back ( std::move ( lineQuads ), y, lineWidth, lineHeight, scale, false );
			}
		}
		*/
		
		void Box::render2 () {

			Font font;
			float scale = 0.0f;
			float lineHeight = 0.0f;

			std::vector<Quad> quads;
			std::vector<Quad> lineQuads;

			
			float x = 0.0f;
			float y = 0.0f;
			int n = 0;
			float wordWidth = 0.0f;
			for (Fragment& fragment : fragments) {

				font = fragment.font;
				scale = fragment.scale;
				lineHeight = font.lineHeight * scale;

				for (auto& c: fragment.text) {

					//AtlasGlyphInfo ch = font.characters[c];
					
					if (x > maxWidth || c == '\n') {
						
						bool isNewLine = std::isspace ( c ) || c == '\n';
						
						if (isNewLine && wordWidth > maxWidth) {
							lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );
						}


						if (isNewLine || wordWidth < maxWidth) {
							//new line
							y -= lineHeight;
							float w = 0.0f;
							float lineWidth = 0.0f;

							for (auto& q : lineQuads) {

								q.yPos = y - (q.ch.height - q.ch.bearingY) * scale;
								q.xPos = w + q.ch.bearingX * scale;
								lineWidth += q.ch.advance * scale;
								w += q.ch.advance * scale;
							}

							lines.emplace_back ( std::move ( lineQuads ), y, lineWidth, lineHeight, 1.0f, true );
							lineQuads.clear ();

							if (isNewLine) {
								quads.clear ();
								wordWidth = 0.0f;
								x = 0.0f;
							}
							else {
								if (!quads.empty ()) {
									if (quads[0].c == ' ') {
										quads.erase ( quads.begin () );
									}
								}
								
								x = wordWidth;
							}
						}
						
						if (c == '\n' || std::isspace ( c )) {
							continue;
						}
					}
					else {

						if (std::isspace ( c )) {
							
							lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );
							quads.clear ();
							wordWidth = 0.0f;
						}
						// add word to line
						//lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );
						//quads.clear ();
					}
					Quad q;
					AtlasGlyphInfo ch = font.characters[c];
					q.c = c;
					q.ch = ch;

					float xpos = x + ch.bearingX * scale;
					//float ypos = y - (ch.height - ch.bearingY) * scale;

					q.xPos = xpos;
					q.yPos = 0.0f;
					q.width = ch.width * scale;
					q.height = ch.height * scale;

					x += ch.advance * scale;
					wordWidth += ch.advance * scale;
					quads.emplace_back ( q );
					n++;
					
				}


				
			}
			if (!quads.empty ()) {
				lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );

			}


			if (!lineQuads.empty ()) {
				//lineQuads.insert ( lineQuads.end (), quads.begin (), quads.end () );
				y -= lineHeight;
				float w = 0.0f;
				float lineWidth = 0.0f;
				for (auto& q : lineQuads) {

					q.yPos = y - (q.ch.height - q.ch.bearingY) * scale;
					q.xPos = w + q.ch.bearingX * scale;
					lineWidth += q.ch.advance * scale;
					w += q.ch.advance * scale;
				}

				lines.emplace_back ( std::move ( lineQuads ), y, w, lineHeight, scale, false );
			}
		}
		
		void Box::render () {
			std::vector<Token> tokens;
			Font font;
			std::string buffer;
			float w = 0.0f;
			float scale = 1.0f;
			std::vector<Quad> quads ;
			float lineHeight = 0.0f;
		


			for (auto& fragment : fragments) {


				for (size_t i = 0; i < fragment.text.size (); ++i) {
					lineHeight = std::max ( lineHeight, (float) fragment.font.lineHeight );
					font = fragment.font;
					scale = fragment.scale;
					char c = fragment.text[i];
					AtlasGlyphInfo ch = fragment.font.characters[c];
					if (c == '\n') {
						if (!buffer.empty ()) {
							tokens.push_back ( { TokenType::WORD, fragment.font, buffer, fragment.scale, lineHeight * fragment.scale, w, quads } );
							buffer.clear ();
							quads.clear ();
							w = 0.0f;
						}
						tokens.push_back ( { TokenType::NEWLINE, fragment.font, "\\n", 0.0f, {} } );
						lineHeight = 0.0f;
					}
					else if (c == ' ') {
						if (!buffer.empty ()) {
							tokens.push_back ( { TokenType::WORD, fragment.font, buffer, fragment.scale, (float) fragment.font.lineHeight * fragment.scale,w, quads } );
							buffer.clear ();
							quads.clear ();
							w = 0.0f;
						}

						std::string spaces;
						while (i < fragment.text.size () && fragment.text[i] == ' ') {



							spaces += ' ';
							i++;
							w += ch.advance * fragment.scale;
						}
						i--; // Ajustar el índice porque el `while` lo adelantó una vez extra
						tokens.push_back ( { TokenType::SPACE, fragment.font, spaces, fragment.scale, (float) fragment.font.lineHeight * fragment.scale, w } );
					}
					else {
						w += ch.advance * fragment.scale;
						buffer += c;
						Quad q;
						
						q.c = c;
						q.ch = ch;

						q.offsetX =  ch.bearingX * fragment.scale;
						q.offsetY =  (ch.height - ch.bearingY) * fragment.scale;
						

						q.xPos = 0.0;
						q.yPos = 0.0;
						q.width = ch.width * fragment.scale;
						q.height = ch.height * fragment.scale;
						q.scale = fragment.scale;
						q.lineHeight = font.lineHeight * fragment.scale;
						quads.emplace_back ( q );
						
						
					}

				}
			}
			if (!buffer.empty ()) {
				tokens.push_back ( { TokenType::WORD, font, buffer, scale, (float) font.lineHeight * scale, w, quads } );
			}

			// Variables para el renderizado
			float x = posX;
			float y = posY;
			float lineWidth = 0.0f;
			//float lineHeight = 0.0f;
			std::vector<Quad> currentLineQuads;

			// Procesar los tokens para calcular las posiciones de los quads
			for (auto& token : tokens) {
				//lineHeight = std::max ( lineHeight, token.lineHeight );
				if (token.type == TokenType::NEWLINE) {
					// Finalizar la línea actual
					if (!currentLineQuads.empty ()) {
						y -= lineHeight;
						for (auto& q : currentLineQuads) {
							
							q.yPos = y - q.offsetY;
						}

						lines.emplace_back ( std::move ( currentLineQuads ), y, lineWidth, lineHeight, 1.0f, true );
						currentLineQuads.clear ();
					}

					// Cambiar a la siguiente línea
					//y -= lineHeight; // Mover hacia abajo
					x = posX;
					lineWidth = 0.0f;
					lineHeight = 0.0f;
				}
				else if (token.type == TokenType::SPACE) {
					// Agregar espacio a la línea actual
					float spaceWidth = 0.0f;

					for (char c : token.text) {
						//AtlasGlyphInfo ch = c.; // Usar la primera fuente como referencia
						spaceWidth += token.width;

					}
					for (char c : token.text) {
						//AtlasGlyphInfo ch = c.; // Usar la primera fuente como referencia
						//spaceWidth += ch.advance * token.scale;
					}

					if (x + spaceWidth > maxWidth) {
						// Cambiar de línea si el espacio no cabe
						y -= lineHeight;
						for (auto& q : currentLineQuads) {

							q.yPos = y - q.offsetY;
						}

						lines.emplace_back ( std::move ( currentLineQuads ), y, lineWidth, lineHeight, 1.0f, true );
						currentLineQuads.clear ();
						//y -= lineHeight;
						x = posX;
						lineWidth = 0.0f;
						lineHeight = 0.0f;
					}

					x += spaceWidth;
					lineWidth += spaceWidth;
				}
				else if (token.type == TokenType::WORD) {
					// Agregar palabra a la línea actual
					/*
					if (!lines.empty ()) {
						auto lastLine = lines.back ();
						if (lineHeight > lastLine.lineHeight) {
							y += lastLine.lineHeight - token.lineHeight;
							//lineHeight = token.lineHeight;

							for (auto& q : currentLineQuads) {
								float ypos = y - (q.ch.height - q.ch.bearingY) * token.scale;
								q.yPos = ypos;
							}
						}

					}
					*/
					if (x + token.width > maxWidth) {
						// Cambiar de línea si la palabra no cabe
						y -= lineHeight;
						for (auto& q : currentLineQuads) {

							q.yPos = y - q.offsetY;
						}

						lines.emplace_back ( std::move ( currentLineQuads ), y, lineWidth, lineHeight, 1.0f, true );
						currentLineQuads.clear ();
						//y -= lineHeight;
						x = posX;
						lineWidth = 0.0f;
						lineHeight = 0.0f;
					}

					// Calcular las posiciones de los quads de la palabra
					for (auto& quad : token.quads) {

						if (quad.c == 'w') {
							std::cout << "";
						}

						quad.xPos = x + quad.offsetX;
						//quad.yPos = y -  quad.offsetY;
						currentLineQuads.push_back ( quad );

						x += quad.ch.advance * quad.scale;
						lineWidth += quad.ch.advance * quad.scale;
						lineHeight = std::max ( lineHeight, quad.lineHeight  );
					}
				}
			}

			// Finalizar la última línea si no está vacía
			if (!currentLineQuads.empty ()) {
				y -= lineHeight;
				for (auto& q : currentLineQuads) {

					q.yPos = y - q.offsetY;
				}
				lines.emplace_back ( std::move ( currentLineQuads ), y, lineWidth, lineHeight, 1.0f, false );
			}

			std::cout << "Render quad: ";

		}

		void Box::addText ( const std::string& text, Font font, float scale ) {

			

			float x = posX;
			float y = posY;

			currentLine;


			std::vector<Quad> quads {};
			
			float currentLineHeight = font.lineHeight * scale;

			if (!lines.empty ()) {

				auto& lastLine = lines.back ();

				if (!lastLine.isCompleted ()) {
					if (currentLineHeight > lineHeight) {
						y = lastLine.resetPositionY ( currentLineHeight );
					}
					quads = std::move ( lastLine.quads );

					lines.pop_back ();
				}
			}

			lineHeight = std::max ( lineHeight, currentLineHeight );

			std::vector<Quad> word;
			std::vector<std::vector<Quad>> words;

			float w = 0.0f;
			for (char c : text) {

				if (std::isspace ( c )) {
					words.push_back ( word );
					addWord ( word, x );
					word.clear ();

				}


				Quad q;
				AtlasGlyphInfo ch = font.characters[c];
				q.c = c;
				q.ch = ch;

				float xpos = x + ch.bearingX * scale;
				float ypos = y - (ch.height - ch.bearingY) * scale;

				q.xPos = xpos;
				q.yPos = ypos;
				q.width = ch.width * scale;
				q.height = ch.height * scale;

				x += ch.advance * scale;
				w += ch.advance * scale;
				word.emplace_back ( q );
				




				continue;
				if (c == '\n' || std::isspace ( c ) && x > maxWidth) {

					lines.emplace_back ( std::move ( quads ), y, x, lineHeight, scale, true );
					lineHeight = currentLineHeight;
					x = 0.0f;
					y -= lineHeight;
					quads.clear ();
					continue;
				}
				{
					Quad q;
					AtlasGlyphInfo ch = font.characters[c];
					q.c = c;
					q.ch = ch;

					float xpos = x + ch.bearingX * scale;
					float ypos = y - (ch.height - ch.bearingY) * scale;

					q.xPos = xpos;
					q.yPos = ypos;
					q.width = ch.width * scale;
					q.height = ch.height * scale;

					x += ch.advance * scale;

					quads.emplace_back ( q );
				}
				
			}

			if (!quads.empty ()) {

				lines.emplace_back ( std::move ( quads ), y, x, currentLineHeight, scale, false );
			}

			posX = x;
			posY = y;
			lastScale = scale;
		}

		/*void Box::addText ( const std::string& text, Font font, float scale ) {

			std::string text1 = "  yanny esteban nunez\njimenez";
			std::istringstream stream ( text1 );
			std::string word;

			while (stream >> word) {
				std::cout << "palabra: " << word << "\n";
			}


			float x = posX;
			float y = posY;

			std::vector<Quad> quads {};
			
			float currentLineHeight = font.lineHeight * scale;

			if (!lines.empty ()) {

				auto& lastLine = lines.back ();

				if (!lastLine.isCompleted ()) {
					if (currentLineHeight > lineHeight) {
						y = lastLine.resetPositionY ( currentLineHeight );
					}
					quads = std::move ( lastLine.quads );

					lines.pop_back ();
				}
			}

			lineHeight = std::max ( lineHeight, currentLineHeight );

			for (char c : text) {
				if (c == '\n' || std::isspace ( c ) && x > maxWidth) {

					lines.emplace_back ( std::move ( quads ), y, x, lineHeight, scale, true );
					lineHeight = currentLineHeight;
					x = 0.0f;
					y -= lineHeight;
					quads.clear ();
					continue;
				}

				Quad q;
				AtlasGlyphInfo ch = font.characters[c];
				q.c = c;
				q.ch = ch;

				float xpos = x + ch.bearingX * scale;
				float ypos = y - (ch.height - ch.bearingY) * scale;

				q.xPos = xpos;
				q.yPos = ypos;
				q.width = ch.width * scale;
				q.height = ch.height * scale;

				x += ch.advance * scale;

				quads.emplace_back ( q );
			}

			if (!quads.empty ()) {

				lines.emplace_back ( std::move ( quads ), y, x, currentLineHeight, scale, false );
			}

			posX = x;
			posY = y;
			lastScale = scale;
		}*/

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
			: quads ( std::move ( quads ) ), y ( y ), lineWidth ( lineWidth ), lineHeight ( lineHeight ), scale ( scale ), full ( full ) {

			std::cout << "ancho de linea: " << lineWidth << "\n";
		}

		void Line::format ( TextAlignment alignment, float maxWidth ) {
			if (quads.empty ()) return;

			float offsetX = 0.0f;
			float totalExtraSpace = maxWidth - lineWidth;
			int spaceCount = 0;
			switch (alignment) {
			case TextAlignment::CENTER:
				offsetX = (maxWidth - lineWidth) / 2.0f;
				break;
			case TextAlignment::RIGHT:
				offsetX = maxWidth - lineWidth;
				break;
			case TextAlignment::JUSTIFY:
				// Calcular el espacio total que se necesita agregar
				

				if (!full) {
					// No justificar la última línea
					break;
				}
				
				if (lineWidth > maxWidth) {
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

					for (size_t i = 0; i < quads.size (); ++i) {
						quads[i].xPos += accumulatedOffset;

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

			if (alignment != TextAlignment::JUSTIFY) {
				for (auto& quad : quads) {
					quad.xPos += offsetX;
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