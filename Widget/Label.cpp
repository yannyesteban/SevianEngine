#include "Label.h"


namespace SEVIAN::WIDGET {


	Label::Label ( std::shared_ptr<RENDERER::RenderInterface> render, std::string text ) :
		widgetRenderer ( render->getManager<RENDERER::iWidgetManager> () ), text(text) {

		SEVIAN::SpriteInfo spriteInfo { "escultura", {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.9f, 0.1f, 2.0f} };
		//object = widgetRenderer->createSprite ( spriteInfo );

		//auto l = std::make_unique<Linear> ();

		//setLayout ( std::make_unique<Block> ( render->getViewport () ) );


		textManager = render->getManager<RENDERER::iTextManager> ();

		auto* textStyle = this->getStyle<TextComponent> ();

		if (!textStyle) {
			textStyle = new TextComponent {};
		}


		//box.format ( CENTER );
		if (!object) {
			textStyle->fontFamily = "arial";

			auto font1 = textManager->getFont ( "arial" );
			box.addText ( text, font1, 50.0f );



			box.render ();
			object = textManager->createText ( position.x, position.y, box.getQuads (), "arial" );
		}
	}


	glm::mat4 Label::getModelMatrix () {

		
		return glm::mat4 ( 1.0f );

		//std::cout << "Stack::update: " << this->id << "\n";

		//layout->arrange ( this );

		//auto o = getRenderObject();


		glm::vec4 backGroundColor = { 1.0f, 0.3f, 0.0f, 1.0f };
		glm::vec4 borderColor = { 0.0f, 0.1f, 0.1f, 1.0f };
		float borderWith = 0.0f;
		float borderRadius = 0.0f;
		int useTexture = 0;

		auto* background = this->getStyle<BackgroundComponent> ();

		if (background) {
			//color = glm::vec4 ( background->backGroundColor, 1.0f );
			//object->addData ( RENDERER::DataResource::STYLE, (void *)&background->backGroundColor.toVec4() );
			backGroundColor = background->backGroundColor.toVec4 ();




		}
		auto* style = this->getStyle<LayoutComponent> ();

		auto marginLeft = 0.0f;
		auto marginTop = 0.0f;

		auto x = position.x;
		auto y = position.y;

		auto width = size.x;
		auto height = size.y;

		if (style) {
			borderColor = style->border.color.toVec4 ();
			borderWith = style->border.size.px ();
			borderRadius = style->border.radius.px ();

			if (style->boxSizing == BoxSizing::ContentBox) {

			}

		}


		if (id == "ref") {
			//
			std::cout << "";
		}

		styleUbo = {
			size,
			backGroundColor,
			borderColor,
			borderWith,
			borderRadius,
			0

		};

		object->addData ( RENDERER::DataResource::STYLE, &styleUbo );

		auto translation = glm::mat4 ( 1.0f );
		glm::mat4 rotationMatrix = glm::mat4 ( 1.0f );

		auto scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );

		//PositionComponent* position = this->getStyle<PositionComponent> ();
		translation = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( position.x, position.y, 1.0f ) );
		scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );




		auto modelMatrix = translation * rotationMatrix * scaleMatrix;
		return modelMatrix;
	}
}

