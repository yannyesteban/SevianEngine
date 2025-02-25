#pragma once
#include <glm/glm.hpp> 
#include "IInputManager.h"
#include <string>
namespace SEVIAN {

    // Forward declaration para evitar incluir EventTarget.h
    class EventTarget;

    enum class EventType
    {
        None,
        MouseDown,
        MouseUp,
        MouseMove,
        MouseEnter,
        MouseLeave,
        Click,
        DblClick,
        
        KeyPress,
        KeyDown,
        KeyUp,
        
        Focus,
        FocusIn, // lo máximo
        Blur,
        FocusOut, // lo máximo

        ContextMenu,
        // Eventos de arrastre
        DragStart,
        Drag,
        DragEnter,
        DragOver,
        DragLeave,
        Drop,
        DragEnd,

        // Agrega otros tipos de eventos según sea necesario
    };

    enum class EventPhase
    {
        NONE, CAPTURING_PHASE, AT_TARGET, BUBBLING_PHASE
    };

    struct Event
    {
        EventType type;
        float x = 0.0f, y = 0.0f;
        
        bool bubbles = true;
        bool cancelable = true;
        bool stopPropagation;
        bool stopImmediatePropagation = false; // Para detener toda propagación inmediatamente
        bool defaultPrevented = false;
        EventTarget* target;         // Solo necesita forward declaration
        EventTarget* currentTarget;   // Solo necesita forward declaration
        EventPhase phase = EventPhase::NONE;

        INPUT::Key key;
        INPUT::KeyAction action;
        int mods;
        int keyCode;     // Para eventos de teclado

        // Otros datos del evento...


        // Métodos para controlar el comportamiento del evento
        void preventDefault () {
            if (cancelable) {
                defaultPrevented = true;
            }
        }

        void stopEventPropagation () {
            stopPropagation = true;
        }

        void stopEventImmediatePropagation () {
            stopImmediatePropagation = true;
            stopPropagation = true;
        }
    };

    struct DragEvent : Event
    {
        // Campos para eventos de arrastre
        EventTarget* dragSource = nullptr;         // Widget que se está arrastrando
        std::string dataTransferType = "";    // Tipo de datos arrastrados
        void* dataTransfer = nullptr;         // Puntero a los datos arrastrados
        glm::vec2 dragStartPosition = { 0, 0 }; // Posición inicial del arrastre
        glm::vec2 dragOffset = { 0, 0 };        // Diferencia entre posición del cursor y esquina del widget

    };
    struct FocusEvent : Event
    {
        // Campos para eventos de focus
        EventTarget* relatedTarget = nullptr;         // Widget que se está arrastrando

    };

    struct KeyboardEvent : Event
    {
		// Campos para eventos de teclado
		INPUT::Key key;
		INPUT::KeyAction action;
		int mods;
		int keyCode;     // Para eventos de teclado

        bool altKey = false;
        std::string code;
        bool ctrlKey = false;
        bool isComposing = false;
       // std::string key;
        std::string location;
		bool metaKey = false;
		bool repeat = false;
		bool shiftKey = false;

        
  //      std::string keyIdentifier; // trash
		//
		//
		//std::string charCode;
		//std::string keyCodeStr;
		//
		//std::string locale;
		//std::string repeatStr;
		//std::string isComposingStr;
		//std::string altKeyStr;
		//std::string ctrlKeyStr;
		//std::string metaKeyStr;

		//std::string shiftKeyStr;
		//std::string keyIdentifierStr;
		//std::string codeStr;
		//std::string keyStr;
		//std::string charCodeStr;
		//std::string locationStr;
		//std::string localeStr;
       
        //Returns a boolean value indicating if a modifier key such as Alt, Shift, Ctrl, or Meta, was pressed when the event was created.
		bool getModifierState ( std::string keyArg ) {
			return false;
		}

        

    };

}
