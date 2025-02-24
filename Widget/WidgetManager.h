#pragma once
#include <string>
#include <glm/glm.hpp> 
#include <memory>
#include "Widget.h"
#include "LabelWidget.h"
#include <vector>
#include <RenderInterface.h>
#include "IInputManager.h"

namespace SEVIAN {

	namespace WIDGET {

		class WidgetManager : public Widget
		{
		private:
			//std::vector<std::shared_ptr<Widget>> widgets;
			std::shared_ptr<RENDERER::RenderInterface> renderer;
			std::shared_ptr<INPUT::IInputManager> inputManager;
			float lastMouseX = 0.0f;
			float lastMouseY = 0.0f;

			Widget* lastWidget = nullptr;
			Widget* lastMouseDown = nullptr;
			Widget* lastClickWidget = nullptr;

			int clickCount = 0;
			std::chrono::steady_clock::time_point lastClickTime;

		public:

			WidgetManager ( std::shared_ptr<RENDERER::RenderInterface> renderer, std::shared_ptr<INPUT::IInputManager> inputManager );

			void update ( float deltaTime );
			void render ( std::shared_ptr<RENDERER::RenderInterface> render, Camera2D camera );
			std::shared_ptr<RENDERER::IRenderizable> getRenderObject () override;
			std::vector<Widget*> buildPropagationChain ( Widget* target );
			bool dispatchEvent ( Widget* root, Event& event );

			void onKeyPress ( INPUT::Key key, INPUT::KeyAction action, int mods );
			void onMouseMove ( float x, float y );
			void onMouseButton ( int button, INPUT::KeyAction action, float x, float y );

			Widget* getWidgetAt ( float x, float y );

		};
	}
}