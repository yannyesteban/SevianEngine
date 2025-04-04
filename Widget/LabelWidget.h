#pragma once
#include "Widget.h"
#include <Text.h>
#include <TextManager.h>

namespace SEVIAN::WIDGET {
	class TextWidget :
		public Widget
	{

	private:
		RENDERER::iTextManager * renderer;
		TextInfo info;
		//std::string text = "";
		SEVIAN::TEXT::Box box { 400,400 };
		std::shared_ptr<RENDERER::IRenderizable> object;
	protected:
	public:
		
		std::vector<std::unique_ptr<Widget>> widgets;


		TextWidget ( std::shared_ptr<RENDERER::RenderInterface> render, TextInfo info );
		
		void update ( float deltaTime ) override;
		//std::shared_ptr<RENDERER::IRenderizable> getRenderObject ( Camera2D camera ) override;

	};
}


