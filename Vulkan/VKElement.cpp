#include "VKElement.h"

namespace SEVIAN::VULKAN {
	void VKElement::addChild ( std::shared_ptr<IRenderizable> child ) {
		childs.emplace_back ( child );
	}
	void VKElement::updateTransform ( const RENDERER::Transform& parentTransform ) {
	}
	void VKElement::render ( VkCommandBuffer commandBuffer, uint32_t swapchainImageIndex, Camera2D camera ) {
	}
	void VKElement::addData ( RENDERER::DataResource id, void* info ) {
		data[id] = info;
	}
	std::unordered_map<RENDERER::DataResource, void*> VKElement::getData () {

		return data;
	}
	void* VKElement::getData ( RENDERER::DataResource id ) {
		return data[id];
	}
}