#include "Linear.h"

namespace SEVIAN::WIDGET {


	Block::Block ( RENDERER::Viewport viewport ) : viewport ( viewport ) { 	}

	void Block::arrange ( Widget* widget ) {
		container = widget;
		//std::cout << "Linear::arrange\n";

		auto layout = container->getStyle<LayoutComponent> ();

		if (!layout) {
			return;
		}
		if (container->id == "ref") {
			std::cout << container->id << "\n";
		}




		auto parentWidth = getParentWidth ( container );
		auto parentHeight = getParentHeight ( container );


		auto pos = getParentPosition ( container );

		auto parentX = pos.x;
		auto parentY = pos.y;

		const auto width = r ( layout->width, parentWidth );
		const auto height = r ( layout->height, parentHeight );

		float left = 0.0f;
		float top = 0.0f;


		if (layout->position != Position::Static) {
			left = r ( layout->left, parentWidth );
			top = r ( layout->top, parentWidth );
		}

		float marginLeft = r ( layout->margin.left, parentWidth );
		float marginRight = r ( layout->margin.right, parentWidth );

		float marginTop = r ( layout->margin.top, parentHeight );
		float marginBottom = r ( layout->margin.bottom, parentHeight );

		float paddingLeft = r ( layout->padding.left, parentWidth );
		float paddingTop = r ( layout->padding.top, parentHeight );
		float paddingRight = r ( layout->padding.right, parentHeight );
		float paddingBottom = r ( layout->padding.bottom, parentHeight );

		float borderLeft = r ( layout->border.left.size, 0.0f );
		float borderTop = r ( layout->border.top.size, 0.0f );
		float borderRight = r ( layout->border.right.size, 0.0f );
		float borderBottom = r ( layout->border.bottom.size, 0.0f );

		float calcWidth = width;
		float calcHeight = height;

		float availableWidth = 0.0;
		float availableHeight = 0.0;

		if (container->id == "root") {
			std::cout << container->id << "\n";
		}
		if (container->id == "text1") {
			std::cout << container->id << "\n";
		}


		if (layout->width == UnitType::Auto) {
			if (isBlock ( layout->display )) {
				auto parentSize = availableSize ( container->parent );
				availableWidth = parentSize.x - marginLeft - marginRight - borderLeft - borderRight - paddingLeft - paddingRight;
			}
			else {
				availableWidth = estimatedWidth ( 0.0f );
			}
		}
		else if (layout->boxSizing == BoxSizing::BorderBox) {
			availableWidth = width - borderLeft - borderRight - paddingLeft - paddingRight;
		}
		else {
			availableWidth = width;
		}

		if (layout->height == UnitType::Auto) {
			availableHeight = estimatedHeight ( availableWidth );
		}
		else if (layout->boxSizing == BoxSizing::BorderBox) {
			availableHeight = height - borderTop - borderBottom - paddingTop - paddingBottom;
		}
		else {
			availableHeight = height;
		}

		arrangeChilds ( marginLeft + left + borderLeft + paddingLeft, marginTop + top + borderTop + paddingTop, availableWidth, availableHeight );

		if (!widget->parent) {
			container->setPosition ( marginLeft + left, marginTop + top );
		}

		if (container->id == "root") {
			std::cout << container->id << "\n";
		}
		if (container->id == "ref") {
			std::cout << container->id << "\n";
		}

		auto w = availableWidth + borderLeft + borderRight + paddingLeft + paddingRight;
		auto h = availableHeight + borderLeft + borderRight + paddingLeft + paddingRight;
		container->setMargin ( { marginTop, marginRight , marginBottom, marginLeft } );
		container->setBorder ( { borderTop, borderRight , borderBottom, borderLeft } );
		container->setPadding ( { paddingTop, paddingRight , paddingBottom, paddingLeft } );
		container->setSize ( w, h );


		if (layout->position == Position::Absolute || layout->position == Position::Fixed) {

			auto parent = getAncestorWithPosition ( container );

			auto position = (parent) ? parent->getPosition () : glm::vec2 { 0.0f, 0.0f };
			container->positioned = true;
			container->setPosition ( position.x + marginLeft + left, position.y + marginTop + top );

		} else if (layout->position == Position::Relative || layout->position == Position::Sticky) {

			

			auto position = getParentPosition ( container );
			container->positioned = true;
			container->setPosition ( position.x + marginLeft + left, position.y + marginTop + top );

		}

	}


