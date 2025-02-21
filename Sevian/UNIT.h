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
        enum class UnitType
        {
            Pixel,      // Unidad absoluta (px)
            Percent,    // Porcentaje relativo al contenedor/padre (%)
            Em,         // Relativo al tamaño de fuente del contexto (ej: 1em = 16px)
            Rem,        // Relativo al tamaño de fuente raíz (root em)
            Vw,         // 1% del ancho del viewport
            Vh,         // 1% del alto del viewport
            Auto,        // Valor calculado automáticamente (ej: centrado)


            Fraction, // Agrega un tipo de unidad fr (fracción) para Grid:
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

            Unit operator+( const Unit& other ) const {
                if (type == other.type) {
                    return { value + other.value, type };
                }
                // Lógica para mezclar unidades (ej: convertir todo a píxeles)
                // Nota: Esto es simplificado, requeriría un contexto.
                return *this;
            }

        private:
            // Constructor privado para forzar el uso de métodos estáticos fábrica
            Unit ( float val, UnitType t ) : value ( val ), type ( t ) { }

            // Amistad con la función de parsing (si la mantienes fuera de la clase)
            friend Unit parseUnitString ( const std::string& unitString );
        
        };

        Unit parseUnitString ( const std::string& unitString ) {
            std::stringstream ss ( unitString );
            float value;
            std::string unitTypeStr;

            ss >> value >> unitTypeStr;

            if (unitTypeStr == "px") {
                return  { value, UnitType::Pixel };
            }
            else if (unitTypeStr == "%") {
                return Unit { value, UnitType::Percent };
            }
            else if (unitTypeStr == "vw") {
                return Unit { value, UnitType::Vw };
            }
            else if (unitTypeStr == "vh") {
                return  { value, UnitType::Vh };
            }
            else {
                throw std::invalid_argument ( "Unidad desconocida: " + unitTypeStr );
            }
        }
        float resolveUnitToPixels ( const Unit& unit, float parentWidth, float parentHeight, int viewportWidth, int viewportHeight ) {
            switch (unit.type) { // Usando getter para el tipo
            case UnitType::Pixel:
                return unit.value; // Usando getter para el valor
            case UnitType::Percent:
                // Asume que el porcentaje es relativo al ancho del padre para el ancho, y al alto para el alto
                return (unit.value / 100.0f) * parentWidth;
            case UnitType::Vw:
                return (unit.value / 100.0f) * viewportWidth;
            case UnitType::Vh:
                return (unit.value / 100.0f) * viewportHeight;
            default:
                return unit.value;
            }
        }

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