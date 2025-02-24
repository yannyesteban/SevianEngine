#include "Game.h"
#include "InputManager.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "RenderSystem2.h"
#include "AnimationSystem.h"
#include "RaceSystem.h"

#include "CameraEntity.h"
#include "CameraSystem.h"

#include "LightEntity.h"
#include "LightSystem.h"

#include "SceneData.h"
#include "WidgetManager.h"
#include "LabelWidget.h"
#include "ButtonWidget.h"
#include "FontManager.h"
//#include "SpriteManager.h"

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
	GLFWwindow* window = glfwCreateWindow ( info.width, info.height, info.title.c_str (), NULL, NULL );
	auto inputManager = std::make_shared<InputManager> ();

	Tools::addManager ( inputManager );
	glfwSetWindowUserPointer ( window, inputManager.get () );
	glfwSetKeyCallback ( window, Tools::keyCallback2 );
	glfwSetCursorPosCallback ( window, Tools::CursorCallback );
	glfwSetMouseButtonCallback ( window, Tools::MouseButtonCallback );
	glfwSetScrollCallback ( window, Tools::ScrollCallback );
	
	
	FONT::Manager::createFont ( "arial", "C:\\source\\2024\\Sevian\\Engine\\fonts\\arial.ttf" );
	//FONT::Manager::createFont ( "arialbd", "C:\\source\\2024\\Sevian\\Engine\\fonts\\arialbd.ttf" );
	//FONT::Manager::createFont ( "baily", "C:\\source\\2024\\Sevian\\Engine\\fonts\\baily.ttf" );
	//FONT::Manager::createFont ( "hemihead", "C:\\source\\2024\\Sevian\\Engine\\fonts\\hemihead.ttf" );

	
	

	if (info.engine == SEVIAN::VK) {
		render = std::make_shared<VULKAN::VulkanRenderer> ( window, info.width, info.height );
		render->initialize ();
		render->printTest ();
		
		//spriteManager = std::make_shared<VULKAN::SpriteManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );

		//renderText = std::make_shared<VULKAN::TextManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
		

		//render->registerManager<VULKAN::SpriteManager, VULKAN::SpriteManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
		render->registerManager<RENDERER::iSpriteManager, VULKAN::SpriteManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
		//render->registerManager<VULKAN::TextManager> ( INFO );
		render->registerManager<RENDERER::iTextManager, VULKAN::TextManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );

	}

	auto r = render->getManager<RENDERER::iTextManager> ();
	r->addFont ( FONT::Manager::getFont ( "arial" ), true );

	//r->addFont ( FONT::Manager::getFont ( "arial" ), true );
	//renderText->addFont ( FONT::Manager::getFont ( "arialbd" ) );
	//renderText->addFont ( FONT::Manager::getFont ( "baily" ) );
	//renderText->addFont ( FONT::Manager::getFont ( "hemihead" ) );

	GameInfo gameInfo;


	gameInfo.widgetManager = std::make_shared<WIDGET::WidgetManager> ( render, inputManager );
	gameInfo.widgetManager->id = "root";
	WIDGET::TextInfo textInfo { "arial", "Yanny Esteban" };
	textInfo.x = 100.0f;
	textInfo.y = 100.0f;
	textInfo.width = 600.0f;
	textInfo.height = 600.0f;

	textInfo.scale = 40.0f;

	auto labelOk = std::make_unique<WIDGET::Label> ( render, textInfo );
	gameInfo.widgetManager->appendChild ( std::move(labelOk) ); // <-error
	

	WIDGET::TextInfo okButtonInfo { "arial", "Aceptar" };
	okButtonInfo.x = 300.0f;
	okButtonInfo.y = 400.0f;
	okButtonInfo.width = 300.0f;
	okButtonInfo.height = 100.0f;
	okButtonInfo.scale = 10.0f;

	auto okButton = std::make_unique<WIDGET::Button> ( render, okButtonInfo );
	okButton->id = "okButton";
	okButton->attachEvent ( EventType::Click,  []( const Event& event ) {
		std::cout << "Botón Aceptar presionado\n";
		return false;
		} );
	/**/okButton->attachEvent (EventType::MouseEnter, [](const Event& event) {
		std::cout << "Entrando al Botón\n";
		return false;

		} );
	okButton->attachEvent ( EventType::MouseLeave, []( const Event& event ) {
		std::cout << "Saliendo del Botón\n";
		return false;

		} );
	okButton->attachEvent ( EventType::DblClick, []( const Event& event ) {
		std::cout << "Presionando Doble click\n";
		return false;
		} );
	gameInfo.widgetManager->appendChild ( std::move ( okButton ) );
	
	/*WIDGET::TextInfo cancelButtonInfo { "arial", "Cancelar" };
	cancelButtonInfo.x = 400.0f;
	cancelButtonInfo.y = 200.0f;
	cancelButtonInfo.width = 150.0f;
	cancelButtonInfo.height = 50.0f;
	cancelButtonInfo.scale = 10.0f;
	auto cancelButton = gameInfo.widgetManager->create<WIDGET::Button> ( cancelButtonInfo );

	*/
	//gameInfo.widgetManager->addWidget ( cancelButton );




	
	/* 
	
	
	

	


	WIDGET::TextInfo textInfo2 { "arialbd", "Cancelar" };
	textInfo2.x = -100.0f;
	textInfo2.y = -300.0f;
	auto labelOk2 = std::make_shared<WIDGET::Label> ( render, textInfo2 );
	gameInfo.widgetManager->addWidget ( labelOk2 );

	WIDGET::TextInfo textInfo3 { "hemihead", "Welcome!" };
	textInfo3.x = -100.0f;
	textInfo3.y = -50.0f;
	textInfo3.scale = 100;
	auto labelOk3 = std::make_shared<WIDGET::Label> ( render, textInfo3 );
	gameInfo.widgetManager->addWidget ( labelOk3 );
	*/
	
	
	gameInfo.render = render;
	

	render->addTexture ( "escultura", "textures/escultura.jpg" );


	auto world = std::make_unique<World> ();
	auto scene = world->createScene ( "main" );

	//auto cameraEntity = createCamera ( *scene );

	CameraComponent camInfo = {  };
	camInfo.width = info.width;
	camInfo.height = info.height;
	auto cam2 = createCamera ( *scene, camInfo );


	CameraComponent camInfo2 = {  };
	camInfo2.width = info.width;
	camInfo2.height = info.height;
	camInfo2.position = glm::vec3 ( 0.0f, 0.0f, 2.0f );
	camInfo2.front = glm::vec3 ( 0.0f, 0.0f, 0.0f );
	camInfo2.up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

	createCamera2D ( *scene, camInfo2 );


	LightComponent lightInfo = {  };
	auto light = createLight ( *scene, lightInfo );
	//createActorEntity ( *scene, "models/doscosas.glb" );

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


	auto spriter = render->getManager<RENDERER::iSpriteManager> ();

	SEVIAN::SpriteInfo spriteInfo1 { "escultura", {0.0, 0.0, 0.0}, {100.0f, 80.0f}, {0.9f, 0.1f, 0.0f} };
	SEVIAN::SpriteInfo spriteInfo2 { "escultura", {100.0, 0.0f, 0.0}, {100.0f, 80.0f}, {0.9f, 0.1f, 2.0f} };
	SEVIAN::SpriteInfo spriteInfo3 { "escultura", {200.0, 0.0f, 0.0}, {100.0f, 80.0f}, {0.9f, 0.1f, 2.0f} };
	spriteInfo1.scale = glm::vec3 ( 100.0f, 100.0f, 1.0f );
	spriteInfo2.scale = glm::vec3 ( 100.0f, 100.0f, 1.0f );
	spriteInfo2.rotation = glm::vec3 ( 45.0f, 0.0f, 0.0f );
	spriteInfo3.scale = glm::vec3 ( 100.0f, 100.0f, 1.0f );


	auto sprite1 = createSprite2DEntity ( *scene, spriter, spriteInfo1 );
	auto sprite2 = createSprite2DEntity ( *scene, spriter, spriteInfo2 );
	auto sprite3 = createSprite2DEntity ( *scene, spriter, spriteInfo3 );


	
	//createActorEntity ( *scene, "models/StereoJack.obj");

	auto inputSystem = std::make_shared<InputSystem> ( inputManager );
	auto lightSystem = std::make_shared<LightSystem> ();
	auto cameraSystem = std::make_shared<CameraSystem> ();
	auto camera2DSystem = std::make_shared<Camera2DSystem> ();
	auto transformSystem = std::make_shared<TransformSystem> ();
	auto animationSystem = std::make_shared<AnimationSystem> ();
	auto movementSystem = std::make_shared<RaceSystem> ();
	auto renderSystem = std::make_shared<RenderSystem2> ( gameInfo );

	scene->addSystem ( movementSystem );
	scene->addSystem ( lightSystem );
	scene->addSystem ( cameraSystem );
	scene->addSystem ( camera2DSystem );
	scene->addSystem ( transformSystem );
	scene->addSystem ( animationSystem );
	scene->addSystem ( renderSystem );


	scene->init ( 0 );

	auto camera = SEVIAN::Camera ();

	

	while (!glfwWindowShouldClose ( window )) {
		float deltaTime = calculateDeltaTime ();

		//render->beginFrame ();

		world->update ( deltaTime );

		//render->drawText ( "YANNY", { 0.0f, 0.0f, 0.0f }, camera );
		//render->endFrame ();
		// Código de renderizado de Vulkan...

		glfwPollEvents ();
	}

	glfwDestroyWindow ( window );
	glfwTerminate ();
	return 0;
}
