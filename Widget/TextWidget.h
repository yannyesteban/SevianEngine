#pragma once
#include "Widget.h"
#include "Linear.h"
namespace SEVIAN::WIDGET {
	class TextWidget :
		public Widget
	{
	private:
		RENDERER::iTextManager* textRenderer;
		TextInfo info;
		//std::string text = "";
		SEVIAN::TEXT::Box box { 400,400 };
		//std::shared_ptr<RENDERER::IRenderizable> object;
		int withTexture = 1;
		glm::vec4 color = { 1.0f, 0.6f, 0.1f, 1.0f };
		StyleUbo styleUbo;
	public:
		TextWidget ( std::shared_ptr<RENDERER::RenderInterface> render, std::string text );
		//void update ( float deltaTime ) override;
		//std::shared_ptr<RENDERER::IRenderizable> getRenderObject ( Camera2D camera ) override;

		glm::mat4 getModelMatrix () override;

	};
}