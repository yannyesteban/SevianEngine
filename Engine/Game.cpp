#include "Game.h"
#include "RenderSystem.h"
#include "AnimationSystem.h"
#include "RaceSystem.h"

#include "CameraEntity.h"
#include "CameraSystem.h"

#include "LightEntity.h"
#include "LightSystem.h"

#include "SceneData.h"

float calculateDeltaTime () {
    static auto lastTime = std::chrono::high_resolution_clock::now ();
    auto currentTime = std::chrono::high_resolution_clock::now ();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - lastTime ).count ();
    lastTime = currentTime;
    return deltaTime;
}

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

    //auto cameraEntity = createCamera ( *scene );

    CameraComponent camInfo = {  };
    camInfo.width = info.width;
    camInfo.height = info.height;
    auto cam2 = createCamera ( *scene, camInfo );


    LightComponent lightInfo = {  };
    auto light = createLight ( *scene, lightInfo );


    //auto squareEntity4 = createSpriteEntity ( *scene, "c", "textures/c.jpg", 0.0, 0.0, 0.0f );
    
    //auto modelEntity1 = createModelEntity ( *scene, "models/viking_room.obj", "textures/viking_room.png" );
    
    //auto squareEntity4 = createSpriteEntity ( Key::N1, * scene, "c", "textures/texture.jpg", 0.0, 0.0, 0.0f, 1.0, 1.0, glm::vec3(1.0f, 1.0f, 1.0f) );
    //auto squareEntity5 = createSpriteEntity ( Key::N2,  * scene, "a", "textures/c.jpg", 0.1, 0.0, 0.0f, 1.0, 1.0, glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
    //auto squareEntity6 = createSpriteEntity ( Key::N7, *scene, "a1", "textures/aifuture.jpeg", 0.1, 0.3, 0.0f, 1.0, 1.0, glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
    

    //loadScene ( "models/viking_room.obj" );
    /*
    auto cubo4 = createCubeEntity ( "aifuture", "textures/aifuture.jpeg", Key::N0, *scene, 0.0, 0.0, 2.0 );
    auto cubo1 = createCubeEntity ( "aTravel", "textures/aTravel.jpeg", Key::N1, *scene, 0.0, 0.0, 0.0 );
    auto cubo2 = createCubeEntity ( "escultura", "textures/escultura.jpg", Key::N2, *scene, 0.0, 0.0, -2.5 );
    auto cubo3 = createCubeEntity ( "suiza", "textures/suiza.jpg", Key::N3, * scene, 0.0, 0.0, -3.0 );
    
    auto sphere1 = createSphereEntity ( "escultura", "textures/escultura.jpg", Key::N5, *scene, 0.0, 0.0, -1.0, 0.5, 64, 64 );
    */
    //createActorEntity ( *scene, "models/doscosas.glb" );

   
    
    if (1 == 0) {
        createActorEntity ( *scene, "C:/sources2025/Blender/tres.glb" );
    }
    
    
    
    
    
    //createActorEntity ( *scene, "models/StereoJack.obj");
    
   
    auto lightSystem = std::make_shared<LightSystem> ();
    auto cameraSystem = std::make_shared<CameraSystem> ();
    auto transformSystem = std::make_shared<TransformSystem> ();
    auto animationSystem = std::make_shared<AnimationSystem> ();
    auto movementSystem = std::make_shared<RaceSystem> ();
    auto renderSystem = std::make_shared<RenderSystem> ( render );

    scene->addSystem ( movementSystem );
    scene->addSystem ( lightSystem );
    scene->addSystem ( cameraSystem );
    scene->addSystem ( transformSystem );
    scene->addSystem ( animationSystem );
    scene->addSystem ( renderSystem );
	
    
    scene->init (0);

    auto camera = SEVIAN::Camera ();
    while (!glfwWindowShouldClose ( window )) {
        float deltaTime = calculateDeltaTime ();

        //render->beginFrame ();

        world->update ( deltaTime );

        //render->drawText ( "YANNY", { 0.0f, 0.0f, 0.0f }, camera );
        //render->endFrame ();
        // C�digo de renderizado de Vulkan...

        glfwPollEvents ();
    }

    glfwDestroyWindow ( window );
    glfwTerminate ();
    return 0;
}
