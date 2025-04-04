#include "UNIT.h"

namespace SEVIAN::UNIT {
    void Unit::add () {
    }
    bool Unit::operator==( UnitType otherType ) const {
		return type == otherType; // Simplemente compara el tipo de unidad del objeto con el tipo dado
	}


    bool Unit::operator!=( UnitType otherType ) const {
        return !(*this == otherType); // Reutiliza el operador ==
    }
    bool Unit::operator==( const Unit& otherUnit ) const {
        return type == otherUnit.type && value == otherUnit.value;
    }
    bool Unit::operator!=( const Unit& otherUnit ) const {
        return !(*this == otherUnit); // Reutiliza el operador == para objetos Unit
    }

    bool operator==( const Unit& unit, float pixels ) {
        return unit.px () == pixels;
    }

    bool operator!=( const Unit& unit, float pixels ) {
        return unit.px () != pixels;
    }

    bool operator==( float pixels, const Unit& unit ) {
        return pixels == unit.px ();
    }

    bool operator!=( float pixels, const Unit& unit ) {
        return pixels != unit.px ();
    }

    float operator+( const Unit& left, const Unit& right ) {
        return left.px () + right.px (); // Suma los valores resueltos a píxeles de ambos objetos Unit
    }

    float operator+( const Unit& unit, float pixels ) {
        return unit.px () + pixels;
    }

    float operator+( float pixels, const Unit& unit ) {
        return unit.px () + pixels;
    }

    float operator-( const Unit& left, const Unit& right ) {
        return left.px () - right.px (); // Suma los valores resueltos a píxeles de ambos objetos Unit
    }

    float operator-( const Unit& unit, float pixels ) {
        return unit.px () - pixels;
    }

    float operator-( float pixels, const Unit& unit ) {
        return pixels - unit.px ();
    }

    bool operator>( const Unit& left, const Unit& right ) {
        return left.px () > right.px ();
    }
    bool operator<( const Unit& left, const Unit& right ) {
        return left.px () < right.px ();
    }

    bool operator>=( const Unit& left, const Unit& right ) {
        return left.px () >= right.px ();
    }

    bool operator<=( const Unit& left, const Unit& right ) {
        return left.px () <= right.px ();
    }

    bool operator>( const Unit& unit, float pixels ) {
        return unit.px () > pixels;
    }

    bool operator<( const Unit& unit, float pixels ) {
        return unit.px () < pixels;
    }

    bool operator>=( const Unit& unit, float pixels ) {
        return unit.px () >= pixels;
    }

    bool operator<=( const Unit& unit, float pixels ) {
        return unit.px () <= pixels;
    }

    // Conmutatividad para las comparaciones con float
    bool operator>( float pixels, const Unit& unit ) {
        return pixels > unit.px ();
    }

    bool operator<( float pixels, const Unit& unit ) {
        return pixels < unit.px ();
    }

     bool operator>=( float pixels, const Unit& unit ) {
        return pixels >= unit.px ();
    }

    bool operator<=( float pixels, const Unit& unit ) {
        return pixels <= unit.px ();
    }


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
}