	float Block::estimatedWidth ( float width ) {
		if (!container || container->children.empty ()) return 0.0f;

		float x = 0.0f;
		float y = 0.0f;
		float calcWidth = 0.0f;
		float childWidth = 0.0f;
		// calculate of container with
		for (auto& child : container->children) {
			if (!child->isVisible) continue;

			auto childLayout = child->getStyle<LayoutComponent> ();
			
			if (!childLayout) continue;
			
			if (childLayout->position == Position::Absolute) {
				continue;
			}

			auto _width = r ( childLayout->width, width );
			//auto _height = r ( childLayout->height, height );

			if (childLayout->width == UnitType::Auto) {
				_width = child->getSize ().x;
			}

			//float _left = r ( childLayout->left, width );
			//float _top = r ( childLayout->top, height );

			float _marginLeft = r ( childLayout->margin.left, width );
			float _marginRight = r ( childLayout->margin.right, width );
			//float _marginTop = r ( childLayout->margin.top, height );// no va
			//float _marginBottom = r ( childLayout->margin.bottom, height );// no va

			float _paddingLeft = r ( childLayout->padding.left, width );
			//float _paddingTop = r ( childLayout->padding.top, height ); // no va

			float _paddingRight = r ( childLayout->padding.right, width );
			//float _paddingBottom = r ( childLayout->padding.bottom, height );

			float _borderLeft = r ( childLayout->border.left.size, 0.0f );
			//float _borderTop = r ( childLayout->border.top.size, 0.0f );
			float _borderRight = r ( childLayout->border.right.size, 0.0f );
			//float _borderBottom = r ( childLayout->border.bottom.size, 0.0f );



			if (isBlock ( childLayout->display )) {
				x = 0.0f;
			}
			else if (width > 0 && x > width) {
				x = 0.0f;
			}

			if (childLayout->boxSizing == BoxSizing::BorderBox) {
				childWidth = _width + _marginLeft + _marginRight;
			}
			else {
				childWidth = _width + _marginLeft + _marginRight + _paddingLeft + _paddingRight + _borderLeft + _borderRight;
			}

			calcWidth = std::max ( calcWidth, childWidth );

			x += childWidth;
		}
		return calcWidth;
	}

