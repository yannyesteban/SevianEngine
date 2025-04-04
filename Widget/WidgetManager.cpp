#include "WidgetManager.h"
#include <algorithm>
#include "Linear.h"
#include "StackingContext.h"

namespace SEVIAN::WIDGET {

	void WidgetManager::render ( std::shared_ptr<RENDERER::RenderInterface> render, Camera2D camera ) {


		auto newNode = std::make_unique<StackingContextNode> ( this );
		newNode->createsStackingContext = true;

		//collectContextStack ( this, newNode.get () );

		// Recopilar la jerarquía de stacking contexts
		for (auto& child : this->children) {
			collectContextStack ( child.get (), newNode.get () );
		}

		renderStackingContext ( newNode.get (), camera );

		//collectWidgets ( this );
		//for (auto& widget : allWidgets) {
		//	//std::cout << "Render stage: " << this->id << "\n";
		//	if (!widget->positioned) {
		//		auto o = widget->getRenderObject ( camera );

		//		render->draw ( o, camera );
		//	}


		//}

		//for (auto& widget : allWidgets) {
		//	//std::cout << "Render stage: " << this->id << "\n";
		//	if (widget->positioned) {
		//		auto o = widget->getRenderObject ( camera );

		//		//render->draw ( o, camera );
		//	}


		//}
	}


	void WidgetManager::collectContextStack ( Widget* widget, StackingContextNode* parentContext ) {
		
		auto layout = widget->getStyle<LayoutComponent> ();
		

		auto newNode = StackingContextNode::newNode ( widget );
		
		// Guardamos el puntero al nodo actual antes de moverlo
		StackingContextNode* currentContext = newNode.get ();

		// Añadimos el nodo al padre
		parentContext->children.push_back ( std::move ( newNode ) );

		// 3. Procesar los hijos
		for (auto& child : widget->children) {
			if (currentContext->createsStackingContext) {
				// Si este widget crea un nuevo contexto, sus hijos pertenecen a este nuevo contexto
				collectContextStack ( child.get (), currentContext );
			}
			else {
				// Si no crea un nuevo contexto, los hijos pertenecen al mismo contexto que el padre
				collectContextStack ( child.get (), parentContext );
			}
		}
	}

	void WidgetManager::renderStackingContext ( const StackingContextNode* node, Camera2D camera ) {
		if (node->widget) {
			renderWidget ( node->widget, camera ); // Implementa esto según tus capas
		}
		

		std::vector<StackingContextNode*> negativeZChildren;
		for (const auto& child : node->children) {
			if (child->layer == LayerType::NegativeZIndex) {
				negativeZChildren.push_back ( child.get () );
			}
		}
		std::sort ( negativeZChildren.begin (), negativeZChildren.end (),
			[]( const StackingContextNode* a, const StackingContextNode* b ) {
				return a->zIndex.value < b->zIndex.value; // Orden ascendente
			} );

		for (const auto* child : negativeZChildren) {
			if (child->createsStackingContext) {
				renderStackingContext ( child, camera ); // Renderizar subárbol completo
			}
			else {
				renderWidget ( child->widget, camera ); // Renderizar widget directamente
			}
		}

		// Paso 2: Renderizar hijos estáticos o en el mismo contexto
		for (const auto& child : node->children) {
			if (child->layer == LayerType::Content) {
				renderStackingContext ( child.get(), camera);
				//renderWidget ( child->widget, camera ); // Orden de aparición
			}
		}

		// Paso 3: Renderizar hijos con z-index 0 o auto (que crean contexto)
		std::vector<StackingContextNode*> zeroZChildren;
		for (const auto& child : node->children) {
			if (child->layer == LayerType::ZeroZIndex) {
				zeroZChildren.push_back ( child.get () );
			}
		}
		// No ordenamos aquí, respetamos el orden de aparición según CSS
		for (const auto* child : zeroZChildren) {
			renderStackingContext ( child, camera );
			if (child->createsStackingContext) {
				//renderStackingContext ( child, camera );
			}
			// Los que no crean contexto ya se renderizaron en el paso 2
		}

		// Paso 4: Renderizar hijos con z-index positivo
		std::vector<StackingContextNode*> positiveZChildren;
		for (const auto& child : node->children) {
			if (child->layer == LayerType::PositiveZIndex) {
				positiveZChildren.push_back ( child.get () );
			}
		}
		std::sort ( positiveZChildren.begin (), positiveZChildren.end (),
			[]( const StackingContextNode* a, const StackingContextNode* b ) {
				return a->zIndex.value < b->zIndex.value; // Orden ascendente
			} );
		for (const auto* child : positiveZChildren) {
			renderStackingContext ( child, camera );
			/*if (child->createsStackingContext) {
				renderStackingContext ( child, camera );
			}
			else {
				renderWidget ( child->widget, camera );
			}*/
		}

	}

