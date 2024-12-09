#include "App1.h"
#include "createSquareEntity.h"





using namespace SEVIAN;






App1::App1 ( std::shared_ptr<RendererFactory> factory ) {
	renderer = factory->createRenderer ();
	texture = factory->createTexture ();
	shader = factory->createShader ();

}

void SEVIAN::App1::onKeyPress ( int key ) {
	switch (key) {
	case GLFW_KEY_A:
		Tools::setLastKey ( Key::A );
		break;

	case GLFW_KEY_B:
		Tools::setLastKey ( Key::B );
		break;
	case GLFW_KEY_C:
		Tools::setLastKey ( Key::C );
		break;
	case GLFW_KEY_D:
		Tools::setLastKey ( Key::D );
		break;
	case GLFW_KEY_E:
		Tools::setLastKey ( Key::E );
		break;
	case GLFW_KEY_F:
		Tools::setLastKey ( Key::F );
		break;
	case GLFW_KEY_G:
		Tools::setLastKey ( Key::G );
		break;
	case GLFW_KEY_H:
		Tools::setLastKey ( Key::H );
		break;
	case GLFW_KEY_I:
		Tools::setLastKey ( Key::I );
		break;
	case GLFW_KEY_K:
		Tools::setLastKey ( Key::K );
		break;
	case GLFW_KEY_S:
		Tools::setLastKey ( Key::S );
		break;
	case GLFW_KEY_W:
		Tools::setLastKey ( Key::W );
		break;

	case GLFW_KEY_RIGHT:
		Tools::setLastKey ( Key::RIGHT );
	std::cout << "derecha\n";
		break;
	case GLFW_KEY_LEFT:
		Tools::setLastKey ( Key::LEFT );
		break;
	case GLFW_KEY_DOWN:
		Tools::setLastKey ( Key::DOWN );
		break;
	case GLFW_KEY_UP:
		Tools::setLastKey ( Key::UP );
		break;
	}

}



void SEVIAN::App1::onKeyRelease ( int key ) {
	Tools::setLastKey ( Key::NONE );
	std::cout << key << "Tecla DES presionada\n";
}

void App1::run () {



	

	renderer->initialize ( this );

	//renderer->doLoop ();

	auto scene = world.createScene ( "main" );
	auto cameraEntity = createCamera ( *scene );


	//auto squareEntity = createSquareEntity ( *scene, 0.0, 0.0, 0.0 );
	//auto squareEntity2 = createSquareEntity2 ( *scene, "a", "textures/a1.jpeg", 0.2, 0.2, 2.0f );
	//auto squareEntity3 = createSquareEntity2 ( *scene, "b", "textures/Bandera-Suiza.jpg", -0.7, -0.2, -2.5f );
	//auto squareEntity4 = createSquareEntity2 ( *scene, "c", "textures/a2.jpeg", -0.2, 0.1, -1.5f );

	auto squareEntity4 = createSquareEntity2 ( *scene, "c", "textures/a2.jpeg", 0.0, 0.0, 0.0f );

	auto initSystem = std::make_shared<InitializationSystem> ( renderer );

	auto inputSystem = std::make_shared<InputSystem> ( renderer );
	auto movementSystem = std::make_shared<MovementSystem> ( renderer );
	auto renderSystem = std::make_shared<RenderSystem> ( renderer );


	// Register the system
	scene->addStartSystem ( initSystem );

	scene->addSystem ( inputSystem );
	scene->addSystem ( movementSystem );
	scene->addSystem ( renderSystem );

	scene->runStartSystems ();



	while (!renderer->windowShouldClose ()) {
		glfwPollEvents ();
		//renderFrame ();

		//renderer->renderFrame ();

		world.update ( 5);

	}


	renderer->cleanup ();
}




