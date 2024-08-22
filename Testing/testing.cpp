#include "VulkanRenderer.h"



int main () {

    const int PI = 3.1415926535;


    if (!glfwInit ()) {
        return -1;
    }

    glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );
    GLFWwindow* window = glfwCreateWindow ( 800, 600,  "Vulkan Window", NULL, NULL );

    // Código de inicialización de Vulkan...

    //std::shared_ptr<SEVIAN::RenderInterface> render = std::make_unique<SEVIAN::VulkanRenderer> (window, 8010, 600 );
    
   //render->initialize ();

   auto render = new SEVIAN::VulkanRenderer ( window, 1800, 600 );
   render->test3 ();
   // render->initialize2 ();
   auto camera = SEVIAN::Camera ();
    while (!glfwWindowShouldClose ( window )) {
       
       
        render->beginFrame ();
      
        render->drawText ( "YANNY", {0.0f, 0.0f, 0.0f }, camera);
        render->endFrame ();
        // Código de renderizado de Vulkan...

        glfwPollEvents ();
    }

    glfwDestroyWindow ( window );
    glfwTerminate ();
    return 0;
}