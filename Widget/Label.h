#pragma once

#include "Widget.h"
#include "Layout.h"

#include <Text.h>
#include <TextManager.h>

namespace SEVIAN::WIDGET {
	class Label :
		public Widget
	{
	private:
		RENDERER::iTextManager* textManager;
		RENDERER::iWidgetManager* widgetRenderer;
		TextInfo info;
		std::string text = "";
		SEVIAN::TEXT::Box box { 400,400 };
		//std::shared_ptr<RENDERER::IRenderizable> object;
		int withTexture = 1;
		glm::vec4 color = { 1.0f, 0.6f, 0.1f, 1.0f };
		StyleUbo styleUbo;
	public:
		Label ( std::shared_ptr<RENDERER::RenderInterface> render, std::string text ) ;
		//void update ( float deltaTime ) override;
		//std::shared_ptr<RENDERER::IRenderizable> getRenderObject ( Camera2D camera ) override;

		glm::mat4 getModelMatrix () override;
	};
}
