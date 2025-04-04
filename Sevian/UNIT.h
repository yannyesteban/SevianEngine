#pragma once

#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <stdexcept>

namespace SEVIAN {
    /*
    8. Optimizaciones

    Caching: Almacenar tamaños calculados si el contexto no cambia.

    Dirty Flags: Recalcular solo cuando las propiedades cambian.

    Batch Updates: Procesar múltiples elementos en paralelo.
    */
	namespace UNIT {

        static float parentSize = 0.0f;
        static float fontSize = 0.0f;
        static float rootFontSize = 0.0f;
        static float viewportWidth = 0.0f;
        static float viewportHeight = 0.0f;
        enum class UnitType
        {
            Pixel,      // Unidad absoluta (px)
            Percent,    // Porcentaje relativo al contenedor/padre (%)
            Em,         // Relativo al tamaño de fuente del contexto (ej: 1em = 16px)
            Rem,        // Relativo al tamaño de fuente raíz (root em)
            Vw,         // 1% del ancho del viewport
            Vh,         // 1% del alto del viewport
            Auto,        // Valor calculado automáticamente (ej: centrado)
            Viewport,   // vw/vh - Porcentaje del viewport idea of claude 
            //Fraction    // fr - Fracción del espacio disponible (como en Grid) idea of claude 


            Fraction, // Agrega un tipo de unidad fr (fracción) para Grid:
            Calc,
			Normal,
        };
        struct Unit
        {
            float value;      // Valor numérico (ej: 50.0f)
            UnitType type;    // Tipo de unidad (ej: UnitType::Percent)
            // Opcional: operadores para combinar unidades (ej: 50% + 20px)


             // Métodos estáticos de fabricación (factory methods)
            static Unit px ( float val ) { return { val, UnitType::Pixel }; }
            static Unit pc ( float val ) { return { val, UnitType::Percent }; }
            static Unit em ( float val ) { return { val, UnitType::Em }; }
            static Unit rem ( float val ) { return { val, UnitType::Rem }; }
            static Unit vw ( float val ) { return { val, UnitType::Vw }; }
            static Unit vh ( float val ) { return { val, UnitType::Vh }; }
            static Unit fr ( float val ) { return { val, UnitType::Fraction }; }
            static Unit Calc ( ) { return { 0.0f, UnitType::Calc }; }
            static Unit AUTO ( ) { return { 0.0f, UnitType::Auto }; }
            static Unit NORMAL () { return { 0.0f, UnitType::Normal }; }

            bool isRelative () {
                switch (type) {
                case UnitType::Pixel:
                case UnitType::Vw:
                case UnitType::Vh:
                case UnitType::Auto:
                case UnitType::Em:
                case UnitType::Rem:
                    return false;
                case UnitType::Percent:
                case UnitType::Viewport:
                case UnitType::Fraction:
                    return true;
                default:
                    return false;;
                }
            }
            void add ();
            

            //Unit operator+( const Unit& other ) const {
            //    if (type == other.type) {
            //        return { value + other.value, type };
            //    }
            //    // Lógica para mezclar unidades (ej: convertir todo a píxeles)
            //    // Nota: Esto es simplificado, requeriría un contexto.
            //    return *this;
            //}
            
            bool operator==( UnitType otherType ) const;
            //bool operator==( UnitType otherType ) const {
            //    return type == otherType; // Simplemente compara el tipo de unidad del objeto con el tipo dado
            //}
            bool operator!=( UnitType otherType ) const;
            bool operator==( const Unit& otherUnit ) const;
            bool operator!=( const Unit& otherUnit ) const;

            friend bool operator==( const Unit& unit, float pixels );

            friend bool operator!=( const Unit& unit, float pixels );

            friend bool operator==( float pixels, const Unit& unit );

            friend bool operator!=( float pixels, const Unit& unit ) ;

            friend float operator+( const Unit& left, const Unit& right );

            friend float operator+( const Unit& unit, float pixels );

            friend float operator+( float pixels, const Unit& unit );

            friend float operator-( const Unit& left, const Unit& right );

            friend float operator-( const Unit& unit, float pixels );

            friend float operator-( float pixels, const Unit& unit );

            friend bool operator>( const Unit& left, const Unit& right );
            friend bool operator<( const Unit& left, const Unit& right );

            friend bool operator>=( const Unit& left, const Unit& right );

            friend bool operator<=( const Unit& left, const Unit& right );

            friend bool operator>( const Unit& unit, float pixels );

            friend bool operator<( const Unit& unit, float pixels );

            friend bool operator>=( const Unit& unit, float pixels );

            friend bool operator<=( const Unit& unit, float pixels );

            // Conmutatividad para las comparaciones con float
            friend bool operator>( float pixels, const Unit& unit );

            friend bool operator<( float pixels, const Unit& unit );

            friend bool operator>=( float pixels, const Unit& unit );

            friend bool operator<=( float pixels, const Unit& unit );

            // Parse from string (basic implementation)
            static Unit fromString ( const std::string& str ) {
                if (str == "auto") return AUTO ();
                if (str == "normal") return NORMAL ();

                // Encuentra el valor numérico y la unidad
                size_t unitPos = 0;
                float val = std::stof ( str, &unitPos );
                std::string unitStr = str.substr ( unitPos );

                if (unitStr == "px" || unitStr.empty ()) return px ( val );
                if (unitStr == "%") return pc ( val );
                if (unitStr == "em") return em ( val );
                if (unitStr == "rem") return rem ( val );
                if (unitStr == "vw") return vw ( val );
                if (unitStr == "vh") return vh ( val );
                if (unitStr == "fr") return fr ( val );

                // Por defecto, devuelve píxeles
                return px ( val );
            }

