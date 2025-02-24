#include "SpriteWidget.h"

namespace SEVIAN::WIDGET {
	
	std::shared_ptr<RENDERER::IRenderizable> SpriteWidget::getRenderObject () {
		return std::shared_ptr<RENDERER::IRenderizable> ();
	}
	void SpriteWidget::update ( float deltaTime ) {

	}
}