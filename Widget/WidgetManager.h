#pragma once
#include <string>
#include <glm/glm.hpp> 
#include <memory>
#include "Widget.h"
#include "LabelWidget.h"
#include <vector>
#include <RenderInterface.h>
#include "IInputManager.h"
#include "StackingContext.h"

namespace SEVIAN {

	namespace WIDGET {

		class WidgetManager : public Widget
		{
		private:
			//std::vector<std::shared_ptr<Widget>> widgets;
			std::shared_ptr<RENDERER::RenderInterface> renderer;
			std::shared_ptr<INPUT::IInputManager> inputManager;
			RENDERER::iWidgetManager* widgetRenderer;
			float lastMouseX = 0.0f;
			float lastMouseY = 0.0f;

			Widget* lastWidget = nullptr;
			Widget* lastMouseDown = nullptr;
			Widget* lastClickWidget = nullptr;

			int clickCount = 0;
			std::chrono::steady_clock::time_point lastClickTime;

			// Estado de arrastre
			bool isDragging = false;
			Widget* draggedWidget = nullptr;
			glm::vec2 dragStartPos = { 0, 0 };
			glm::vec2 dragOffset = { 0, 0 };
			Widget* lastDragOverWidget = nullptr;

			// Estado de focus
			Widget* focusedWidget = nullptr;

			std::vector<Widget*> allWidgets;
			void collectWidgets ( Widget* widget ) {
				allWidgets.push_back ( widget );
				for (auto& child : widget->children) {
					collectWidgets ( child.get () );
				}
			}

			//StackingContextNode buildStackingContextTree ( Widget* root );

			void collectContextStack ( Widget* widget, StackingContextNode* context );
			void renderStackingContext ( const StackingContextNode* node, Camera2D camera );

			void renderWidget( Widget* target, Camera2D camera );
			StyleUbo styleUbo;
		public:

			WidgetManager ( std::shared_ptr<RENDERER::RenderInterface> renderer, std::shared_ptr<INPUT::IInputManager> inputManager );

			//void update ( float deltaTime );
			void render ( std::shared_ptr<RENDERER::RenderInterface> render, Camera2D camera );
			//std::shared_ptr<RENDERER::IRenderizable> getRenderObject () override;
			std::vector<Widget*> buildPropagationChain ( Widget* target );

			void onKeyPress ( INPUT::Key key, INPUT::KeyAction action, int mods );
			void onMouseMove ( float x, float y );
			void onMouseButton ( int button, INPUT::KeyAction action, float x, float y );
			
			void doFocus ( Widget* widget );
			void clearFocus ();
			Widget* getFocusedWidget () const { return focusedWidget; }
			

			Widget* getWidgetAt ( float x, float y );

			glm::mat4 getModelMatrix () override;

		};
	}
}