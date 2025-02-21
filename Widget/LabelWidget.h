#pragma once
#include "Widget.h"
#include <Text.h>
#include <TextManager.h>

namespace SEVIAN::WIDGET {
	class Label :
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
		Label ( std::shared_ptr<RENDERER::RenderInterface> render, TextInfo info );
		void render () override;
		void update ( float deltaTime ) override;
		std::shared_ptr<RENDERER::IRenderizable> getRenderObject () override;
	};
}