            float px () const {
                switch (type) {
                case UnitType::Pixel:
                    return value;
                case UnitType::Percent:
                    return (value / 100.0f) * parentSize;
                case UnitType::Em:
                    return value * fontSize;
                case UnitType::Rem:
                    return value * rootFontSize;
                case UnitType::Viewport:
                    // Si es vw o vh dependerá del contexto donde se use
                    return (value / 100.0f) * (type == UnitType::Viewport ? viewportWidth : viewportHeight);
                case UnitType::Auto:
                case UnitType::Fraction:
                    // Estos requieren cálculos especiales por el layout manager
                    return 0.0f;
                default:
                    return value;
                }
            }


            float resolve ( float parentSize, float fontSize, float rootFontSize, float viewportWidth, float viewportHeight ) const {
                switch (type) {
                case UnitType::Pixel:
                    return value;
                case UnitType::Percent:
                    return (value / 100.0f) * parentSize;
                case UnitType::Em:
                    return value * fontSize;
                case UnitType::Rem:
                    return value * rootFontSize;
                case UnitType::Vw:
                    // Si es vw o vh dependerá del contexto donde se use
                    return (value / 100.0f) * viewportWidth;
                case UnitType::Vh:
                    // Si es vw o vh dependerá del contexto donde se use
                    return (value / 100.0f) * viewportHeight;
                case UnitType::Viewport:
                    // Si es vw o vh dependerá del contexto donde se use
                    return (value / 100.0f) * (type == UnitType::Viewport ? viewportWidth : viewportHeight);
                case UnitType::Auto:
                case UnitType::Fraction:
                    // Estos requieren cálculos especiales por el layout manager
                    return 0.0f;
                default:
                    return value;
                }
            }

            // Soporte para serialización/deserialización
            std::string toString () const {
                if (type == UnitType::Auto) return "auto";

                std::string result = std::to_string ( value );
                switch (type) {
                case UnitType::Pixel: result += "px"; break;
                case UnitType::Percent: result += "%"; break;
                case UnitType::Em: result += "em"; break;
                case UnitType::Rem: result += "rem"; break;
                case UnitType::Viewport: result += "vw"; break; // Simplificado
                case UnitType::Fraction: result += "fr"; break;
                default: break;
                }
                return result;
            }

        private:
            // Constructor privado para forzar el uso de métodos estáticos fábrica
            Unit ( float val, UnitType t ) : value ( val ), type ( t ) { }

            // Amistad con la función de parsing (si la mantienes fuera de la clase)
            friend Unit parseUnitString ( const std::string& unitString );
        
        };

        

        
        float resolveUnitToPixels ( const Unit& unit, float parentWidth, float parentHeight, int viewportWidth, int viewportHeight );

        class UnitResolver
        {
        public:
            struct Context
            {
                glm::vec2 viewportSize;    // Tamaño del viewport (ej: 1920x1080)
                glm::vec2 parentSize;      // Tamaño del elemento padre (ej: 800x600)
                float fontSize;           // Tamaño de fuente actual (ej: 16px)
                float rootFontSize = 16.0f; // Tamaño de fuente raíz (ej: 16px)
            };

            static float resolve ( const Unit& unit, const Context& ctx, bool isHorizontal = true ) {
                switch (unit.type) {
                case UnitType::Pixel: return unit.value;
                case UnitType::Percent: return (unit.value / 100.0f) * (isHorizontal ? ctx.parentSize.x : ctx.parentSize.y);
                case UnitType::Em: return unit.value * ctx.fontSize;
                case UnitType::Rem: return unit.value * ctx.rootFontSize;
                case UnitType::Vw: return (unit.value / 100.0f) * ctx.viewportSize.x;
                case UnitType::Vh: return (unit.value / 100.0f) * ctx.viewportSize.y;
                default: return 0.0f;
                }
            }
        };

	}
	


}


/*



int main() {
    Unit widthSpriteHorse = Unit::px(100);     // 100 píxeles
    Unit heightSpriteHorse = Unit::pc(50);     // 50% (porcentaje)
    Unit viewportWidthUI = Unit::vw(0.8f);    // 80% del ancho de la ventana (viewport width)
    Unit viewportHeightUI = Unit::vh(0.9f);   // 90% del alto de la ventana (viewport height)


    int viewportWidth = 800;
    int viewportHeight = 600;
    float parentWidthForSprites = viewportWidth;
    float parentHeightForSprites = viewportHeight;

    float resolvedWidthHorse = resolveUnitToPixels(widthSpriteHorse, parentWidthForSprites, parentHeightForSprites, viewportWidth, viewportHeight);
    float resolvedHeightHorse = resolveUnitToPixels(heightSpriteHorse, parentWidthForSprites, parentHeightForSprites, viewportWidth, viewportHeight);
    float resolvedWidthUI = resolveUnitToPixels(viewportWidthUI, parentWidthForSprites, parentHeightForSprites, viewportWidth, viewportHeight);
    float resolvedHeightUI = resolveUnitToPixels(viewportHeightUI, parentWidthForSprites, parentHeightForSprites, viewportWidth, viewportHeight);


    std::cout << "Ancho Sprite Horse (px): " << resolvedWidthHorse << std::endl;
    std::cout << "Alto Sprite Horse (px): " << resolvedHeightHorse << std::endl;
    std::cout << "Ancho UI (px): " << resolvedWidthUI << std::endl;
    std::cout << "Alto UI (px): " << resolvedHeightUI << std::endl;


    return 0;
}
*/