	float Block::estimatedHeight ( float calcWidth ) {
		if (!container || container->children.empty ()) return 0.0f;

		float x = 0.0f;              // Posición horizontal en la línea actual
		float calcHeight = 0.0f;     // Altura total acumulada
		float lineHeight = 0.0f;     // Altura de la línea actual (para inline)
		float lastMarginBottom = 0.0f; // Margen inferior del último elemento block

		for (auto& child : container->children) {
			if (!child->isVisible) continue;

			auto childLayout = child->getStyle<LayoutComponent> ();

			if (!childLayout) continue;

			if (childLayout->position == Position::Absolute) {
				continue; // Ignorar elementos con position: absolute
			}

			// Calcular dimensiones y propiedades del hijo
			float _width = r ( childLayout->width, calcWidth );
			float _height = r ( childLayout->height, 0.0f ); // Altura inicial 0, se ajusta después

			if (childLayout->width == UnitType::Auto) {
				_width = child->getSize ().x;
			}
			if (childLayout->height == UnitType::Auto) {
				_height = child->getSize ().y;
			}

			float _marginLeft = r ( childLayout->margin.left, calcWidth );
			float _marginRight = r ( childLayout->margin.right, calcWidth );
			float _marginTop = r ( childLayout->margin.top, 0.0f ); // No tenemos altura aún
			float _marginBottom = r ( childLayout->margin.bottom, 0.0f );

			float _paddingLeft = r ( childLayout->padding.left, calcWidth );
			float _paddingRight = r ( childLayout->padding.right, calcWidth );
			float _paddingTop = r ( childLayout->padding.top, 0.0f );
			float _paddingBottom = r ( childLayout->padding.bottom, 0.0f );

			float _borderLeft = r ( childLayout->border.left.size, 0.0f );
			float _borderRight = r ( childLayout->border.right.size, 0.0f );
			float _borderTop = r ( childLayout->border.top.size, 0.0f );
			float _borderBottom = r ( childLayout->border.bottom.size, 0.0f );

			// Calcular dimensiones auxiliares según box-sizing
			float auxWidth, auxHeight;
			if (childLayout->boxSizing == BoxSizing::BorderBox) {
				auxWidth = _width + _marginLeft + _marginRight;
				auxHeight = _height;
			}
			else {
				auxWidth = _width + _marginLeft + _marginRight + _paddingLeft + _paddingRight + _borderLeft + _borderRight;
				auxHeight = _height + _paddingTop + _paddingBottom + _borderTop + _borderBottom;
			}

			bool isBlockElement = isBlock ( childLayout->display );

			if (isBlockElement) {
				// Terminar la línea anterior si existe
				if (lineHeight > 0) {
					calcHeight += lineHeight;
					lineHeight = 0.0f;
				}

				// Colapso de márgenes: máximo entre el margen inferior anterior y el superior actual
				float effectiveMarginTop = std::max ( lastMarginBottom, _marginTop );
				calcHeight += effectiveMarginTop;

				// Sumar la altura del elemento (contenido + padding + bordes)
				calcHeight += auxHeight;

				// Actualizar el margen inferior para el próximo elemento block
				lastMarginBottom = _marginBottom;

				x = 0.0f; // Reiniciar posición horizontal
			}
			else {
				// Elementos inline
				if (x + auxWidth > calcWidth) {
					// Saltar a la siguiente línea
					calcHeight += lineHeight;
					x = 0.0f;
					lineHeight = 0.0f;
				}
				x += auxWidth;
				lineHeight = std::max ( lineHeight, auxHeight );
				lastMarginBottom = 0.0f; // Reiniciar margen inferior tras inline
			}
		}

		// Sumar la última línea si hay elementos inline pendientes
		if (lineHeight > 0) {
			calcHeight += lineHeight;
		}
		calcHeight += lastMarginBottom;
		return calcHeight;
	}

