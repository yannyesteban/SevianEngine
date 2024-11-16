#include "ShadowWorld.h"

VULKAN::ShadowWorld::ShadowWorld ( Device* device, uint32_t width, uint32_t height ):
	device(device), width(width), height(height) {


	
	VkFormat depthFormat = device->findShadowMapFormat (); //VK_FORMAT_D16_UNORM;// VK_FORMAT_D32_SFLOAT;// VK_FORMAT_D16_UNORM;// 
	

	device->createImage ( width, height, depthFormat, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory );
	imageView = device->createImageView ( image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT );


	


	VkSamplerCreateInfo samplerInfo {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	//samplerInfo.anisotropyEnable = VK_FALSE;

	samplerInfo.anisotropyEnable = VK_TRUE;

	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_LESS;// VK_COMPARE_OP_ALWAYS;// VK_COMPARE_OP_LESS;  // Importante para shadow mapping
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;// VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 1.0f;

	vkCreateSampler ( device->device, &samplerInfo, nullptr, &device->shadowMapSampler );

	renderPass = device->createShadowRenderPass ();
	frameBuffer = createShadowFramebuffer ( renderPass, imageView, width, height );
	device->shadowFrameBuffer = frameBuffer;
	device->shadowRenderPass = renderPass;
	VulkanDepthResources depthResource {};

	device->depthResource.imageView = imageView;
}


VkFramebuffer VULKAN::ShadowWorld::createShadowFramebuffer ( VkRenderPass shadowRenderPass, VkImageView depthImageView, uint32_t width, uint32_t height ) {

	// Configuración de la información del framebuffer
	VkFramebufferCreateInfo framebufferInfo {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = shadowRenderPass;  // Render pass específico para sombras
	framebufferInfo.attachmentCount = 1;  // Solo 1 attachment (depth)
	framebufferInfo.pAttachments = &depthImageView;  // El attachment es el depth image view

	framebufferInfo.width = width;   // Tamaño del framebuffer para sombras
	framebufferInfo.height = height; // Tamaño del framebuffer para sombras
	framebufferInfo.layers = 1;  // Solo una capa, a menos que estés trabajando con un shadow map en 3D

	VkFramebuffer shadowFramebuffer;
	if (vkCreateFramebuffer ( device->device, &framebufferInfo, nullptr, &shadowFramebuffer ) != VK_SUCCESS) {
		throw std::runtime_error ( "failed to create shadow framebuffer!" );
	}

	return shadowFramebuffer;
}