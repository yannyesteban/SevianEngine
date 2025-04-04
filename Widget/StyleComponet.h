#pragma once
#include <glm/glm.hpp>
#include <optional>
#include "Style.h"
#include "UNIT.h"
#include "Color.h"
#include "array"
namespace SEVIAN::WIDGET {


	using namespace UNIT;
	class StyleComponent
	{
	public:

		~StyleComponent () = default;
		//virtual void init(Widget* widget) = 0;
		//virtual void apply(Widget* widget) = 0; // Aplica las propiedades al widget
		//virtual void update ( float deltaTime ) = 0;
	};


	struct ZIndex
	{
		int value = 0;
		bool isAuto = true;

		ZIndex () : value ( 0 ), isAuto ( true ) { }
		ZIndex ( int val ) : value ( val ), isAuto ( false ) { }

		static ZIndex AUTO () {
			return ZIndex ();
		}

		static ZIndex set ( int val ) {
			return ZIndex ( val );
		}
		bool operator<( const ZIndex& other ) const { return value < other.value; }
		bool operator>( const ZIndex& other ) const { return value > other.value; }
		bool operator==( const ZIndex& other ) const { return isAuto == other.isAuto && value == other.value; }

	};
	struct Margin
	{
		Unit top = Unit::px ( 0.0f );
		Unit right = Unit::px ( 0.0f );
		Unit bottom = Unit::px ( 0.0f );
		Unit left = Unit::px ( 0.0f );

		glm::vec2 getTotal () const {
			return { left + right, top + bottom };
		}

		static Margin px ( float value ) {
			return { Unit::px ( value ), Unit::px ( value ), Unit::px ( value ), Unit::px ( value ) };
		}
		static Margin px ( float top, float right, float bottom, float left ) {
			return { Unit::px ( top ), Unit::px ( right ), Unit::px ( bottom ), Unit::px ( left ) };
		}
		static Margin AUTO () {
			return { Unit::AUTO (), Unit::AUTO (), Unit::AUTO (), Unit::AUTO () };
		}
	};

	struct Padding
	{
		Unit top = Unit::px ( 0.0f );
		Unit right = Unit::px ( 0.0f );
		Unit bottom = Unit::px ( 0.0f );
		Unit left = Unit::px ( 0.0f );

		glm::vec2 getTotal () const {
			return { left + right, top + bottom };
		}

		static Padding px ( float value ) {
			return { Unit::px ( value ), Unit::px ( value ), Unit::px ( value ), Unit::px ( value ) };
		}
		static Padding AUTO () {
			return { Unit::AUTO (), Unit::AUTO (), Unit::AUTO (), Unit::AUTO () };
		}
	};

	struct BorderData
	{
		Unit size = Unit::px ( 0.0f );
		BorderStyle style = BorderStyle::Solid;
		COLOR::Color color = Colors::Black;



	};

	struct Border
	{
		BorderData top = { Unit::px ( 0.0f ), BorderStyle::None, Colors::Black };
		BorderData right = { Unit::px ( 0.0f ), BorderStyle::None, Colors::Black };
		BorderData bottom = { Unit::px ( 0.0f ), BorderStyle::None, Colors::Black };
		BorderData left = { Unit::px ( 0.0f ), BorderStyle::None, Colors::Black };

		Unit size = Unit::px ( 0.0f );
		BorderStyle style = BorderStyle::Solid;
		COLOR::Color color = Colors::Black;
		Unit radius = Unit::px ( 0.0f );
		std::array<Unit, 4> borderRadius {  // Inicialización directa
			Unit::px ( 0.0f ),
			Unit::px ( 0.0f ),
			Unit::px ( 0.0f ),
			Unit::px ( 0.0f )
		};

		Border ( Unit width, BorderStyle borderStyle, COLOR::Color borderColor, Unit radius = Unit::px ( 0.0f ) ) :
			size ( width ), style ( borderStyle ), color ( borderColor ), radius ( radius ),
			top { width, borderStyle, borderColor },
			right { width, borderStyle, borderColor },
			bottom { width, borderStyle, borderColor },
			left { width, borderStyle, borderColor },
			borderRadius { radius, radius, radius, radius } {
		}

		Border ( float width, BorderStyle style, COLOR::Color color, float radius = 0.0f )
			: Border ( Unit::px ( width ), style, color, Unit::px ( radius ) ) {
		}
		void setAllBorders ( Unit width, BorderStyle style, COLOR::Color color ) {
			top = right = bottom = left = { width, style, color };
		}
		void setRadius ( float all ) {
			borderRadius.fill ( Unit::px ( all ) );
		}
		void setRadius ( Unit tl, Unit tr, Unit br, Unit bl ) {
			borderRadius = { tl, tr, br, bl };
		}
	};

	// Estructura principal de Layout



	struct PositionComponent : public StyleComponent
	{
		Position position = Position::Static;


		float top = 0.0f;
		float left = 0.0f;
		float right = 0.0f;
		float bottom = 0.0f;
		int zIndex = 0;