	/*
	float Block::estimatedHeight ( float calcWidth ) {
		if (!container || container->children.empty ()) return 0.0f;

		float x = 0.0f;
		float y = 0.0f;
		float height = 0.0f;

		float lineHeight = 0.0f;
		float auxWidth = 0.0f;
		float auxHeight = 0.0f;
		float calcHeight = height;
		for (auto& child : container->children) {

			if (!child->isVisible) continue;

			auto childLayout = child->getStyle<LayoutComponent> ();

			if (childLayout->position == Position::Absolute) {
				continue; // no necesariamente
			}

			auto _width = r ( childLayout->width, calcWidth );
			auto _height = r ( childLayout->height, height );

			if (childLayout->width == UnitType::Auto) {
				_width = child->getSize ().x;
			}
			if (childLayout->height == UnitType::Auto) {
				_height = child->getSize ().y;
			}

			//float _left = r ( childLayout->left, calcWidth );
			//float _top = r ( childLayout->top, height );

			float _marginLeft = r ( childLayout->margin.left, calcWidth );
			float _marginRight = r ( childLayout->margin.right, calcWidth );
			float _marginTop = r ( childLayout->margin.top, height );
			float _marginBottom = r ( childLayout->margin.bottom, height );

			float _paddingLeft = r ( childLayout->padding.left, calcWidth );
			float _paddingTop = r ( childLayout->padding.top, height );

			float _paddingRight = r ( childLayout->padding.right, calcWidth );
			float _paddingBottom = r ( childLayout->padding.bottom, height );

			float _borderLeft = r ( childLayout->border.left.size, 0.0f );
			float _borderTop = r ( childLayout->border.top.size, 0.0f );
			float _borderRight = r ( childLayout->border.right.size, 0.0f );
			float _borderBottom = r ( childLayout->border.bottom.size, 0.0f );





			if (childLayout->boxSizing == BoxSizing::BorderBox) {
				auxWidth = _width + _marginLeft + _marginRight;
				auxHeight = _height + _marginTop + _marginBottom;
			}
			else {
				auxWidth = _width + _marginLeft + _marginRight + _paddingLeft + _paddingRight + _borderLeft + _borderRight;
				auxHeight = _height + _marginTop + _marginBottom + _paddingTop + _paddingBottom + _borderTop + _borderBottom;
			}


			auto newLine = isBlock ( childLayout->display );

			if (newLine || x + auxWidth > calcWidth) {

				if (newLine && lineHeight > 0) {
					calcHeight += lineHeight;
					lineHeight = 0.0f;
				}
				x = 0.0f;
				calcHeight += std::max ( auxHeight, lineHeight );
				lineHeight = 0.0f;
			}
			else {
				x += auxWidth;
				lineHeight = std::max ( auxHeight, lineHeight );
			}





		}


		return calcHeight;
	}
	*/
	
	
	/*
	void Block::arrangeChilds ( float posX, float posY, float calcWidth, float calcHeight ) {
		if (!container || container->children.empty ()) return;
		std::vector<Widget*> lines;

		float lineHeight = 0.0f;

		float x = posX;
		float y = posY;
		auto lastMarginBottom = 0.0f;
		for (auto& child : container->children) {

			if (!child->isVisible) continue;

			auto childLayout = child->getStyle<LayoutComponent> ();

			if (childLayout->position == Position::Absolute) {
				continue; // no necesariamente
			}

			float aWidth = 0.0;
			float aHeight = 0.0;

			auto _width = r ( childLayout->width, calcWidth );
			auto _height = r ( childLayout->height, calcHeight );



			float _left = (childLayout->position != Position::Static)? r ( childLayout->left, calcWidth ) :0.0f;
			float _top = (childLayout->position != Position::Static) ? r ( childLayout->top, calcHeight ): 0.0f;

			

			float _marginLeft = r ( childLayout->margin.left, calcWidth );
			float _marginRight = r ( childLayout->margin.right, calcWidth );
			float _marginTop = r ( childLayout->margin.top, calcHeight );
			float _marginBottom = r ( childLayout->margin.bottom, calcHeight );

			float _paddingLeft = r ( childLayout->padding.left, calcWidth );
			float _paddingTop = r ( childLayout->padding.top, calcHeight );
			float _paddingRight = r ( childLayout->padding.right, calcWidth );
			float _paddingBottom = r ( childLayout->padding.bottom, calcHeight );

			float _borderLeft = r ( childLayout->border.left.size, 0.0f );
			float _borderTop = r ( childLayout->border.top.size, 0.0f );
			float _borderRight = r ( childLayout->border.right.size, 0.0f );
			float _borderBottom = r ( childLayout->border.bottom.size, 0.0f );

			float width = 0.0f;
			float height = 0.0f;



			

			if (child->id == "ref") {
				std::cout << child->id << "\n";
			}
			if (childLayout->width == UnitType::Auto) {
				//width = child->getSize ().x;

				width = 0.0f;
			}
			else {
				if (childLayout->boxSizing == BoxSizing::BorderBox) {
					width = _width;
				}
				else {
					width = _borderLeft + _paddingLeft + _width + _paddingRight + _borderRight;
				}
			}

			if (childLayout->height == UnitType::Auto) {
				height = child->getSize ().y;
			}
			else {
				if (childLayout->boxSizing == BoxSizing::BorderBox) {
					height = _height;
				}
				else {
					height = _borderTop + _paddingTop + _height + _paddingBottom + _borderBottom;
				}
			}



			if (child->id == "stack 04_A") {
				//std::cout << "stack 04_A";
			}
			auto isBlockElement = isBlock ( childLayout->display );


			child->setMargin ( { _marginTop, _marginRight , _marginBottom, _marginLeft } );
			child->setBorder ( { _borderTop, _borderRight , _borderBottom, _borderLeft } );
			child->setPadding ( { _paddingTop, _paddingRight , _paddingBottom, _paddingLeft } );

			if (isBlockElement) {

				if (lineHeight > 0) {
					y += lineHeight;
					lineHeight = 0.0f;
				}

				float marginTopToUse = _marginTop;
				if (lastMarginBottom > 0) {
					marginTopToUse = std::max ( _marginTop, lastMarginBottom );
				}

				//lines.push_back ( child.get() );


				child->setPosition ( posX + _left + _marginLeft, y + _top + marginTopToUse );
				child->setSize ( width, height );

				x = posX;
				y += marginTopToUse + height + _marginBottom;

				continue;

			}

			if (x + width + _marginLeft + _marginRight > calcWidth) {

				child->setPosition ( posX + _left + _marginLeft, y + _top + _marginTop );
				child->setSize ( width, height );

				x = posX;
				y += std::max ( _marginTop + height + _marginBottom, lineHeight );
				lastMarginBottom = 0.0f;
				continue;
			}

			child->setPosition ( x + _left + _marginLeft, y + _top + _marginTop );
			child->setSize ( width, height );
			x += _marginLeft + width + _marginRight;
			lastMarginBottom = 0.0f;
			lineHeight = std::max ( _marginTop + height + _marginBottom, lineHeight );
		}
	}
	*/


void Block::arrangeChilds ( float posX, float posY, float calcWidth, float calcHeight ) {
	if (!container || container->children.empty ()) return;

	float x = posX;
	float y = posY;
	float lineHeight = 0.0f;
	float lastMarginBottom = 0.0f; // Para rastrear el margen inferior del último block

	for (auto& child : container->children) {
		if (!child->isVisible) continue;

		auto childLayout = child->getStyle<LayoutComponent> ();

		if (!childLayout) continue;

		if (childLayout->position == Position::Absolute) {
			continue; // Saltar elementos absolutos
		}

		// Calcular propiedades del hijo
		float _width = r ( childLayout->width, calcWidth );
		float _height = r ( childLayout->height, calcHeight );
		float _marginLeft = r ( childLayout->margin.left, calcWidth );
		float _marginRight = r ( childLayout->margin.right, calcWidth );
		float _marginTop = r ( childLayout->margin.top, calcHeight );
		float _marginBottom = r ( childLayout->margin.bottom, calcHeight );
		float _paddingLeft = r ( childLayout->padding.left, calcWidth );
		float _paddingTop = r ( childLayout->padding.top, calcHeight );
		float _paddingRight = r ( childLayout->padding.right, calcWidth );
		float _paddingBottom = r ( childLayout->padding.bottom, calcHeight );
		float _borderLeft = r ( childLayout->border.left.size, 0.0f );
		float _borderTop = r ( childLayout->border.top.size, 0.0f );
		float _borderRight = r ( childLayout->border.right.size, 0.0f );
		float _borderBottom = r ( childLayout->border.bottom.size, 0.0f );
		float _left = (childLayout->position != Position::Static) ? r ( childLayout->left, calcWidth ) : 0.0f;
		float _top = (childLayout->position != Position::Static) ? r ( childLayout->top, calcHeight ) : 0.0f;

		float width = 0.0f;
		float height = 0.0f;

		if (childLayout->width == UnitType::Auto) {
			width = 0.0f; // Podrías calcularlo basado en el contenido si es necesario
		}
		else {
			if (childLayout->boxSizing == BoxSizing::BorderBox) {
				width = _width;
			}
			else {
				width = _borderLeft + _paddingLeft + _width + _paddingRight + _borderRight;
			}
		}

		if (childLayout->height == UnitType::Auto) {
			height = child->getSize ().y;
		}
		else {
			if (childLayout->boxSizing == BoxSizing::BorderBox) {
				height = _height;
			}
			else {
				height = _borderTop + _paddingTop + _height + _paddingBottom + _borderBottom;
			}
		}

		// Aplicar márgenes, bordes y padding
		child->setMargin ( { _marginTop, _marginRight, _marginBottom, _marginLeft } );
		child->setBorder ( { _borderTop, _borderRight, _borderBottom, _borderLeft } );
		child->setPadding ( { _paddingTop, _paddingRight, _paddingBottom, _paddingLeft } );

		bool isBlockElement = isBlock ( childLayout->display );

		if (isBlockElement) {
			// Colapso de márgenes: usar el máximo entre el margen inferior anterior y el superior actual
			float effectiveMarginTop = std::max ( lastMarginBottom, _marginTop );
			if (lineHeight > 0) {
				y += lineHeight; // Terminar la línea anterior si existe
				lineHeight = 0.0f;
			}
			y += effectiveMarginTop;

			// Posicionar el widget
			child->setPosition ( posX + _left + _marginLeft, y + _top );
			child->setSize ( width, height );

			// Actualizar y para el siguiente elemento
			y += height;
			lastMarginBottom = _marginBottom; // Guardar el margen inferior para el próximo block
			x = posX; // Reiniciar x para el siguiente elemento
		}
		else {
			// Elementos inline
			if (x + width + _marginLeft + _marginRight > calcWidth) {
				// Saltar a la siguiente línea
				y += lineHeight;
				x = posX;
				lineHeight = 0.0f;

				child->setPosition ( posX + _left + _marginLeft, y + _top + _marginTop );
				child->setSize ( width, height );
			}
			else {
				// Posicionar en la línea actual
				child->setPosition ( x + _left + _marginLeft, y + _top + _marginTop );
				child->setSize ( width, height );
				x += _marginLeft + width + _marginRight;
			}

			// Actualizar la altura de la línea
			lineHeight = std::max ( lineHeight, _marginTop + height + _marginBottom );
			lastMarginBottom = 0.0f; // Reiniciar margen inferior si hay inline después de un block
		}
	}

	// Sumar la última lineHeight si hay elementos inline al final
	if (lineHeight > 0) {
		y += lineHeight;
	}
}
	float Block::resolve ( Unit& unit, float parentSize ) {

		return unit.resolve ( parentSize, 24.0f, 16.0f, viewport.width, viewport.height );
	}

