#pragma once
#include <glm/glm.hpp> 
#include "IInputManager.h"
namespace SEVIAN {
    enum class EventType
    {
        Click,
        DblClick,
        MouseMove,
		MouseEnter,
		MouseLeave,
        MouseDown,
        MouseUp,
        KeyPress,
        ContextMenu,
        // Agrega otros tipos de eventos según sea necesario
    };

    struct Event
    {
        EventType type;
        glm::vec2 mousePos;   // Para eventos de mouse
        float x = 0.0f;
        float y = 0.0f;
        
        INPUT::Key key;
        INPUT::KeyAction action;
        int mods;
        int keyCode;     // Para eventos de teclado

        // Otros datos del evento...
    };
}
