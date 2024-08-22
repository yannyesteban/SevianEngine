#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Vertex.h>


namespace SEVIAN {
    enum class Key
    {
        NONE,
        A,
        B,
        C,
        D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        
        LEFT,
        RIGHT,
        UP,
        DOWN,
        N1, N2, N3, N4, N5,
        MAX_KEYS // Esto asegura que tengamos un tamaño correcto para el bitset
    };
    
	class Tools
	{
    public:
        
        static bool isKeyPressed ( Key key );
        static void setLastKey ( Key key );

        static void setCamera ( Camera * camera );
        static Camera * getCamera ( );

        static void keyCallback ( GLFWwindow* window, int key, int scancode, int action, int mods );
	};
}


