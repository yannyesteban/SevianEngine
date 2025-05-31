#pragma once

#include "Widget.h"
#include "Layout.h"



#include <vector>
#include <algorithm>


namespace SEVIAN::WIDGET {

	struct Metrics
	{
		float x;
		float y;

		float width;
		float height;

		float marginLeft;
		float marginTop;
		float marginRight;
		float marginBottom;

		float borderLeft;
		float borderTop;
		float borderRight;
		float borderBottom;

		float paddingLeft;
		float paddingTop;
		float paddingRight;
		float paddingBottom;

		float parentWidth;
		float parentHeight;

	};

	class Block :
		public Layout
	{
	private:
		RENDERER::Viewport viewport;
		Widget* container = nullptr;
		float estimatedWidth ( float width );
		float estimatedHeight ( float width );
		void arrangeChilds ( float posX, float posY, float width, float height );
		glm::vec2 getParentSize ( Widget* widget );
		Widget* getAncestorWithPosition ( Widget* widget );

	public:
		// Heredado vía Layout
		Block ( RENDERER::Viewport );
		float resolve ( Unit& unit, float parentSize );
		void arrange ( Widget* widget ) override;

		float r ( Unit& unit, float parentSize );



		float getParentWidth ( Widget* widget );
		float getParentHeight ( Widget* widget );
		glm::vec2 getParentPosition ( Widget* widget );

		Metrics getMetrics ( Widget* widget );

		glm::vec2 availableSize ( Widget* widget );
		Widget* getRootWidget ( Widget* widget );
	};
}
/*
namespace SEVIAN::WIDGET {
	void Linear::arrange ( Widget* container ) {
		if (!container || container->children.empty ()) return;
		const auto& layout = container->style.layout.linear;
		const bool isRow = layout.direction == Direction::Row || layout.direction == Direction::RowReverse;
		const bool isReverse = layout.direction == Direction::RowReverse || layout.direction == Direction::ColumnReverse;
		float currentMain = 0.0f;
		float currentCross = 0.0f;
		for (auto& child : container->children) {
			if (!child->isVisible) continue;
			const auto& childStyle = child->style.layout.linearItem;
			const glm::vec2 childSize = child->getContentSize ();
			glm::vec2 position;
			if (isRow) {
				position.x = isReverse ? container->size.x - currentMain - childSize.x : currentMain;
				position.y = currentCross;
			}
			else {
				position.x = currentCross;
				position.y = isReverse ? container->size.y - currentMain - childSize.y : currentMain;
			}
			child->position = position;
			currentMain += (isRow ? childSize.x : childSize.y) + layout.gap;
		}
	}
}
*/