		glm::vec2 topLeft = { 0.0f, 0.0f }; // Para posicionamiento absoluto
	};

	struct BackgroundComponent : public StyleComponent
	{
		COLOR::Color backGroundColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float borderRadius = 0.0f;

	};
	struct TextComponent : public StyleComponent
	{
		std::optional <COLOR::Color> color = COLOR::Color { 0.0f, 0.0f, 0.0f, 1.0f };
		std::optional <Unit> fontSize = Unit::px ( 16.0f );
		std::optional <std::string> fontFamily = "Arial";
		std::optional <TextAlign> textAlign = TextAlign::Left;
		std::optional <VerticalAlign> verticalAlign = VerticalAlign::Baseline;
		std::optional <Unit >lineHeight = Unit::NORMAL ();
		std::optional <FontWeight >fontWeight = FontWeight::Normal;
		// Métodos helper para configuración fluida
		TextComponent& setFontSize ( Unit size ) {
			fontSize = size;
			return *this;
		}
		TextComponent& setFontFamily ( const std::string& family ) {
			fontFamily = family;
			return *this;
		}

		TextComponent& setTextColor ( const COLOR::Color& _color ) {
			color = _color;
			return *this;
		}

		TextComponent& setTextAlign ( TextAlign align ) {
			textAlign = align;
			return *this;
		}
		TextComponent& setVerticalAlign ( VerticalAlign align ) {
			verticalAlign = align;
			return *this;
		}
		TextComponent& setBold ( bool value = true ) {
			fontWeight = value ? FontWeight::Bold : FontWeight::Normal;
			return *this;
		}
		TextComponent& setLineHeight ( Unit height ) {
			lineHeight = height;
			return *this;
		}
	};

	struct FlexComponent : public StyleComponent
	{
		FlexDirection flexDirection = FlexDirection::Row;
		FlexWrap flexWrap = FlexWrap::Nowrap;
		JustifyContent justifyContent = JustifyContent::FlexStart;
		AlignItems alignItems = AlignItems::Stretch;
		AlignContent alignContent = AlignContent::Stretch;
		glm::vec2 gap = { 0.0f, 0.0f }; // row-gap, column-gap
	};
	struct FlexItemComponent : public StyleComponent
	{
		float flexGrow = 0.0f;
		float flexShrink = 1.0f;
		float flexBasis = 0.0f;
	};
	struct GridComponent : public StyleComponent
	{
		std::vector<TrackSize> columns;
		std::vector<TrackSize> rows;
		glm::vec2 gap = { 0.0f, 0.0f };
		JustifyContent justifyItems = JustifyContent::Stretch;
		AlignItems alignItems = AlignItems::Stretch;
	};

	struct GridItemComponent : public StyleComponent
	{
		int columnStart = 0;
		int columnEnd = 1;
		int rowStart = 0;
		int rowEnd = 1;
		std::optional<int> justifySelf; // nullopt = inherit
		std::optional<int> alignSelf;
	};

	struct BorderComponent : public StyleComponent
	{
		float width = 0.0f;
		COLOR::Color color = { 0.0f, 0.0f, 0.0f, 1.0f };
		float radius = 0.0f;
	};
	struct LayoutComponent : public StyleComponent
	{

		Unit top = Unit::AUTO ();
		Unit right = Unit::AUTO ();
		Unit bottom = Unit::AUTO ();
		Unit left = Unit::AUTO ();
		ZIndex zIndex;

		BoxSizing boxSizing = BoxSizing::ContentBox;
		Display display = Display::Block;
		Unit width = Unit::AUTO ();
		Unit height = Unit::AUTO ();


		glm::vec2 topLeft = { 0.0f, 0.0f }; // Para posicionamiento absoluto

		// Flex
		FlexLayout flex;
		FlexItemProperties flexItem;

		// Grid
		GridLayout grid;
		GridItemPlacement gridItem;

		// Espaciado
		Margin margin = Margin::px ( 0.0f ); // top, right, bottom, left
		Padding padding = Padding::px ( 0.0f );
		Border border = Border ( 0.0f, BorderStyle::Solid, Colors::Black );

		// Z-index y overflow
		//int zIndex = 0;
		bool overflowHidden = false;

		Position position = Position::Static;
	};

	struct LayoutComponent1 : public StyleComponent
	{
		~LayoutComponent1 () = default;
	public:
		Display display = Display::Block;
		Position position = Position::Static;
		glm::vec2 topLeft = { 0.0f, 0.0f }; // Para posicionamiento absoluto
		// Flex
		FlexLayout flex;
		FlexItemProperties flexItem;
		// Grid
		GridLayout grid;
		GridItemPlacement gridItem;
		// Espaciado
		glm::vec4 margin = { 0.0f, 0.0f, 0.0f, 0.0f }; // top, right, bottom, left
		glm::vec4 padding = { 0.0f, 0.0f, 0.0f, 0.0f };
		// Z-index y overflow
		int zIndex = 0;
		bool overflowHidden = false;
	};

}