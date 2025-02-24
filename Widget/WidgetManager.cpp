#include "WidgetManager.h"
#include <algorithm>

namespace SEVIAN::WIDGET {

	WidgetManager::WidgetManager ( std::shared_ptr<RENDERER::RenderInterface> renderer, std::shared_ptr<INPUT::IInputManager> inputManager ) :
		renderer ( renderer ), inputManager ( inputManager ) {


		inputManager->subscribeKeyInput ( [&]( INPUT::Key key, INPUT::KeyAction action, int mods ) {
			onKeyPress ( key, action, mods );
			} );

		inputManager->subscribeMouseMoveInput ( [&]( double x, double y ) {
			onMouseMove ( x, y );
			} );

		inputManager->subscribeMouseButtonInput ( [&]( int button, INPUT::KeyAction action, int mods ) {
			onMouseButton ( button, action, lastMouseX, lastMouseY );
			} );
	}

	void WidgetManager::update ( float deltaTime ) {
		for (auto& widget : children) {
			widget->update ( deltaTime );
		}
	}

	void WidgetManager::render ( std::shared_ptr<RENDERER::RenderInterface> render, Camera2D camera ) {

		for (auto& widget : children) {
			UniformBufferObject ubo {};

			ubo.model = widget->getModelMatrix ();
			ubo.view = camera.getView ();
			ubo.proj = camera.getProjection ();

			render->draw ( widget->getRenderObject (), { ubo } );

			/*if (widget->isVisible)
				widget->render ( );
				*/
		}
	}

	std::shared_ptr<RENDERER::IRenderizable> WidgetManager::getRenderObject () {

		return std::shared_ptr<RENDERER::IRenderizable> ();
	}

	std::vector<Widget*> WidgetManager::buildPropagationChain ( Widget* target ) {

		std::vector<Widget*> chain;
		auto current = target;
		while (current) {
			chain.push_back ( current );
			current = current->parent;
		}
		std::reverse ( chain.begin (), chain.end () );
		return chain;
	}

	bool WidgetManager::dispatchEvent ( Widget* root, Event& event ) {
		// Se busca el widget objetivo usando hitTest a partir de la raíz
		//auto target = root->getWidgetAt ( event.x, event.y );

		Widget* target = nullptr;
		if (event.type == EventType::MouseLeave && lastWidget) {
			target = lastWidget;
		}
		else if (root ){
			target = root->getWidgetAt ( event.x, event.y );
		}


		if (!target) return false; // Si no se encontró widget, no se hace nada

		// Se construye la cadena de widgets desde la raíz hasta el widget objetivo
		auto chain = buildPropagationChain ( target );

		// Fase de captura: desde la raíz hasta el padre del widget objetivo
		for (size_t i = 0; i < chain.size () - 1; ++i) {
			auto widget = chain[i];
			auto it = widget->captureCallbacks.find ( event.type );
			if (it != widget->captureCallbacks.end ()) {
				for (auto& callback : it->second) {

					if (callback ( event )) { // Si la callback consume el evento
						std::cout << "Evento consumido en fase de captura por un widget.\n";
						return true;
					}

				}
			}
		}
		// Fase objetivo: en el widget objetivo se pueden ejecutar primero los callbacks de captura y luego los de burbujeo
		{
			auto widget = chain.back ();
			auto it = widget->captureCallbacks.find ( event.type );
			if (it != widget->captureCallbacks.end ()) {
				for (auto& callback : it->second) {
					if (callback ( event )) {
						std::cout << "Evento consumido en fase objetivo (burbujeo interno) por el widget target.\n";
						return true;
					}
				}
			}
			it = widget->bubbleCallbacks.find ( event.type );
			if (it != widget->bubbleCallbacks.end ()) {
				for (auto& callback : it->second) {
					if (callback ( event )) {
						std::cout << "Evento consumido en fase de burbujeo por un widget ancestro.\n";
						return true;
					}
				}
			}
		}
		// Fase de burbujeo: desde el padre del widget objetivo hasta la raíz (en orden inverso)
		for (int i = static_cast<int>(chain.size ()) - 2; i >= 0; --i) {
			auto widget = chain[i];
			auto it = widget->bubbleCallbacks.find ( event.type );
			if (it != widget->bubbleCallbacks.end ()) {
				for (auto& callback : it->second) {
					if (callback ( event )) {
						std::cout << "Evento consumido en fase de burbujeo por un widget ancestro.\n";
						return true;
					}
				}
			}
		}

		return false;

	}

	void WidgetManager::onKeyPress ( INPUT::Key key, INPUT::KeyAction action, int mods ) {
		std::cout << "  ... . key press: " << "\n";
		Event event { EventType::KeyPress, { 0.0f, 0.0f }, 0.0f, 0.0f, key };
		dispatchEvent ( this, event );
	}



	void WidgetManager::onMouseMove ( float x, float y ) {
		//std::cout << "  ... . mouse move: " << x << " y: " << y << "\n";
		Event event { EventType::MouseMove, { x, y }, x, y, INPUT::Key::UNKNOWN };
		dispatchEvent ( this, event );

		auto currentHovered = getWidgetAt ( x, y );

		if (lastWidget != currentHovered) {
			if (lastWidget) {
				Event event { EventType::MouseLeave, { x, y }, x, y, INPUT::Key::UNKNOWN };
				dispatchEvent ( lastWidget, event );
			}
			if (currentHovered) {
				Event event { EventType::MouseEnter, { x, y }, x, y, INPUT::Key::UNKNOWN };
				dispatchEvent ( currentHovered, event );
			}
			lastWidget = currentHovered;
		}

		lastMouseX = x;
		lastMouseY = y;
	}

	void WidgetManager::onMouseButton ( int button, INPUT::KeyAction action, float x, float y ) {
		lastMouseX = x;
		lastMouseY = y;
		if (button == 0) {
			

			auto currentHovered = getWidgetAt ( x, y );

			if (action == INPUT::KeyAction::PRESS) {
				Event event { EventType::MouseDown, { x, y }, x, y, INPUT::Key::UNKNOWN };
				dispatchEvent ( this, event );

				lastMouseDown = currentHovered;
			}
			else if (action == INPUT::KeyAction::RELEASE) {
				Event event { EventType::MouseUp, { x, y }, x, y, INPUT::Key::UNKNOWN };
				dispatchEvent ( currentHovered, event );

				auto now = std::chrono::steady_clock::now ();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count ();

				if (lastClickWidget == currentHovered &&  duration < 500) {
					clickCount++;
				}
				else {
					clickCount = 1;
				}
				lastClickTime = now;
				lastClickWidget = currentHovered;

				if (clickCount == 2) {
					Event event { EventType::DblClick, { x, y }, x, y, INPUT::Key::UNKNOWN };
					dispatchEvent ( currentHovered, event );
					clickCount = 0;
				}
				else

				
				
				if (lastMouseDown == currentHovered) {
					Event event { EventType::Click, { x, y }, x, y, INPUT::Key::UNKNOWN };
					dispatchEvent ( currentHovered, event );
				}
			}
			std::cout << "  ... . mouse Button 1: " << button << " x: " << lastMouseX << " y: " << lastMouseY << "\n";
			
		}
	}
	
	Widget* WidgetManager::getWidgetAt ( float x, float y ) {
		for (auto& widget : children) {
			if (widget->hitTest ( { x, y } )) {
				// Si el widget tiene hijos, verifica si alguno de ellos está bajo el cursor
				for (auto& child : widget->children) {
					if (child->hitTest ( { x, y } )) {
						return child.get ();
					}
				}
				return widget.get ();
			}
		}
		return nullptr;
	}
	
}


