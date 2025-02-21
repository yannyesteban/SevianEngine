#include "VKElement.h"

namespace SEVIAN::VULKAN {
	void VKElement::addChild ( std::shared_ptr<IRenderizable> child ) {
		childs.emplace_back ( child );
	}
	void VKElement::updateTransform ( const RENDERER::Transform& parentTransform ) {
	}
}