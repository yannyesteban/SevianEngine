#pragma once
#include <glm/glm.hpp> 
#include <optional>
#include <string>
#include <unordered_map>


namespace SEVIAN::WIDGET {
	// Enums avanzados tipo CSS
	enum class Display { None, Contents, Block, Inline, Flex, Grid, InlineBlock, InlineFlex, InlineGrid };
	enum class Position { Static, Relative, Absolute, Fixed, Sticky };
	enum class FlexDirection { Row, RowReverse, Column, ColumnReverse };
	enum class FlexWrap { Nowrap, Wrap, WrapReverse };
	enum class JustifyContent
	{
		FlexStart, FlexEnd, Center, SpaceBetween, SpaceAround, SpaceEvenly, Stretch
	};
	enum class AlignItems { Stretch, FlexStart, FlexEnd, Center, Baseline };
	enum class AlignContent { Stretch, FlexStart, FlexEnd, Center, SpaceBetween, SpaceAround };
	enum class GridTrackType { Fixed, Percent, Fraction, Auto, MinContent, MaxContent };

	// Enums tipo CSS (mejor que usar ints)
	enum class TextAlign { Left, Center, Right, Justify };
	enum class VerticalAlign { Baseline, Top, Middle, Bottom, Sub, Super, TextTop, TextBottom };
	enum class FontWeight
	{
		Normal,
		Bold,
		Light,
		SemiBold
	};
	enum class TextDecoration
	{
		None,
		Underline,
		LineThrough,
		Overline
	};
	enum class TextTransform
	{
		None,
		Capitalize,
		Uppercase,
		Lowercase
	};

	enum class BoxSizing { ContentBox, BorderBox };

	enum class BorderStyle { None, Solid, Dotted, Dashed, Double, Groove, Ridge, Inset, Outset };


	// Función auxiliar para verificar si el Display es Inline
	inline bool isInline ( Display display ) {
		return display == Display::Inline || display == Display::InlineBlock
			|| display == Display::InlineFlex || display == Display::InlineBlock;
	}
	// Puedes agregar más funciones auxiliares según sea necesario
	inline bool isBlock ( Display display ) {
		return display == Display::Block || display == Display::Flex
			|| display == Display::Grid;
	}
	// Estructura para tamaños en grid (ej: 1fr, 100px, 30%)
	struct TrackSize
	{
		GridTrackType type = GridTrackType::Auto;
		float value = 0.0f;

		static TrackSize Fixed ( float val ) { return { GridTrackType::Fixed, val }; }
		static TrackSize Percent ( float val ) { return { GridTrackType::Percent, val }; }
		static TrackSize Fraction ( float val ) { return { GridTrackType::Fraction, val }; }
		static TrackSize Auto () { return { GridTrackType::Auto, 0.0f }; }
	};

	// Subestructura específica para Flexbox
	struct FlexLayout
	{
		FlexDirection direction = FlexDirection::Row;
		FlexWrap wrap = FlexWrap::Nowrap;
		JustifyContent justify = JustifyContent::FlexStart;
		AlignItems align = AlignItems::Stretch;
		AlignContent alignContent = AlignContent::Stretch;
		glm::vec2 gap = { 0.0f, 0.0f }; // row-gap, column-gap
	};

	// Subestructura específica para Grid
	struct GridLayout
	{
		std::vector<TrackSize> columns;
		std::vector<TrackSize> rows;
		glm::vec2 gap = { 0.0f, 0.0f };
		JustifyContent justifyItems = JustifyContent::Stretch;
		AlignItems alignItems = AlignItems::Stretch;
	};

	// Propiedades de ítem para Grid
	struct GridItemPlacement
	{
		int columnStart = 0;
		int columnEnd = 1;
		int rowStart = 0;
		int rowEnd = 1;
		std::optional<int> justifySelf; // nullopt = inherit
		std::optional<int> alignSelf;
	};

	// Propiedades de ítem para Flex
	struct FlexItemProperties
	{
		float grow = 0.0f;
		float shrink = 1.0f;
		TrackSize basis = TrackSize::Auto ();
		std::optional<int> order;
	};





	struct OLDBorder
	{
		float width = 0;
		glm::vec4 color = { 0,0,0,1 };
		float radius = 0;
		// ... Estilos de borde
	};

	struct TextStyle
	{
		glm::vec3 color = { 0,0,0 };
		std::string font = "Arial";
		float size = 16.0f;
		TextAlign align = TextAlign::Left;
		// ... Sombras, espaciado, etc.
	};

	// Componente principal de estilo (agrupa todas las subestructuras)
	struct StyleComponent1
	{
		//LayoutComponent layout;
		//Background background;
		OLDBorder border;
		TextStyle text;
		// ... Agregar más categorías (Transiciones, Animaciones, etc.)

		// Método para heredar valores no definidos de un estilo padre
		void inheritFrom ( const StyleComponent1& parent ) {
			//if (!layout.margin_top) layout.margin_top = parent.layout.margin_top;
			if (!text.font.empty ()) text.font = parent.text.font;
			// ... Aplicar lógica de cascada CSS
		}
	};

}



// ... Agregar más enums según necesites

struct Style
{
	glm::vec3 backGroundColor;
	glm::vec3 color;
	float border;
	glm::vec3 borderColor;
	int borderStyle;
	float borderWidth;
	float borderRadius;

	float margin;
	float padding;
	float textShadow;
	float boxShadow;
	float zIndex;

	int textAlign;
	int verticalAlign;
	int display;
	int position;
	int overflow;


};

// Subestructuras organizadas por categorías CSS





// Componente para transiciones de estilo (CSS transition)
struct StyleTransition
{
	struct PropertyTransition
	{
		float duration = 0.3f;
		float delay = 0.0f;
		std::string easing = "ease-in-out";
		bool active = false;
		float progress = 0.0f;
	};

	std::unordered_map<std::string, PropertyTransition> properties;
	// Ej: properties["background.color"], properties["layout.top_left.x"]
};

// Sistema que actualiza transiciones
class StyleTransitionSystem
{
public:
	//void update ( float deltaTime ) {
	//	for (auto& [entity, transition] : view<StyleTransition> ()) {
	//		auto& style = entity.get<StyleComponent> ();

	//		for (auto& [prop, data] : transition.properties) {
	//			if (data.active) {
	//				// Interpolar propiedad según progreso
	//				// Ej: style.background.color = lerp(...)
	//			}
	//		}
	//	}
	//}
};

struct ComputedStyles
{
	glm::vec2 absolute_position; // Calculado desde top/left/parent
	glm::vec2 size;
	// ... demás propiedades resueltas
};

class StyleComputationSystem
{
public:
	void update () {
		//for (auto& widget : widgets) {
		//	ComputedStyles computed;

		//	// Ejemplo: Calcular posición absoluta
		//	if (widget.has<LayoutComponent> ()) {
		//		auto& layout = widget.get<LayoutComponent> ();
		//		computed.absolute_position = calculateAbsolutePosition ( layout, widget.parent );
		//	}

		//	widget.computed_styles = computed;
		//}
	}
};