	float Block::r ( Unit& unit, float parentSize ) {
		return unit.resolve ( parentSize, 24.0f, 16.0f, viewport.width, viewport.height );
	}


	glm::vec2 Block::getParentSize ( Widget* widget ) {



		if (widget->parent) {
			auto layout = widget->parent->getStyle<LayoutComponent> ();
			if (layout->width != UnitType::Auto) {
				return { layout->width.px (), layout->height.px () };
			}
			else {
				return { getParentWidth ( widget->parent ),  getParentHeight ( widget->parent ) };
			}
		}
		return { viewport.width, viewport.height };
	}

	Widget* Block::getAncestorWithPosition ( Widget* widget ) {
		
		if (!widget) return nullptr;


		// Comenzamos desde el padre del widget
		Widget* current = widget->parent;

		// Recorremos la jerarquía hacia arriba
		while (current != nullptr) {
			auto layout = current->getStyle<LayoutComponent> ();
			if (layout && layout->position != Position::Static) {
				// Encontramos el primer ancestro con position diferente de static
				return current;
			}
			// Si no cumple, subimos al siguiente nivel
			current = current->parent;
		}

		// Si llegamos aquí, no hay ancestros con position != static,
		// así que devolvemos el widget raíz (el último antes de nullptr)
		// En este caso, podrías tener un widget raíz específico; si no, ajusta según tu sistema
		//return getRootWidget ( widget ); // Asumiendo que tienes un método para obtener la raíz
		return nullptr;
	}

