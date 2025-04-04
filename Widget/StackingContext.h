#pragma once
#include "Widget.h"
#include "Stacking.h"

namespace SEVIAN {
	namespace WIDGET {
		enum class StackingContextType
		{
			Root,           // Contexto raíz (similar al body en HTML)
			Static,

			Positioned,     // Elementos con position != static
			Transformed,    // Elementos con transform
			Opacity,        // Elementos con opacity < 1
			Blend,          // Elementos con mix-blend-mode
			Filter,         // Elementos con filtros aplicados
			Custom          // Para contextos específicos del motor de juego
		};

		enum class LayerType
		{
			Background,
			NegativeZIndex,
			Content,
			ZeroZIndex,
			PositiveZIndex,
			Foreground
		};
		struct StackingContextNode
		{
			Widget* widget;                   // Elemento del juego
			StackingContextType type = StackingContextType::Root;         // Tipo de contexto
			bool createsStackingContext = false;      // ¿Crea un nuevo contexto?
			ZIndex zIndex;                    // Valor de z-index
			
			LayerType layer = LayerType::Content;                  // Capa de renderizado
			std::vector<std::unique_ptr<StackingContextNode>> children; // Hijos del nodo

			StackingContextNode ( Widget* w )
				: widget ( w ), createsStackingContext ( false ), zIndex () {
			}
			static std::unique_ptr<StackingContextNode> newNode ( Widget* widget ) {

				auto node = std::make_unique<StackingContextNode> ( widget );
				auto layout = widget->getStyle<LayoutComponent> ();

				if (!layout) {
					return node;
				}

				if (layout->position == Position::Fixed || layout->position == Position::Sticky) {
					node->createsStackingContext = true;
				}

				if (!layout->zIndex.isAuto && (layout->position == Position::Absolute || layout->position == Position::Relative) ){

					node->createsStackingContext = true;
				}

				if (layout->position != Position::Static) {

					if (node->zIndex.value < 0) {
						node->layer = LayerType::NegativeZIndex;
					}
					else if (node->zIndex.value > 0) {
						node->layer = LayerType::PositiveZIndex;
					}
					else {
						node->layer = LayerType::ZeroZIndex;
					}

					
					
						
				}

				if (!widget->parent) {
					node->layer = LayerType::Background;
				}
				node->zIndex = layout ? layout->zIndex : 0; // z-index por defecto 0 si no está definido
				

				return node;
			}
			static bool isCreatesStackingContext ( Widget* widget ) {
				auto layout = widget->getStyle<LayoutComponent> ();

				if (layout->position == Position::Fixed || layout->position == Position::Sticky) {
					return true;
				}

				if (layout->position == Position::Absolute || layout->position == Position::Relative) {
					if (layout->zIndex.isAuto) {
						return false;
					}
					return true;
				}


				return false;

				/*return layout->position != Position::Static ||
					layout->transform != Transform::None ||
					layout->opacity < 1.0f ||
					layout->blendMode != BlendMode::Normal ||
					layout->filter != Filter::None;*/
			};

			static StackingContextType determineContextType ( Widget* widget ) {
				auto layout = widget->getStyle<LayoutComponent> ();
				if (layout->position == Position::Fixed || layout->position == Position::Sticky) return StackingContextType::Positioned;

				return StackingContextType::Static;
				/*if (layout->opacity < 1.0f) return StackingContextType::Opacity;
				if (layout->transform != Transform::None) return StackingContextType::Transformed;
				if (layout->position != Position::Static) return StackingContextType::Positioned;
				return StackingContextType::Root;*/
			}
		};


		class StackingContext
		{
			Widget* root = nullptr;
			 // Widgets que pertenecen a este contexto
			
			ZIndex zIndex;

		public:
			std::vector<Widget*> elements;
			std::vector<std::unique_ptr<StackingContext>> children;
			StackingContext ( Widget* root ) : root ( root ) { }
		};

	}
}