	void WidgetManager::renderWidget ( Widget* widget, Camera2D camera ) {
		auto o = widget->getRenderObject ( camera );

		renderer->draw ( o, camera );
	}

	WidgetManager::WidgetManager ( std::shared_ptr<RENDERER::RenderInterface> renderer, std::shared_ptr<INPUT::IInputManager> inputManager ) :
		renderer ( renderer ), inputManager ( inputManager ), widgetRenderer ( renderer->getManager<RENDERER::iWidgetManager> () ) {

		setLayout ( std::make_unique<Block> ( renderer->getViewport () ) );

		SEVIAN::SpriteInfo spriteInfo { "escultura", {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.9f, 0.1f, 2.0f} };
		object = widgetRenderer->createSprite ( spriteInfo );

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
		
			FocusEvent FocusOutEvent {};
			FocusOutEvent.type = EventType::FocusOut;
			FocusOutEvent.bubbles = true;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( FocusOutEvent );
		}

		// Establecer nuevo widget con foco
		focusedWidget = widget;

		// Si hay un nuevo widget, enviarle evento de Focus
		if (focusedWidget) {
			FocusEvent focusEvent {};
			focusEvent.type = EventType::Focus;
			focusEvent.bubbles = false;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( focusEvent );
		
			FocusEvent focusInEvent {};
			focusInEvent.type = EventType::FocusIn;
			focusInEvent.bubbles = true;  // Configura según tus necesidades
			focusedWidget->dispatchEvent ( focusInEvent );
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

	glm::mat4 WidgetManager::getModelMatrix () {
		//std::cout << "Stack::update: " << this->id << "\n";

		layout->arrange ( this );

		//auto o = getRenderObject();


		glm::vec4 backGroundColor = { 1.0f, 0.3f, 0.0f, 1.0f };
		glm::vec4 borderColor = { 0.0f, 0.1f, 0.1f, 1.0f };
		float borderWith = 0.0f;
		float borderRadius = 0.0f;
		int useTexture = 0;

		auto* background = this->getStyle<BackgroundComponent> ();

		if (background) {
			//color = glm::vec4 ( background->backGroundColor, 1.0f );
			//object->addData ( RENDERER::DataResource::STYLE, (void *)&background->backGroundColor.toVec4() );
			backGroundColor = background->backGroundColor.toVec4 ();




		}
		auto* style = this->getStyle<LayoutComponent> ();

		if (style) {
			borderColor = style->border.color.toVec4 ();
			borderWith = style->border.size.px ();
			borderRadius = style->border.radius.px ();

		}


		if (id == "stack 05") {
			//
			std::cout << "";
		}

		styleUbo = {
			size,
			backGroundColor,
			borderColor,
			borderWith,
			borderRadius,
			0

		};

		object->addData ( RENDERER::DataResource::STYLE, &styleUbo );

		auto translation = glm::mat4 ( 1.0f );
		glm::mat4 rotationMatrix = glm::mat4 ( 1.0f );

		auto scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );

		//PositionComponent* position = this->getStyle<PositionComponent> ();
		translation = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( position.x, position.y, 1.0f ) );
		scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3 ( size.x, size.y, 1.0f ) );




		auto modelMatrix = translation * rotationMatrix * scaleMatrix;
		return modelMatrix;
	}
	
}


