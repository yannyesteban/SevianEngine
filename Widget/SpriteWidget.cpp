#include "SpriteWidget.h"

namespace SEVIAN::WIDGET {
	void SpriteWidget::render () {
	}
	std::shared_ptr<RENDERER::IRenderizable> SpriteWidget::getRenderObject () {
		return std::shared_ptr<RENDERER::IRenderizable> ();
	}
	void SpriteWidget::update ( float deltaTime ) {

	}
}