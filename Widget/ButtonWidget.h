#pragma once
#include "Widget.h"
#include <Text.h>
#include <TextManager.h>

namespace SEVIAN::WIDGET {
	class Button :
		public Widget
	{

	private:
		

		RENDERER::iSpriteManager* spriteManager;
		TextInfo info;
		//std::string text = "";
		SEVIAN::TEXT::Box box { 400,400 };
		std::shared_ptr<RENDERER::IRenderizable> object;
	protected:
	public:
		Button ( std::shared_ptr<RENDERER::RenderInterface> render, TextInfo info );
		
		void update ( float deltaTime ) override;
		std::shared_ptr<RENDERER::IRenderizable> getRenderObject () override;
	};
}

