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

    if (info.engine == VULKAN) {
        render = std::make_unique<SEVIAN::VulkanRenderer> ( window, info.width, info.height );
    }
    
    render->initialize ();

    auto world = std::make_unique<World>();
    auto scene = world->createScene ( "main" );

    auto cameraEntity = createCamera ( *scene );

    //auto squareEntity4 = createSpriteEntity ( *scene, "c", "textures/c.jpg", 0.0, 0.0, 0.0f );
    auto squareEntity4 = createSpriteEntity (Key::N1,  * scene, "c", "textures/texture.jpg", 0.0, 0.0, 0.0f, 1.0, 2.0);
    auto squareEntity5 = createSpriteEntity ( Key::N2,  * scene, "a", "textures/c.jpg", 0.1, 0.0, 0.0f, 1.5, 1.0);
    auto squareEntity6 = createSpriteEntity ( Key::N3, *scene, "a1", "textures/a1.jpeg", 0.1, 0.3, 0.0f, 1.5, 1.0 );



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
        // C�digo de renderizado de Vulkan...

        glfwPollEvents ();
    }

    glfwDestroyWindow ( window );
    glfwTerminate ();
    return 0;
}
