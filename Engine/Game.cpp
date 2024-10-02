#include "Game.h"
#include "RenderSystem.h"
#include "RaceSystem.h"


int SEVIAN::Game::play () {


    if (!glfwInit ()) {
        return -1;
    }

    glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );
    GLFWwindow* window = glfwCreateWindow ( info.width, info.height, info.title.c_str(), NULL, NULL);

    glfwSetKeyCallback ( window, Tools::keyCallback );

    if (info.engine == SEVIAN::VK) {
        render = std::make_unique<VULKAN::VulkanRenderer> ( window, info.width, info.height );
    }
    
    render->initialize ();

    auto world = std::make_unique<World>();
    auto scene = world->createScene ( "main" );

    auto cameraEntity = createCamera ( *scene );

    //auto squareEntity4 = createSpriteEntity ( *scene, "c", "textures/c.jpg", 0.0, 0.0, 0.0f );
    
    //auto modelEntity1 = createModelEntity ( *scene, "models/viking_room.obj", "textures/viking_room.png" );
    
    //auto squareEntity4 = createSpriteEntity ( Key::N1, * scene, "c", "textures/texture.jpg", 0.0, 0.0, 0.0f, 1.0, 1.0, glm::vec3(1.0f, 1.0f, 1.0f) );
    //auto squareEntity5 = createSpriteEntity ( Key::N2,  * scene, "a", "textures/c.jpg", 0.1, 0.0, 0.0f, 1.0, 1.0, glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
    //auto squareEntity6 = createSpriteEntity ( Key::N3, *scene, "a1", "textures/a1.jpeg", 0.1, 0.3, 0.0f, 1.0, 1.0, glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
    
    auto cubo = createCubeEntity ( Key::N5, * scene, 0.0, 0.0, 0.0 );
    //auto cubo2 = createCubeEntity ( Key::N7, *scene, 0.0, 0.0, 0.0 );
    auto movementSystem = std::make_shared<RaceSystem> ( );
    auto renderSystem = std::make_shared<RenderSystem> ( render );

    scene->addSystem ( movementSystem );
    scene->addSystem ( renderSystem );
	

    
    scene->init ();

    auto camera = SEVIAN::Camera ();
    while (!glfwWindowShouldClose ( window )) {


        //render->beginFrame ();

        world->update ();

        //render->drawText ( "YANNY", { 0.0f, 0.0f, 0.0f }, camera );
        //render->endFrame ();
        // Código de renderizado de Vulkan...

        glfwPollEvents ();
    }

    glfwDestroyWindow ( window );
    glfwTerminate ();
    return 0;
}
