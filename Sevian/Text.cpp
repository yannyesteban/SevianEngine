#include "Text.h"





void SEVIAN::TEXT::Box::addText ( const std::string& text, Font font, float scale ) {


	float width = 500.0f;

	float lineWidth = 0.0f;

	int line = curretLine;
	float x = posX;
	float y = posY;

	float w = 0.0f;
	std::vector<Quad> quads {};

	

	int size = 100;



	//double lineHeight = lineHeight;

	if (!lines.empty () && font.lineHeight * scale > lineHeight) {
		y += lineHeight;
		// Ajustar la �ltima l�nea al nuevo `lineHeight`
		lineHeight = font.lineHeight * scale;
		y -= lineHeight;
		auto& lastLine = lines.back (); // �ltima l�nea

		// Recalcular la posici�n vertical para cada car�cter en la �ltima l�nea
		for (Quad& q : lastLine) {
			float ypos = y - (q.ch.height - q.ch.bearingY) * lastScale;
			q.yPos = ypos;
		}

		// Reajustar la posici�n `y` para empezar correctamente la nueva l�nea
		//y -= lineHeight ;
	}
	
	if (font.lineHeight > lineHeight) {
		lineHeight = font.lineHeight * scale;
	}

	for (char c : text) {

		if (c == '\n') { // Si es un car�cter de nueva l�nea
			// Almacenar la l�nea actual en la lista de l�neas
			lines.push_back ( quads );
			quads.clear (); // Limpiar los quads para la nueva l�nea

			// Reiniciar la posici�n horizontal y mover la posici�n vertical hacia abajo
			x = 0.0f;
			y -= lineHeight ; // Mover hacia abajo (ajusta seg�n la altura de la l�nea)
			line++; // Incrementar el n�mero de l�nea
			continue; // Saltar al siguiente car�cter
		}

		Quad q;
		AtlasGlyphInfo ch = font.characters[c];
		q.c = c;
		q.ch = ch;
		q.line = line;
		float xpos = x + ch.bearingX * scale;
		float ypos = y - (ch.height - ch.bearingY) * scale;

		q.xPos = xpos;
		q.yPos = ypos;
		q.w = ch.width * scale;
		q.h = ch.height * scale;


		x += ch.advance * scale;
		w += x;

		if (std::isspace ( c ) && x > width) {
			lines.push_back ( quads );
			quads.clear ();
			y -= lineHeight ;
			x = 0.0;
			line++;



		}

		quads.push_back ( q );

	}

	if (!quads.empty ()) {
		lines.push_back ( quads );
	}

	// Actualizar posY para el siguiente texto
	posX = x;
	posY = y; // Mover posY hacia abajo
	lastScale = scale;

}

std::vector<std::vector<Quad>> SEVIAN::TEXT::Box::getLines () {

	return lines;
	
}
