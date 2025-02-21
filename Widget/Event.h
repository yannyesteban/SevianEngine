#pragma once
#include <glm/glm.hpp> 

namespace X {

	enum class EventType
	{
		MouseMove,
		MouseDown,
		MouseUp,
		KeyPress,
		// Otros eventos que necesites...
	};

	struct Event
	{
		EventType type;
		glm::vec2 mousePos; // Posición del mouse (por ejemplo, en píxeles)
		int keyCode;        // Para eventos de teclado
		// Puedes agregar más información según tus necesidades
	};

}
