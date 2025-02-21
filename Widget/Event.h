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
		glm::vec2 mousePos; // Posici�n del mouse (por ejemplo, en p�xeles)
		int keyCode;        // Para eventos de teclado
		// Puedes agregar m�s informaci�n seg�n tus necesidades
	};

}