	float Block::getParentWidth ( Widget* widget ) {



		if (widget->parent) {
			auto layout = widget->parent->getStyle<LayoutComponent> ();
			if (layout->width != UnitType::Auto) {
				return layout->width.px ();
			}
			else {
				return getParentWidth ( widget->parent );
			}
		}
		return viewport.width;
	}

	float Block::getParentHeight ( Widget* widget ) {
		if (widget->parent) {
			auto layout = widget->getStyle<LayoutComponent> ();
			if (layout->height != UnitType::Auto) {
				return layout->height.px ();
			}
			else {
				return getParentWidth ( widget->parent );
			}
		}
		return viewport.height;
	}

	glm::vec2 Block::getParentPosition ( Widget* widget ) {
		if (widget->parent) {

			return widget->getPosition ();

		}
		return { 0.0f, 0.0f };
	}

	Metrics Block::getMetrics ( Widget* widget ) {
		Metrics m;
		auto layout = container->getStyle<LayoutComponent> ();

		if (!layout) {
			return m;
		}





		m.parentWidth = getParentWidth ( container );
		m.parentHeight = getParentHeight ( container );


		auto pos = getParentPosition ( container );

		m.x = pos.x;
		m.y = pos.y;

		m.width = r ( layout->width, m.parentWidth );
		m.height = r ( layout->height, m.parentHeight );

		float left = r ( layout->left, m.parentWidth );
		float top = r ( layout->top, m.parentHeight );

		m.marginLeft = r ( layout->margin.left, m.parentWidth );
		m.marginRight = r ( layout->margin.right, m.parentWidth );

		m.marginTop = r ( layout->margin.top, m.parentHeight );
		m.marginBottom = r ( layout->margin.bottom, m.parentHeight );

		m.paddingLeft = r ( layout->padding.left, m.parentWidth );
		m.paddingTop = r ( layout->padding.top, m.parentHeight );
		m.paddingRight = r ( layout->padding.right, m.parentHeight );
		m.paddingBottom = r ( layout->padding.bottom, m.parentHeight );

		m.borderLeft = r ( layout->border.left.size, 0.0f );
		m.borderTop = r ( layout->border.top.size, 0.0f );
		m.borderRight = r ( layout->border.right.size, 0.0f );
		m.borderBottom = r ( layout->border.bottom.size, 0.0f );

		if (layout->boxSizing == BoxSizing::BorderBox) {
			m.width = m.width;
		}
		else {
			m.width = m.borderLeft + m.paddingLeft + m.width + m.paddingRight + m.borderRight;
			m.height = m.borderTop + m.paddingTop + m.height + m.paddingBottom + m.borderBottom;
		}

		return m;
	}

	glm::vec2 Block::availableSize ( Widget* widget ) {

		if (!widget) {
			return { viewport.width, viewport.height };
		}


		auto size = widget->getSize ();
		auto margin = widget->getMargin ();
		auto border = widget->getBorder ();
		auto padding = widget->getPadding ();

		return { size.x - border.left - border.right - padding.left - padding.right, size.y - border.top - border.bottom - padding.top - padding.bottom };
		


	}

	Widget* Block::getRootWidget ( Widget* widget ) {
		while (widget && widget->parent) {
			widget = widget->parent;
		}
		return widget;
	}

}

