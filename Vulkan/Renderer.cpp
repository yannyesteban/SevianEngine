#include "Renderer.h"
//#include "VulkanProperty.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>




using namespace ::SEVIAN;
namespace SEVIAN {
	namespace VULKAN {

		static void loadModel ( const std::string path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices ) {
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string warn, err;

			if (!tinyobj::LoadObj ( &attrib, &shapes, &materials, &warn, &err, path.c_str () )) {
				throw std::runtime_error ( warn + err );
			}

			std::unordered_map<Vertex, uint32_t> uniqueVertices {};

			for (const auto& shape : shapes) {
				for (const auto& index : shape.mesh.indices) {
					Vertex vertex {};

					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.texCoords = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
					};

					vertex.color = { 1.0f, 1.0f, 1.0f };

					if (uniqueVertices.count ( vertex ) == 0) {
						uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size ());
						vertices.push_back ( vertex );
					}

					indices.push_back ( uniqueVertices[vertex] );
				}
			}
		}

		void VulkanRenderer::initialize () {

			createInstance ();
			createSurface ();


			auto physical = PhysicalDevice::create1 ( instance, surface, VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU );
			device = physical.createDevice ();
			commandPool = physical.createCommandPool ();
			graphicsQueue = physical.graphicsQueue;
			presentQueue = physical.presentQueue;
			swapChain = physical.createSwapChain ( window );

			//physicalDevices = getPhysicalDevices ();




			//int selectedDevice = 0;

			//device = physicalDevices.at ( selectedDevice ).createDevice ();
			//commandPool = physicalDevices.at ( selectedDevice ).createCommandPool ();

			//graphicsQueue = physicalDevices.at ( selectedDevice ).graphicsQueue;
			//presentQueue = physicalDevices.at ( selectedDevice ).presentQueue;

			device->commandPool = commandPool;
			device->createCommandBuffers ( device->frames, commandPool );

			//swapChain = physicalDevices.at ( selectedDevice ).createSwapChain ( window );
			extent = swapChain.extent;
			renderPass = device->createRenderPass ( swapChain.imageFormat ); // OK


			depthResources = device->createDepthResources ( extent );
			// = device->createDepthResources ( extent );


			//swapChain.framebuffers = physicalDevices.at ( selectedDevice ).createFramebuffers ( swapChain, renderPass, { depthResources.imageView } );
			swapChain.framebuffers = physical.createFramebuffers ( swapChain, renderPass, { depthResources.imageView } );
			device->renderPass = renderPass;
			descriptorPool = device->createDescriptorPool ();


			shadowWorld = new ShadowWorld ( device, shadowExtent.width, shadowExtent.height );

			textureManager = std::make_shared<TextureManager> ( device );

			std::map<std::string, VulkanTexture> mTextures;

			frames.resize ( 2 );

			uint32_t index = 0;
			for (auto& frame : frames) {
				frame.index = index;
				device->createSyncObjects ( frame );
				index++;
			}

			device->createCommandBuffers ( frames, device->commandPool );

			if (2 > 0) {
				fontText.device = device;
				fontText.frames = frames;
				fontText.descriptorPool = descriptorPool;



				//fontText.initialize ();

				textManager = new TextEntity ( MSDF,
					"hola",
					"C:\\source\\2024\\Sevian\\Engine\\fonts\\arial.ttf",
					//"C:\\source\\2024\\Sevian\\Engine\\fonts\\hemi.otf",

					device,
					frames,
					descriptorPool

				);

				texto = textManager->createText ( "hello!" );
			}

			meshManager = new MeshManager ( device, textureManager.get () );
		}


		void VulkanRenderer::renderFrame () { }


		void VulkanRenderer::cleanup () {
			// Inicialización específica de Vulkan
		}

		bool VulkanRenderer::windowShouldClose () {
			return glfwWindowShouldClose ( window );
		}





		void VulkanRenderer::initEntity ( Info3D info ) {

			//auto entity = std::make_unique<Entity> ();


			//std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( frames, sizeof ( UniformBufferObject2 ) );

			//auto texture = mTextures[info.texture.c_str ()].get();
			////auto texture = mTextures[info.texture.c_str ()];
			//entity->descriptorSets = device->createDescriptorSets ( ubo,  texture->imageView, texture->sampler, sizeof ( UniformBufferObject2 ) );
			//auto attributeDescriptions = getAttributeDescriptions ();
			//auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

			//entity->vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
			//entity->indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
			//entity->indicesSizes = info.indices.size ();
			////device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

			//entity->pipeline = pipeline.pipeline;
			//entity->pipelineLayout = pipeline.pipelineLayout;
			//entity->ubo = ubo;

			//entities[info.entityId] = std::move ( entity );


		}

		void VulkanRenderer::drawEntity ( uint32_t entityId, glm::vec3 position, Camera camera ) {

			auto entity = entities[entityId].get ();

			//auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);

			if (entity) {

				Frame frame = frames[device->currentFrame];
				updateUniformBuffer ( entity->ubo[device->currentFrame].buffersMapped, position, camera );



				VkBuffer vertexBuffers[] = { entity->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipeline );
				vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( commandBuffer, entity->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipelineLayout, 0, 1, &entity->descriptorSets[device->currentFrame], 0, nullptr );
				vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(entity->indicesSizes), 1, 0, 0, 0 );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}
		}

		void VulkanRenderer::initEntity ( RENDERER::Entity3D* unit ) {
		}

		void VulkanRenderer::drawEntity ( RENDERER::Entity3D* unit, glm::vec3 position, Camera ) {
		}



		void VulkanRenderer::cleanupSwapChain () {

			vkDestroyImageView ( device->device, depthResources.imageView, nullptr );
			vkDestroyImage ( device->device, depthResources.image, nullptr );
			vkFreeMemory ( device->device, depthResources.imageMemory, nullptr );

			for (auto framebuffer : swapChain.framebuffers) {
				vkDestroyFramebuffer ( device->device, framebuffer, nullptr );
			}

			for (auto imageView : swapChain.imageViews) {
				vkDestroyImageView ( device->device, imageView, nullptr );
			}

			vkDestroySwapchainKHR ( device->device, swapChain.swapchain, nullptr );
		}

		void VulkanRenderer::recreateSwapChain () {

			int width = 0, height = 0;
			glfwGetFramebufferSize ( window, &width, &height );
			while (width == 0 || height == 0) {
				glfwGetFramebufferSize ( window, &width, &height );
				glfwWaitEvents ();
			}

			vkDeviceWaitIdle ( device->device );

			cleanupSwapChain ();


			int position = 0;

			swapChain = physicalDevices.at ( position ).createSwapChain ( window );
			depthResources = device->createDepthResources ( swapChain.extent );
			swapChain.framebuffers = physicalDevices.at ( position ).createFramebuffers ( swapChain, renderPass, { depthResources.imageView } );
		}

		void VulkanRenderer::addTexture ( TextureInfo info ) {

			textureManager->add ( info );
		}

		void VulkanRenderer::addTexture ( std::string name, std::string path ) {
			if (mTextures.find ( name ) != mTextures.end ()) {

				std::cerr << "Warning: Texture with name '" << name << "' already exists. Skipping assignment." << std::endl;
				return;
			}

			mTextures[name] = device->createTexture ( path.c_str () );
		}

		void VulkanRenderer::addShaders ( std::string, std::string ) {
		}
		void VulkanRenderer::updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position, Camera camera ) {
			static auto startTime = std::chrono::high_resolution_clock::now ();

			auto currentTime = std::chrono::high_resolution_clock::now ();
			float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

			UniformBufferObject2 ubo {};

			//std::cout << position.x << ":" << position.y << ":" << position.x << "\n";
			glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), position );
			glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
			ubo.model = /* rotation * */  translation;


			// Posición de la cámara arriba en el eje Z
			glm::vec3 cameraPos = camera.position;//glm::vec3 ( 0.0f, 0.0f, -5.0f );

			//glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 5.0f );

			// Punto al que está mirando la cámara (el origen en este caso)
			glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
			// Dirección "up" (hacia el eje Y)
			//glm::vec3 up = camera.up; // glm::vec3 ( 0.0f, 1.0f, 0.0f );
			glm::vec3 up = glm::vec3 ( 0.0f, 10.0f, 0.0f );

			ubo.view = glm::lookAt ( cameraPos, target, up );

			//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
			ubo.proj = glm::perspective ( glm::radians ( 45.0f ), extent.width / (float) extent.height, 0.1f, 100.0f );

			//ubo.view = glm::mat4 ( 1.0f );
			//ubo.proj = glm::mat4 ( 1.0f );

			ubo.proj[1][1] *= -1;

			memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
		}

		void VulkanRenderer::updateUniformBuffer ( Frame frame ) {
			static auto startTime = std::chrono::high_resolution_clock::now ();

			auto currentTime = std::chrono::high_resolution_clock::now ();
			float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

			UniformBufferObject2 ubo {};
			ubo.model = glm::rotate ( glm::mat4 ( 1.0f ), time * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
			ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
			ubo.proj = glm::perspective ( glm::radians ( 45.0f ), extent.width / (float) extent.height, 0.1f, 10.0f );
			ubo.proj[1][1] *= -1;

			memcpy ( frame.uniformBuffersMapped, &ubo, sizeof ( ubo ) );
		}

		void VulkanRenderer::setViewport ( float width, float height ) {
			viewport.width = width;
			viewport.height = height;
		}

		RENDERER::Viewport VulkanRenderer::getViewport () {
			return viewport;
		}

		void VulkanRenderer::beginFrame () {
			Frame frame = device->nextFrame ();


			vkWaitForFences ( device->device, 1, &frame.inFlightFences, VK_TRUE, UINT64_MAX );

			//uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR ( device->device, swapChain.swapchain, UINT64_MAX, frame.imageAvailableSemaphores, VK_NULL_HANDLE, &imageIndex );

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				recreateSwapChain ();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error ( "failed to acquire swap chain image!" );
			}

			vkResetFences ( device->device, 1, &frame.inFlightFences );
			vkResetCommandBuffer ( frame.commandBuffer, /*VkCommandBufferResetFlagBits*/ 0 );

			commandBuffer = frame.commandBuffer;

			/* record command buffer */
			VkCommandBufferBeginInfo beginInfo {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			if (vkBeginCommandBuffer ( frame.commandBuffer, &beginInfo ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to begin recording command buffer!" );
			}



		}

		void VulkanRenderer::endFrame () {

			Frame frame = device->getFrame ();
			auto commandBuffer = frame.commandBuffer;


			if (vkEndCommandBuffer ( commandBuffer ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to record command buffer!" );
			}


			VkSubmitInfo submitInfo {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { frame.imageAvailableSemaphores };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &frame.commandBuffer;

			VkSemaphore signalSemaphores[] = { frame.renderFinishedSemaphores };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit ( graphicsQueue, 1, &submitInfo, frame.inFlightFences ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to submit draw command buffer!" );
			}

			VkPresentInfoKHR presentInfo {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { swapChain.swapchain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &imageIndex;


			VkResult result = vkQueuePresentKHR ( presentQueue, &presentInfo );

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {

				std::cout << "error error \n";
				framebufferResized = false;
				recreateSwapChain ();
			}
			else if (result != VK_SUCCESS) {
				throw std::runtime_error ( "failed to present swap chain image!" );
			}

			//device->currentFrame = (device->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		}

		void VulkanRenderer::beginRenderPass ( int index ) {
			Frame frame = device->getFrame ();


			if (index == 0) {

				std::array<VkClearValue, 1> clearValues {};

				clearValues[0].depthStencil = { 1.0f, 0 };

				VkRenderPassBeginInfo renderPassInfo {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = shadowWorld->renderPass; //shadowRenderPass;
				renderPassInfo.framebuffer = shadowWorld->frameBuffer;// shadowFrameBuffer;
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = shadowExtent;

				renderPassInfo.clearValueCount = 1;// static_cast<uint32_t>(clearValues.size ());
				renderPassInfo.pClearValues = clearValues.data ();

				vkCmdBeginRenderPass ( frame.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

				VkViewport viewport {};
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = (float) shadowExtent.width;
				viewport.height = (float) shadowExtent.height;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				vkCmdSetViewport ( frame.commandBuffer, 0, 1, &viewport );

				VkRect2D scissor {};
				scissor.offset = { 0, 0 };
				scissor.extent = shadowExtent;
				vkCmdSetScissor ( frame.commandBuffer, 0, 1, &scissor );

				// Establecer el depth bias dinámico
				float depthBiasConstantFactor = 1.25f; // Cambia según tu necesidad
				float depthBiasClamp = 0.0f;
				float depthBiasSlopeFactor = 1.75f; // Cambia según tu necesidad

				vkCmdSetDepthBias ( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );

				return;
			}
			else {
				std::array<VkClearValue, 2> clearValues {};
				clearValues[0].color = { {0.0f, 0.5f, 0.0f, 1.0f} };
				clearValues[1].depthStencil = { 1.0f, 0 };

				VkRenderPassBeginInfo renderPassInfo {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = device->renderPass;
				renderPassInfo.framebuffer = swapChain.framebuffers[imageIndex];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = swapChain.extent;
				renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size ());
				renderPassInfo.pClearValues = clearValues.data ();

				vkCmdBeginRenderPass ( frame.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );


				VkViewport viewport {};
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = (float) swapChain.extent.width;
				viewport.height = (float) swapChain.extent.height;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				vkCmdSetViewport ( frame.commandBuffer, 0, 1, &viewport );

				VkRect2D scissor {};
				scissor.offset = { 0, 0 };
				scissor.extent = swapChain.extent;
				vkCmdSetScissor ( frame.commandBuffer, 0, 1, &scissor );
			}



		}

		void VulkanRenderer::endRenderPass () {
			Frame frame = device->getFrame ();

			vkCmdEndRenderPass ( frame.commandBuffer );
		}

		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::Entity3D> prop, glm::vec3 position, Camera camera ) {

			auto entity = std::dynamic_pointer_cast<Entity>(prop);

			//auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);

			if (entity) {

				Frame frame = device->getFrame ();
				updateUniformBuffer ( entity->ubo[device->currentFrame].buffersMapped, position, camera );

				VkBuffer vertexBuffers[] = { entity->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipeline );
				vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( commandBuffer, entity->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipelineLayout, 0, 1, &entity->descriptorSets[device->currentFrame], 0, nullptr );
				vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(entity->indicesSizes), 1, 0, 0, 0 );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}


			/*
			auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);

			if (vulkanProp) {

				Frame frame = frames[currentFrame];
				updateUniformBuffer ( vulkanProp->buffers[currentFrame].buffersMapped, position, camera);



				VkBuffer vertexBuffers[] = { vulkanProp->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanProp->pipeline );
				vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( commandBuffer, vulkanProp->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanProp->pipelineLayout, 0, 1, &vulkanProp->descriptorSets[currentFrame], 0, nullptr );
				vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(vulkanProp->indicesSizes), 1, 0, 0, 0 );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}
			*/
		}

		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::Entity3D> prop, UniformBufferObject ubo ) {
			auto entity = std::dynamic_pointer_cast<Entity>(prop);

			//auto vulkanProp = std::dynamic_pointer_cast<VulkanProperty>(prop);
			ve;
			ve->render ( ubo );
			return;

			if (entity) {

				Frame frame = device->getFrame ();

				MeUBO me = {};
				me.color = glm::vec3 ( 0.99f, 0.026f, 0.011f );
				me.color2 = glm::vec3 ( 0.5f, 0.0f, 1.0f );
				me.intensity = 2.1f - 0.11f;


				LightUBO l = { };

				l.position = glm::vec3 ( 0.6f, 0.05f, 0.1f );
				l.color = glm::vec3 ( 0.6f, 0.3f, 0.0f );
				l.intensity = 0.5f;


				UniformBufferObject ubo2 {};


				glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), l.position );
				//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

				glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), l.color.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
					glm::rotate ( glm::mat4 ( 1.0f ), l.color.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
					glm::rotate ( glm::mat4 ( 1.0f ), l.color.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
				ubo2.model = translation * rotationMat;


				//ubo.model = /* rotation * */  translation;


				// Posición de la cámara arriba en el eje Z
				glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, -5.0f );

				//glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 5.0f );

				// Punto al que está mirando la cámara (el origen en este caso)
				glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
				// Dirección "up" (hacia el eje Y)
				//glm::vec3 up = camera.up; // glm::vec3 ( 0.0f, 1.0f, 0.0f );
				glm::vec3 up = glm::vec3 ( 0.0f, -25.0f, 0.0f );

				ubo2.view = glm::lookAt ( cameraPos, target, up );

				//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
				ubo2.proj = glm::perspective ( glm::radians ( 45.0f ), 1300 / (float) 600, 0.1f, 100.0f );


				memcpy ( entity->light[device->currentFrame].buffersMapped, &l, sizeof ( l ) );
				memcpy ( entity->me[device->currentFrame].buffersMapped, &me, sizeof ( me ) );
				memcpy ( entity->ubo[device->currentFrame].buffersMapped, &ubo, sizeof ( ubo ) );
				//memcpy ( entity->ubo2[currentFrame].buffersMapped, &ubo2, sizeof ( ubo2 ) );


				VkBuffer vertexBuffers[] = { entity->vertex.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipeline );
				vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( commandBuffer, entity->indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, entity->pipelineLayout, 0, 1, &entity->descriptorSets[device->currentFrame], 0, nullptr );
				vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(entity->indicesSizes), 1, 0, 0, 0 );

				entity->render ( ubo );
			}
			else {
				std::cerr << "Error: PropertyRender no es una instancia de VulkanProperty" << std::endl;
			}

		}

		void VulkanRenderer::drawText ( std::string text, glm::vec3 position, Camera camera ) {

			//fontText.draw (			text, currentFrame, commandBuffer, position, camera, extent.width, extent.height );


			//textManager->draw ( text, device->currentFrame, commandBuffer, position, camera, extent.width, extent.height );
			texto->render ( extent.width, extent.height );

		}

		void VulkanRenderer::draw ( std::vector<RENDERER::iElement> components, RENDERER::GlobalInfo info ) {


		}

		void updateUniformBuffer1 ( void* uniformBuffersMapped, glm::vec3 position, uint32_t width, uint32_t height ) {
			static auto startTime = std::chrono::high_resolution_clock::now ();

			// Calcular el tiempo transcurrido
			auto currentTime = std::chrono::high_resolution_clock::now ();
			float time = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - startTime ).count ();

			UniformBufferObject2 ubo {};

			// Calcular el aspecto
			float aspectRatio = width / static_cast<float>(height);

			// Elegir el tipo de proyección (comentar/descomentar según se necesite)
			// Proyección Perspectiva (comentada si usas ortográfica)
			// ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
			// ubo.proj[1][1] *= -1;  // Invertir el eje Y para el espacio de recorte de Vulkan
			auto delta = 300.0f;
			// Proyección Ortográfica (descomentada si usas ortográfica)
			ubo.proj = glm::ortho ( -delta * aspectRatio, delta * aspectRatio, -delta, delta, 1.0f, 5.0f );
			//ubo.proj = glm::ortho ( 0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height) );
			ubo.proj = glm::ortho ( -static_cast<float>(width), static_cast<float>(width), -static_cast<float>(height), static_cast<float>(height), 5.0f, -5.0f );
			ubo.proj = glm::ortho ( -static_cast<float>(width), static_cast<float>(width), -static_cast<float>(height), static_cast<float>(height) );

			auto deltaX = static_cast<float>(width) / 2;
			auto deltaY = static_cast<float>(height) / 2;
			ubo.proj = glm::ortho ( -deltaX, deltaX, -deltaY, deltaY, 1.0f, 25.0f );

			ubo.proj = glm::ortho ( -deltaX, deltaX, -deltaY, deltaY );
			// Sin invertir el eje Y para una proyección ortográfica
			ubo.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o OpenGL con un espacio de recorte diferente

			// Modelo y Vista (sin rotación en este caso)
			ubo.model = glm::mat4 ( 1.0f );  // No hay rotación aplicada
			ubo.view = glm::lookAt ( glm::vec3 ( 0.0f, 0.0f, 2.0f ),  // Cámara desde (0, 0, 2)
				glm::vec3 ( 0.0f, 0.0f, 0.0f ),  // Mira al origen
				glm::vec3 ( 0.0f, 1.0f, 0.0f ) ); // "Arriba" en la dirección Y

			// Copiar los datos del UBO en la memoria mapeada


			ubo.view = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( -glm::vec2 ( 0.0f, 0.0f ), 0.0f ) );





			if (1 == 2) {
				ubo.proj = glm::ortho ( -deltaX, deltaX, 0.0f, deltaY * 2.0f );
				//camera.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o Open
			}
			else {
				ubo.proj = glm::ortho ( 0.0f, deltaX * 2.0f, 0.0f, deltaY * 2.0f );
				//camera.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o Open
			}

			//camera.proj = glm::ortho ( 0.0f, deltaX * 2.0f, -deltaY, deltaY, -1.0f, 1.0f );
			ubo.proj[1][1] *= -1;



			memcpy ( uniformBuffersMapped, &ubo, sizeof ( ubo ) );
		}

		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::iElement> _object, RENDERER::GlobalInfo info ) {
			auto object = std::dynamic_pointer_cast<Element1>(_object);
			if (object) {
				uint32_t frameIndex = device->currentIndex ();

				Frame frame = device->getFrame ();
				updateUniformBuffer1 ( object->ubo[frameIndex].buffersMapped, glm::vec3 ( 0.0f ), width, height );



				VkBuffer vertexBuffers[] = { object->vertexBuffer.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipeline );
				vkCmdBindVertexBuffers ( frame.commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( frame.commandBuffer, object->indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object->pipelineLayout, 0, 1, &object->descriptorSets[frameIndex], 0, nullptr );
				vkCmdDrawIndexed ( frame.commandBuffer, static_cast<uint32_t>(object->indicesSizes), 1, 0, 0, 0 );
			}


		}

		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::IRenderizable> object ) {
			//auto element = std::dynamic_pointer_cast<Element1>(_element);

			auto element = std::dynamic_pointer_cast<VKElement>(object);
			if (element) {
				uint32_t frameIndex = device->currentIndex ();

				Frame frame = device->getFrame ();
				updateUniformBuffer1 ( element->ubo[frameIndex].buffersMapped, glm::vec3 ( 0.0f ), width, height );

				VkBuffer vertexBuffers[] = { element->vertexBuffer.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipeline );
				vkCmdBindVertexBuffers ( frame.commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( frame.commandBuffer, element->indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipelineLayout, 0, 1, &element->descriptorSets[frameIndex], 0, nullptr );
				vkCmdDrawIndexed ( frame.commandBuffer, static_cast<uint32_t>(element->indicesSizes), 1, 0, 0, 0 );

				for (auto& _element : element->childs) {
					draw ( _element );
				}
			}
		}
		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::IRenderizable> object, RENDERER::GlobalInfo info ) {
			//auto element = std::dynamic_pointer_cast<Element1>(_element);

			auto element = std::dynamic_pointer_cast<VKElement>(object);
			if (element) {
				uint32_t frameIndex = device->currentIndex ();

				Frame frame = device->getFrame ();
				//updateUniformBuffer1 ( element->ubo[frameIndex].buffersMapped, glm::vec3 ( 0.0f ), width, height );
				memcpy ( element->ubo[frameIndex].buffersMapped, &info.ubo, sizeof ( info.ubo ) );
				VkBuffer vertexBuffers[] = { element->vertexBuffer.buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipeline );
				vkCmdBindVertexBuffers ( frame.commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( frame.commandBuffer, element->indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipelineLayout, 0, 1, &element->descriptorSets[frameIndex], 0, nullptr );
				vkCmdDrawIndexed ( frame.commandBuffer, static_cast<uint32_t>(element->indicesSizes), 1, 0, 0, 0 );

				for (auto& _element : element->childs) {
					draw ( _element, info );
				}
			}
		}

		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::IRenderizable> object, Camera2D camera ) {

			auto element = std::dynamic_pointer_cast<VKElement>(object);

			if (element) {
				if (!element->pipeline) {
					return;
				}

				uint32_t frameIndex = device->currentIndex ();

				Frame frame = device->getFrame ();
				auto& elementFrame = element->frames[frameIndex];

				vkCmdBindPipeline ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipeline );

				VkBuffer vertexBuffers[] = { element->vertexBuffer.buffer };
				VkDeviceSize offsets[] = { 0 };

				for (auto& f : elementFrame.memoryData) {

					auto ubo = element->getData ( f.first );

					//UniformBufferObject ubo2 = static_cast<UniformBufferObject>(ubo);
                    // Reemplaza la línea problemática con un constructor adecuado
                    //UniformBufferObject ubo2 = *static_cast<UniformBufferObject*>(ubo);
					if (ubo) {
						memcpy ( f.second.buffersMapped, ubo, sizeof ( UniformBufferObject ) );
					}


				}

				for (auto& p : element->pushConstantsInfo) {

					auto pushData = element->getData ( p->id );


					
					//SEVIAN::Border border {};
				
					//border.color = glm::vec4 ( 0.2f, 0.3f, 0.5f, 1.0f );


					vkCmdPushConstants ( frame.commandBuffer, element->pipelineLayout, p->stage, p->offset, p->size, pushData );

				}

				

				vkCmdBindVertexBuffers ( frame.commandBuffer, 0, 1, vertexBuffers, offsets );
				//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindIndexBuffer ( frame.commandBuffer, element->indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT32 );

				vkCmdBindDescriptorSets ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipelineLayout, 0, 1, &element->frames[frameIndex].descriptorSets[0], 0, nullptr );
				vkCmdDrawIndexed ( frame.commandBuffer, static_cast<uint32_t>(element->indicesSizes), 1, 0, 0, 0 );

				//element->render ( frame.commandBuffer, frameIndex, camera );
				for (auto& _element : element->childs) {
					draw ( _element, camera );
				}
			}
		}

		void VulkanRenderer::draw ( std::shared_ptr<RENDERER::RenderObject> object, RENDERER::GlobalInfo info ) {


			for (auto& _element : object->elements) {

				//auto element = std::dynamic_pointer_cast<Element1>(_element);

				auto element = std::dynamic_pointer_cast<Element1>(_element);
				if (element) {
					uint32_t frameIndex = device->currentIndex ();

					Frame frame = device->getFrame ();
					updateUniformBuffer1 ( element->ubo[frameIndex].buffersMapped, glm::vec3 ( 0.0f ), width, height );

					VkBuffer vertexBuffers[] = { element->vertexBuffer.buffer };
					VkDeviceSize offsets[] = { 0 };

					vkCmdBindPipeline ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipeline );
					vkCmdBindVertexBuffers ( frame.commandBuffer, 0, 1, vertexBuffers, offsets );
					//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
					vkCmdBindIndexBuffer ( frame.commandBuffer, element->indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT32 );
					//vkCmdBindIndexBuffer(command_buffer, rb->IndexBuffer, 0, sizeof(ImDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
					vkCmdBindDescriptorSets ( frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, element->pipelineLayout, 0, 1, &element->descriptorSets[frameIndex], 0, nullptr );
					vkCmdDrawIndexed ( frame.commandBuffer, static_cast<uint32_t>(element->indicesSizes), 1, 0, 0, 0 );
				}
			}
		}


		std::unique_ptr<RENDERER::Entity3D> VulkanRenderer::createEntity ( Info3D info ) {

			if (0 == 0) {
				auto entity = meshManager->create ( info );
				return std::unique_ptr<RENDERER::Entity3D> ( std::move ( entity ) );
			}

			if (1 == 1) {
				//auto entity = std::make_unique<MeshEntity> ( device, textureManager.get (), info );
				//return std::unique_ptr<Entity3D> ( std::move ( entity ) );
			}

			auto entity = std::make_unique<VulkanEntity> ( device, textureManager.get (), info );
			//auto entity = std::make_unique<Entity3D> ();

			return std::unique_ptr<RENDERER::Entity3D> ( std::move ( entity ) );
			/*
			auto x = new VulkanEntity (
				device, textureManager.get(), info
			);

			ve = x;
			auto entity = std::make_unique<Entity3D> ();


			std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
			std::vector<VulkanUBuffer> ubo2 = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
			std::vector<VulkanUBuffer> lightUBO = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );
			std::vector<VulkanUBuffer> meUBO = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );

			auto texture = mTextures[info.texture].get ();
			//auto texture = mTextures[info.texture.c_str ()];
			std::vector<BufferInfo> buffersInfo;

			buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );
			//buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo2, sizeof ( UniformBufferObject ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
			buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, lightUBO, sizeof ( LightUBO ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
			//
			buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ubo, sizeof ( UniformBufferObject ), texture->imageView, texture->sampler, 2 } );
			buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, meUBO, sizeof ( MeUBO ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 3 } );

			entity->descriptorSets = device->createDescriptorSets ( buffersInfo );
			//entity->descriptorSets = device->createDescriptorSets ( ubo, texture->imageView, texture->sampler, sizeof ( UniformBufferObject ) );


			auto attributeDescriptions = getAttributeDescriptions ();
			//auto pipeline = createGraphicsPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout );

			if (pipeline.pipeline == VK_NULL_HANDLE) {

				//auto fragShaderCode = device->readFile ( "shaders/solid_f.spv" );
				pipeline = device->createGraphPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout, "shaders/total_v.spv", "shaders/total_f.spv" );
				//pipeline = device->createGraphPipeline ( getBindingDescription (), attributeDescriptions, device->descriptorSetLayout, "shaders/lights_v.spv", "shaders/lights_f.spv" );
			}

			entity->vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
			entity->indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
			entity->indicesSizes = info.indices.size ();
			//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );

			entity->pipeline = pipeline.pipeline;
			entity->pipelineLayout = pipeline.pipelineLayout;
			entity->ubo = ubo;
			entity->ubo2 = ubo2;
			entity->light = lightUBO;
			entity->me = meUBO;

			//entities[info.entityId] = std::move ( entity );

			//return static_cast<PropertyRender&>(*entity);
			return std::unique_ptr<PropertyRender> ( std::move ( entity ) );
			//return std::shared_ptr<PropertyRender> ( std::move ( entity ) );
			*/
		}







		void VulkanRenderer::initWindow () {
			glfwInit ();

			glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );

			window = glfwCreateWindow ( width, height, title.c_str (), nullptr, nullptr );
			glfwSetWindowUserPointer ( window, this );
			//glfwSetFramebufferSizeCallback ( window, this->resizeCallback );

			glfwSetKeyCallback ( window, keyCallback );


		}

		bool VulkanRenderer::checkValidationLayerSupport ()

		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties ( &layerCount, nullptr );

			std::vector<VkLayerProperties> availableLayers ( layerCount );
			vkEnumerateInstanceLayerProperties ( &layerCount, availableLayers.data () );

			for (const char* layerName : validationLayers) {
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp ( layerName, layerProperties.layerName ) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		std::vector<const char*> VulkanRenderer::getRequiredExtensions () {
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions ( &glfwExtensionCount );

			std::vector<const char*> extensions ( glfwExtensions, glfwExtensions + glfwExtensionCount );

			if (enableValidationLayers) {
				extensions.push_back ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
			}

			return extensions;
		}

		void VulkanRenderer::populateDebugMessengerCreateInfo ( VkDebugUtilsMessengerCreateInfoEXT& createInfo ) {
			createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
		}

		void VulkanRenderer::createInstance () {
			if (enableValidationLayers && !checkValidationLayerSupport ()) {
				throw std::runtime_error ( "validation layers requested, but not available!" );
			}

			VkApplicationInfo appInfo {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION ( 1, 0, 0 );
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION ( 1, 0, 0 );
			appInfo.apiVersion = VK_API_VERSION_1_1;

			VkInstanceCreateInfo createInfo {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = getRequiredExtensions ();
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size ());
			createInfo.ppEnabledExtensionNames = extensions.data ();

			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
			if (enableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size ());
				createInfo.ppEnabledLayerNames = validationLayers.data ();

				populateDebugMessengerCreateInfo ( debugCreateInfo );
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
			}
			else {
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			if (vkCreateInstance ( &createInfo, nullptr, &instance ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create instance!" );
			}
		}

		VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT ( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger ) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr ( instance, "vkCreateDebugUtilsMessengerEXT" );
			if (func != nullptr) {
				return func ( instance, pCreateInfo, pAllocator, pDebugMessenger );
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void VulkanRenderer::setupDebugMessenger () {
			if (!enableValidationLayers) return;

			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			populateDebugMessengerCreateInfo ( createInfo );

			if (CreateDebugUtilsMessengerEXT ( instance, &createInfo, nullptr, &debugMessenger ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to set up debug messenger!" );
			}
		}

		void VulkanRenderer::createSurface () {

			if (glfwCreateWindowSurface ( instance, window, nullptr, &surface ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create window surface!" );
			}
		}


		void VulkanTextureNO::load ( const std::string& path ) {
			// Inicialización específica de Vulkan
		}

		void VulkanShader::load ( const std::string& vertexPath, const std::string& fragmentPath ) {
			// Inicialización específica de Vulkan
		}






	}
}