#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
namespace SEVIAN::COLOR {



    // Enumeración de colores predefinidos
    enum class ColorName
    {
        // Colores básicos
        Black,
        White,
        Red,
        Green,
        Blue,
        Yellow,
        Cyan,
        Magenta,

        // Grises
        LightGray,
        Gray,
        DarkGray,

        // Colores extendidos
        Orange,
        Purple,
        Pink,
        Brown,
        Lime,
        Navy,
        Teal,
        Olive,
        Maroon,
        Aqua,
        Silver,
        Gold,

        // Colores de interfaz
        Primary,
        Secondary,
        Success,
        Danger,
        Warning,
        Info,
        Light,
        Dark,

        // Color transparente
        Transparent
    };

    // Estructura principal de Color
    struct Color
    {
        
        glm::vec4 rgba = glm::vec4 ( 1.0f, 1.0f, 1.0f, 1.0f );

        

        // Constructores
        Color () = default;

        // Constructor desde componentes individuales
        Color ( float red, float green, float blue, float alpha = 1.0f )
            : rgba ( red, green, blue, alpha ) {
        }

        // Constructor desde vec3 (RGB con alpha=1.0)
        Color ( const glm::vec3& rgb )
            : rgba ( rgb, 1.0f ) {
        }

        // Constructor desde vec4 (RGBA)
        Color ( const glm::vec4& rgba )
            : rgba ( rgba ) {
        }

        // Constructor desde ColorName
        Color ( ColorName name );

        // Utilidades para conversión
        
        const glm::vec4& toVec4 () const { return rgba; }

        // Métodos estáticos para crear colores
        static Color fromRGB ( int red, int green, int blue, int alpha = 255 ) {
            return Color ( red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f );
        }

        static Color fromHex ( const std::string& hexCode );

        // Operadores para manipulación de colores
        Color operator+( const Color& other ) const {
            return Color ( rgba + other.rgba );
        }

        Color operator*( float factor ) const {
            return Color ( rgba * factor );
        }

        bool operator==( const Color& other ) const {
            return rgba == other.rgba;
        }

        bool operator!=( const Color& other ) const {
            return !(*this == other);
        }
    };

    // Implementación del constructor desde ColorName
    inline Color::Color ( ColorName name ) {
        switch (name) {
        case ColorName::Black:        rgba = glm::vec4 ( 0.0f, 0.0f, 0.0f, 1.0f ); break;
        case ColorName::White:        rgba = glm::vec4 ( 1.0f, 1.0f, 1.0f, 1.0f ); break;
        case ColorName::Red:          rgba = glm::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f ); break;
        case ColorName::Green:        rgba = glm::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ); break;
        case ColorName::Blue:         rgba = glm::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ); break;
        case ColorName::Yellow:       rgba = glm::vec4 ( 1.0f, 1.0f, 0.0f, 1.0f ); break;
        case ColorName::Cyan:         rgba = glm::vec4 ( 0.0f, 1.0f, 1.0f, 1.0f ); break;
        case ColorName::Magenta:      rgba = glm::vec4 ( 1.0f, 0.0f, 1.0f, 1.0f ); break;
        case ColorName::LightGray:    rgba = glm::vec4 ( 0.75f, 0.75f, 0.75f, 1.0f ); break;
        case ColorName::Gray:         rgba = glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ); break;
        case ColorName::DarkGray:     rgba = glm::vec4 ( 0.25f, 0.25f, 0.25f, 1.0f ); break;
        case ColorName::Orange:       rgba = glm::vec4 ( 1.0f, 0.65f, 0.0f, 1.0f ); break;
        case ColorName::Purple:       rgba = glm::vec4 ( 0.5f, 0.0f, 0.5f, 1.0f ); break;
        case ColorName::Pink:         rgba = glm::vec4 ( 1.0f, 0.75f, 0.8f, 1.0f ); break;
        case ColorName::Brown:        rgba = glm::vec4 ( 0.65f, 0.32f, 0.17f, 1.0f ); break;
        case ColorName::Lime:         rgba = glm::vec4 ( 0.75f, 1.0f, 0.0f, 1.0f ); break;
        case ColorName::Navy:         rgba = glm::vec4 ( 0.0f, 0.0f, 0.5f, 1.0f ); break;
        case ColorName::Teal:         rgba = glm::vec4 ( 0.0f, 0.5f, 0.5f, 1.0f ); break;
        case ColorName::Olive:        rgba = glm::vec4 ( 0.5f, 0.5f, 0.0f, 1.0f ); break;
        case ColorName::Maroon:       rgba = glm::vec4 ( 0.5f, 0.0f, 0.0f, 1.0f ); break;
        case ColorName::Aqua:         rgba = glm::vec4 ( 0.0f, 1.0f, 1.0f, 1.0f ); break;
        case ColorName::Silver:       rgba = glm::vec4 ( 0.75f, 0.75f, 0.75f, 1.0f ); break;
        case ColorName::Gold:         rgba = glm::vec4 ( 1.0f, 0.84f, 0.0f, 1.0f ); break;
        case ColorName::Primary:      rgba = glm::vec4 ( 0.0f, 0.5f, 1.0f, 1.0f ); break;
        case ColorName::Secondary:    rgba = glm::vec4 ( 0.6f, 0.6f, 0.6f, 1.0f ); break;
        case ColorName::Success:      rgba = glm::vec4 ( 0.2f, 0.8f, 0.2f, 1.0f ); break;
        case ColorName::Danger:       rgba = glm::vec4 ( 0.9f, 0.2f, 0.2f, 1.0f ); break;
        case ColorName::Warning:      rgba = glm::vec4 ( 1.0f, 0.8f, 0.0f, 1.0f ); break;
        case ColorName::Info:         rgba = glm::vec4 ( 0.2f, 0.6f, 1.0f, 1.0f ); break;
        case ColorName::Light:        rgba = glm::vec4 ( 0.98f, 0.98f, 0.98f, 1.0f ); break;
        case ColorName::Dark:         rgba = glm::vec4 ( 0.2f, 0.2f, 0.2f, 1.0f ); break;
        case ColorName::Transparent:  rgba = glm::vec4 ( 0.0f, 0.0f, 0.0f, 0.0f ); break;
        default:                      rgba = glm::vec4 ( 1.0f, 1.0f, 1.0f, 1.0f ); break;
        }
    }

    // Implementación del método fromHex
    inline Color Color::fromHex ( const std::string& hexCode ) {
        // Eliminar el # si existe
        std::string code = hexCode;
        if (code[0] == '#') {
            code = code.substr ( 1 );
        }

        // Conversión según la longitud del código
        try {
            if (code.length () == 6) {
                // Formato RRGGBB
                int r = std::stoi ( code.substr ( 0, 2 ), nullptr, 16 );
                int g = std::stoi ( code.substr ( 2, 2 ), nullptr, 16 );
                int b = std::stoi ( code.substr ( 4, 2 ), nullptr, 16 );
                return fromRGB ( r, g, b );
            }
            else if (code.length () == 8) {
                // Formato RRGGBBAA
                int r = std::stoi ( code.substr ( 0, 2 ), nullptr, 16 );
                int g = std::stoi ( code.substr ( 2, 2 ), nullptr, 16 );
                int b = std::stoi ( code.substr ( 4, 2 ), nullptr, 16 );
                int a = std::stoi ( code.substr ( 6, 2 ), nullptr, 16 );
                return fromRGB ( r, g, b, a );
            }
            else if (code.length () == 3) {
                // Formato RGB
                int r = std::stoi ( code.substr ( 0, 1 ) + code.substr ( 0, 1 ), nullptr, 16 );
                int g = std::stoi ( code.substr ( 1, 1 ) + code.substr ( 1, 1 ), nullptr, 16 );
                int b = std::stoi ( code.substr ( 2, 1 ) + code.substr ( 2, 1 ), nullptr, 16 );
                return fromRGB ( r, g, b );
            }
        }
        catch (const std::exception&) {
            // En caso de error, devolver blanco
            return Color ( 1.0f, 1.0f, 1.0f, 1.0f );
        }

        // Valor por defecto
        return Color ( 1.0f, 1.0f, 1.0f, 1.0f );
    }

    

}

