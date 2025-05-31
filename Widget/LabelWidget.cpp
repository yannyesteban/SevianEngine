#include "LabelWidget.h"



namespace SEVIAN::WIDGET {
	LabelWidget::LabelWidget ( std::shared_ptr<RENDERER::RenderInterface> render, TextInfo _info ) :
		renderer ( render->getManager<RENDERER::iTextManager> () ), box ( { _info.width, _info.height } ) {
	
		position.x = info.x;
		position.y = info.y;
		size.x = 1.0f;
		size.y = 1.0f;


		auto font1 = renderer->getFont( _info.fontName );
		box.addText ( _info.text, font1, _info.scale );


		
		box.render ();
		//box.format ( CENTER );
		object = renderer->createText ( _info.x, _info.y, box.getQuads (), _info.fontName );

	}

	

	void LabelWidget::update ( float deltaTime ) {
	}
	
	
	
}

