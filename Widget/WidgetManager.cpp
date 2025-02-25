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

	bool WidgetManager::dispatchEvent1 ( Widget* root, Event& event ) {
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
		Event event {};
		event.type = (action == INPUT::KeyAction::PRESS) ? EventType::KeyDown :
			(action == INPUT::KeyAction::RELEASE) ? EventType::KeyUp : EventType::KeyPress;
		event.key = key;
		event.x = lastMouseX;
		event.y = lastMouseY;

		// Si hay un widget con foco, enviarle el evento primero
		if (focusedWidget) {
			bool consumed = focusedWidget->dispatchEvent ( event );
			if (consumed) {
				return;  // Si el widget con foco consume el evento, terminar
			}
		}

		// Si el widget con foco no consume el evento o no hay widget con foco,
		// seguir con la propagación normal del evento
		// Despachar el evento desde el manager (actuando como raíz)
		this->dispatchEvent ( event );
	}

	void WidgetManager::onMouseMove ( float x, float y ) {
		
		lastMouseX = x;
		lastMouseY = y;

		// Crear evento de MouseMove
		Event moveEvent {};
		moveEvent.type = EventType::MouseMove;
		moveEvent.x = x;
		moveEvent.y = y;

		// Buscar el widget bajo el cursor
		Widget* currentHovered = getWidgetAt ( x, y );

		// Despachar evento de movimiento
		this->dispatchEvent ( moveEvent );

		// Manejar eventos de MouseEnter y MouseLeave
		if (lastWidget != currentHovered) {
			if (lastWidget) {
				Event leaveEvent {};
				leaveEvent.type = EventType::MouseLeave;
				leaveEvent.x = x;
				leaveEvent.y = y;
				leaveEvent.bubbles = false; // MouseLeave no burbujea en JS
				lastWidget->dispatchEvent ( leaveEvent );
			}

			if (currentHovered) {
				Event enterEvent {};
				enterEvent.type = EventType::MouseEnter;
				enterEvent.x = x;
				enterEvent.y = y;
				enterEvent.bubbles = false; // MouseEnter no burbujea en JS
				currentHovered->dispatchEvent ( enterEvent );
			}

			lastWidget = currentHovered;
		}

		// Si tenemos un potencial candidato para arrastre pero aún no ha comenzado el arrastre
		if (draggedWidget && !isDragging) {
			// Calcular la distancia desde el punto de inicio del arrastre
			float deltaX = x - dragStartPos.x;
			float deltaY = y - dragStartPos.y;
			float distanceSquared = deltaX * deltaX + deltaY * deltaY;

			// Si la distancia es suficiente, iniciar el arrastre
			if (distanceSquared > 25.0f) { // Umbral arbitrario (5 píxeles al cuadrado)
				isDragging = true;

				// Disparar evento DragStart
				DragEvent dragStartEvent {};
				dragStartEvent.type = EventType::DragStart;
				dragStartEvent.x = x;
				dragStartEvent.y = y;
				dragStartEvent.dragSource = draggedWidget;
				dragStartEvent.dragStartPosition = dragStartPos;
				dragStartEvent.dragOffset = dragOffset;

				draggedWidget->dispatchEvent ( dragStartEvent );

				// Si el evento fue cancelado, cancelar el arrastre
				if (dragStartEvent.defaultPrevented) {
					isDragging = false;
					draggedWidget = nullptr;
					return;
				}
			}
		}

		// Si estamos en modo arrastre, generar eventos apropiados
		if (isDragging && draggedWidget) {
			// 1. Evento Drag para el widget que se está arrastrando
			DragEvent dragEvent {};
			dragEvent.type = EventType::Drag;
			dragEvent.x = x;
			dragEvent.y = y;
			dragEvent.dragSource = draggedWidget;
			dragEvent.dragStartPosition = dragStartPos;
			dragEvent.dragOffset = dragOffset;

			draggedWidget->dispatchEvent ( dragEvent );

			// Actualizar la posición del widget arrastrado si es draggable
			// Esto es opcional y depende de tu implementación

			/*if (draggedWidget->draggable) {
				draggedWidget->position.x = x - dragOffset.x;
				draggedWidget->position.y = y - dragOffset.y;
			}*/

			// 2. Identificar el widget debajo del cursor para eventos DragEnter/DragOver/DragLeave
			Widget* currentUnderCursor = getWidgetAt ( x, y );

			// Evitar eventos de arrastre en el propio widget arrastrado
			if (currentUnderCursor == draggedWidget) {
				currentUnderCursor = nullptr;
			}

			// Eventos DragEnter/DragLeave
			if (currentUnderCursor != lastDragOverWidget) {
				// Si hay un widget previo, disparar DragLeave
				if (lastDragOverWidget) {
					DragEvent dragLeaveEvent {};
					dragLeaveEvent.type = EventType::DragLeave;
					dragLeaveEvent.x = x;
					dragLeaveEvent.y = y;
					dragLeaveEvent.dragSource = draggedWidget;

					lastDragOverWidget->dispatchEvent ( dragLeaveEvent );
				}

				// Si hay un nuevo widget, disparar DragEnter
				if (currentUnderCursor) {
					DragEvent dragEnterEvent {};
					dragEnterEvent.type = EventType::DragEnter;
					dragEnterEvent.x = x;
					dragEnterEvent.y = y;
					dragEnterEvent.dragSource = draggedWidget;

					currentUnderCursor->dispatchEvent ( dragEnterEvent );
				}

				lastDragOverWidget = currentUnderCursor;
			}

			// Evento DragOver en el widget actual
			if (currentUnderCursor) {
				DragEvent dragOverEvent {};
				dragOverEvent.type = EventType::DragOver;
				dragOverEvent.x = x;
				dragOverEvent.y = y;
				dragOverEvent.dragSource = draggedWidget;

				currentUnderCursor->dispatchEvent ( dragOverEvent );
			}
		}


	}

	void WidgetManager::onMouseButton ( int button, INPUT::KeyAction action, float x, float y ) {
		// Solo manejar el botón izquierdo por ahora (button 0)
		if (button == 0) {
			Widget* currentHovered = getWidgetAt ( x, y );

			if (action == INPUT::KeyAction::PRESS) {


				// Crear y despachar evento MouseDown
				Event downEvent {};
				downEvent.type = EventType::MouseDown;
				downEvent.x = x;
				downEvent.y = y;

				if (currentHovered) {
					currentHovered->dispatchEvent ( downEvent );
				}
				else {
					this->dispatchEvent ( downEvent );
				}

				lastMouseDown = currentHovered;

				if (currentHovered) {
					// Guardar información para potencial inicio de arrastre
					dragStartPos = { x, y };

					// Calculamos el offset: distancia entre la posición del ratón y la esquina del widget
					dragOffset.x = x - currentHovered->position.x;
					dragOffset.y = y - currentHovered->position.y;

					// Almacenamos temporalmente el widget potencial para arrastrar
					// El arrastre realmente comenzará si el mouse se mueve lo suficiente
					draggedWidget = currentHovered;
				}

				// Establecer foco al hacer mousedown en un widget que puede recibirlo
				if (currentHovered && currentHovered->canFocus) {  
					doFocus ( currentHovered );
				}
				else if (!currentHovered) {
					// Opcionalmente, quitar el foco si se hace clic en ningún widget
					clearFocus ();
				}


				
			}
			else if (action == INPUT::KeyAction::RELEASE) {
				// Crear y despachar evento MouseUp
				Event upEvent {};
				upEvent.type = EventType::MouseUp;
				upEvent.x = x;
				upEvent.y = y;

				if (currentHovered) {
					currentHovered->dispatchEvent ( upEvent );
				}
				else {
					this->dispatchEvent ( upEvent );
				}

				if (button == 0 && action == INPUT::KeyAction::RELEASE && isDragging) {
					// 1. Identificar el widget donde se produce el drop
					Widget* dropTarget = getWidgetAt ( x, y );

					// Evitar drop en el propio widget arrastrado
					if (dropTarget == draggedWidget) {
						dropTarget = nullptr;
					}

					// 2. Evento Drop en el target, si existe
					if (dropTarget) {
						DragEvent dropEvent {};
						dropEvent.type = EventType::Drop;
						dropEvent.x = x;
						dropEvent.y = y;
						dropEvent.dragSource = draggedWidget;

						dropTarget->dispatchEvent ( dropEvent );
					}

					// 3. Evento DragEnd en el widget arrastrado
					DragEvent dragEndEvent {};
					dragEndEvent.type = EventType::DragEnd;
					dragEndEvent.x = x;
					dragEndEvent.y = y;
					dragEndEvent.dragSource = draggedWidget;

					draggedWidget->dispatchEvent ( dragEndEvent );

					// 4. Limpiar estado de arrastre
					isDragging = false;
					draggedWidget = nullptr;
					lastDragOverWidget = nullptr;

					if (draggedWidget && !isDragging) {
						draggedWidget = nullptr;
					}
					return;
				}


				// Manejar clicks y doble clicks
				auto now = std::chrono::steady_clock::now ();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count ();

				if (lastClickWidget == currentHovered && duration < 500) {
					clickCount++;
				}
				else {
					clickCount = 1;
				}

				lastClickTime = now;
				lastClickWidget = currentHovered;

				// Crear eventos Click o DblClick según corresponda
				if (clickCount == 2) {
					Event dblClickEvent {};
					dblClickEvent.type = EventType::DblClick;
					dblClickEvent.x = x;
					dblClickEvent.y = y;

					if (currentHovered) {
						currentHovered->dispatchEvent ( dblClickEvent );
					}

					clickCount = 0;
				}
				else if (lastMouseDown == currentHovered && currentHovered) {
					Event clickEvent {};
					clickEvent.type = EventType::Click;
					clickEvent.x = x;
					clickEvent.y = y;

					currentHovered->dispatchEvent ( clickEvent );
				}

				if (draggedWidget && !isDragging) {
					draggedWidget = nullptr;
				}
				

			}
		}
	}

	void WidgetManager::doFocus ( Widget* widget ) {
		// Si ya tiene el foco, no hacer nada
		if (widget == focusedWidget)
			return;

		// Quitar el foco del widget anterior (si existía)
		if (focusedWidget) {
			FocusEvent blurEvent {};
			blurEvent.type = EventType::Blur;
			blurEvent.bubbles = false;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( blurEvent );
		}

		if (focusedWidget) {
			FocusEvent blurEvent {};
			blurEvent.type = EventType::FocusOut;
			blurEvent.bubbles = true;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( blurEvent );
		}

		// Establecer nuevo widget con foco
		focusedWidget = widget;

		// Si hay un nuevo widget, enviarle evento de Focus
		if (focusedWidget) {
			FocusEvent focusEvent {};
			focusEvent.type = EventType::Focus;
			focusEvent.bubbles = false;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( focusEvent );
		}

		if (focusedWidget) {
			FocusEvent focusEvent {};
			focusEvent.type = EventType::FocusIn;
			focusEvent.bubbles = true;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( focusEvent );
		}
	}

	void WidgetManager::clearFocus () {
		if (focusedWidget) {
			FocusEvent blurEvent {};
			blurEvent.type = EventType::Blur;
			blurEvent.bubbles = false;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( blurEvent );
		}
		if (focusedWidget) {
			FocusEvent blurEvent {};
			blurEvent.type = EventType::FocusOut;
			blurEvent.bubbles = true;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( blurEvent );
		}
		focusedWidget = nullptr;
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


