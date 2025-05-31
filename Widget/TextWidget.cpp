#include "TextWidget.h"
#include "Linear.h"
namespace SEVIAN::WIDGET {
	TextWidget::TextWidget ( std::shared_ptr<RENDERER::RenderInterface> render, std::string text ) : textRenderer ( render->getManager<RENDERER::iTextManager> () ) {
		
		std::string fontFamily = "arial";

		auto font = textRenderer->getFont ( fontFamily );
	

		box.addText ( text, font, 50.0f );
	


		box.render ();
		object = textRenderer->createText ( position.x, position.y, box.getQuads (), "arial" );

		setLayout ( std::make_unique<Block> ( render->getViewport () ) );


		auto layout = addStyle<WIDGET::LayoutComponent> ();
		layout->display = Display::Inline;
	
	}
	glm::mat4 TextWidget::getModelMatrix ()
	{

		layout->arrange ( this );
		auto translation = glm::mat4 ( 1.0f );
		glm::mat4 rotationMatrix = glm::mat4 ( 1.0f );
		size.x = 1.0f;
		size.y = 1.0f;


		auto scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );

		translation = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( position.x, position.y, 1.0f ) );
		scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );

		auto modelMatrix = translation * rotationMatrix * scaleMatrix;
		return modelMatrix;
	}
}