// Namespace con colores predefinidos para uso directo
namespace Colors {
    inline const SEVIAN::COLOR::Color Black = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Black );
    inline const SEVIAN::COLOR::Color White = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::White );
    inline const SEVIAN::COLOR::Color Red = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Red );
    inline const SEVIAN::COLOR::Color Green = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Green );
    inline const SEVIAN::COLOR::Color Blue = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Blue );
    inline const SEVIAN::COLOR::Color Yellow = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Yellow );
    inline const SEVIAN::COLOR::Color Cyan = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Cyan );
    inline const SEVIAN::COLOR::Color Magenta = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Magenta );
    inline const SEVIAN::COLOR::Color LightGray = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::LightGray );
    inline const SEVIAN::COLOR::Color Gray = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Gray );
    inline const SEVIAN::COLOR::Color DarkGray = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::DarkGray );
    inline const SEVIAN::COLOR::Color Orange = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Orange );
    inline const SEVIAN::COLOR::Color Purple = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Purple );
    inline const SEVIAN::COLOR::Color Pink = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Pink );
    inline const SEVIAN::COLOR::Color Brown = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Brown );
    inline const SEVIAN::COLOR::Color Lime = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Lime );
    inline const SEVIAN::COLOR::Color Navy = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Navy );
    inline const SEVIAN::COLOR::Color Teal = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Teal );
    inline const SEVIAN::COLOR::Color Olive = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Olive );
    inline const SEVIAN::COLOR::Color Maroon = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Maroon );
    inline const SEVIAN::COLOR::Color Aqua = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Aqua );
    inline const SEVIAN::COLOR::Color Silver = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Silver );
    inline const SEVIAN::COLOR::Color Gold = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Gold );
    inline const SEVIAN::COLOR::Color Primary = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Primary );
    inline const SEVIAN::COLOR::Color Secondary = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Secondary );
    inline const SEVIAN::COLOR::Color Success = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Success );
    inline const SEVIAN::COLOR::Color Danger = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Danger );
    inline const SEVIAN::COLOR::Color Warning = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Warning );
    inline const SEVIAN::COLOR::Color Info = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Info );
    inline const SEVIAN::COLOR::Color Light = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Light );
    inline const SEVIAN::COLOR::Color Dark = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Dark );
    inline const SEVIAN::COLOR::Color Transparent = SEVIAN::COLOR::Color ( SEVIAN::COLOR::ColorName::Transparent );

    inline const SEVIAN::COLOR::Color rgb ( int red, int green, int blue, int alpha = 255 ) {
        return SEVIAN::COLOR::Color::fromRGB ( red, green, blue, alpha );
    }
}