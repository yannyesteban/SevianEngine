#pragma once
#include <vector>
#include "Widget.h"

class ZLayout
{
private:
	std::vector<SEVIAN::WIDGET::Widget*> widgets;
public:
	void addWidget ( SEVIAN::WIDGET::Widget* w ) {

	}
};


/*

		class Button
		{
			glm::vec2 position;
			glm::vec2 size;
			float cornerRadius;
			std::string text;
			Font font;

			void draw () {
				Rectangle r = Rectangle ( 0.0, 0.0, 2.0, 2.0 );

				TEXT::Box box { 800, 500 };
				box.addText ( "Aceptar", font, 36 );
				box.render ();
				box.getQuads ();




			}
		};

		namespace SEVIAN {

	namespace WIDGET {

		Rectangle::Rectangle ( float x , float y, float width, float height ) {

		}

		void Rectangle::draw () {

		}
	}

}

		*/