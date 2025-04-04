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
#include "Label.h"
#include "ButtonWidget.h"
#include "FontManager.h"
#include <Stack.h>
//#include "SpriteManager.h"

float calculateDeltaTime () {
	static auto lastTime = std::chrono::high_resolution_clock::now ();
	auto currentTime = std::chrono::high_resolution_clock::now ();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period> ( currentTime - lastTime ).count ();
	lastTime = currentTime;
	return deltaTime;
}
using namespace UNIT;
using namespace SEVIAN::WIDGET;
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

	if (1 == 2) {
		FONT::Manager::createFont ( "arial", "C:\\source\\2024\\Sevian\\Engine\\fonts\\arial.ttf" );
		//FONT::Manager::createFont ( "arialbd", "C:\\source\\2024\\Sevian\\Engine\\fonts\\arialbd.ttf" );
		//FONT::Manager::createFont ( "baily", "C:\\source\\2024\\Sevian\\Engine\\fonts\\baily.ttf" );
		//FONT::Manager::createFont ( "hemihead", "C:\\source\\2024\\Sevian\\Engine\\fonts\\hemihead.ttf" );
	}

	if (info.engine == SEVIAN::VK) {
		render = std::make_shared<VULKAN::VulkanRenderer> ( window, info.width, info.height );
		render->initialize ();
		render->printTest ();

		//spriteManager = std::make_shared<VULKAN::SpriteManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );

		//renderText = std::make_shared<VULKAN::TextManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );


		//render->registerManager<VULKAN::SpriteManager, VULKAN::SpriteManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
		render->registerManager<RENDERER::iSpriteManager, VULKAN::SpriteManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
		render->registerManager<RENDERER::iWidgetManager, VULKAN::WidgetRenderer> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
		//render->registerManager<VULKAN::TextManager> ( INFO );

		if (1 == 2) {
			render->registerManager<RENDERER::iTextManager, VULKAN::TextManager> ( std::dynamic_pointer_cast<VULKAN::VulkanRenderer>(render) );
			auto r = render->getManager<RENDERER::iTextManager> ();
			r->addFont ( FONT::Manager::getFont ( "arial" ), true );
		}


	}

	//renderText->addFont ( FONT::Manager::getFont ( "arial" ), true );
	//renderText->addFont ( FONT::Manager::getFont ( "arialbd" ) );
	//renderText->addFont ( FONT::Manager::getFont ( "baily" ) );
	//renderText->addFont ( FONT::Manager::getFont ( "hemihead" ) );

	GameInfo gameInfo;


	gameInfo.widgetManager = std::make_shared<WIDGET::WidgetManager> ( render, inputManager );
	gameInfo.widgetManager->id = "root";

	auto bodyLayout = gameInfo.widgetManager->addStyle<WIDGET::LayoutComponent> ();
	auto bodyBackground = gameInfo.widgetManager->addStyle<WIDGET::BackgroundComponent> ();
	auto bodyText = gameInfo.widgetManager->addStyle<WIDGET::TextComponent> ();
	bodyText->setTextColor ( Colors::White ).setFontSize ( Unit::px ( 16.0f ) ).setTextAlign ( TextAlign::Center );


	bodyLayout->boxSizing = BoxSizing::BorderBox;

	bodyLayout->display = WIDGET::Display::Block;
	bodyLayout->position = Position::Static;

	bodyLayout->margin = Margin::px ( 35.0f );
	bodyLayout->padding = Padding::px ( 50.0f );

	bodyLayout->left = Unit::px ( 50.0f );
	bodyLayout->top = Unit::px ( 20.0f );

	//bodyLayout->width = Unit::AUTO();
	bodyLayout->width = Unit::px ( 550.0f );
	bodyLayout->height = Unit::AUTO ();
	//bodyLayout->height = Unit::px (660.0f);

	bodyBackground->backGroundColor = Colors::rgb ( 120, 14, 87 );
	bodyLayout->border = Border ( 20.0f, BorderStyle::Solid, Colors::rgb ( 202, 198, 198 ), 0.0f );

	//bodyBackground->backGroundColor = Colors::Yellow;

	auto ref = std::make_unique<WIDGET::Stack> ( render );
	ref->id = "ref";
	auto refStyle = ref->addStyle<WIDGET::LayoutComponent> ();
	auto refBackground = ref->addStyle<WIDGET::BackgroundComponent> ();

	refStyle->boxSizing = BoxSizing::ContentBox;

	refStyle->display = WIDGET::Display::Block;
	refStyle->position = Position::Static;

	refStyle->margin = Margin::px ( 55.0f );
	refStyle->padding = Padding::px ( 0.0f );

	refStyle->left = Unit::px ( 0.0f );
	refStyle->top = Unit::px ( 20.0f );

	//refStyle->width = Unit::AUTO ();
	refStyle->width = Unit::px ( 100.0f );
	refStyle->height = Unit::px ( 150.0f );

	refStyle->border = Border ( 10.0f, BorderStyle::Solid, Colors::rgb ( 202, 198, 198 ), 0.0f );
	refBackground->backGroundColor = Colors::rgb ( 244, 41, 180 );
	gameInfo.widgetManager->appendChild ( std::move ( ref ) );

	//***********************************************

	auto stack = std::make_unique<WIDGET::Stack> ( render );
	stack->id = "stack";
	auto stackStyle = stack->addStyle<WIDGET::LayoutComponent> ();

	stackStyle->boxSizing = BoxSizing::BorderBox;

	stackStyle->display = WIDGET::Display::Block;
	stackStyle->position = Position::Static;

	stackStyle->left = Unit::px ( 20.0f );
	stackStyle->top = Unit::px ( 20.0f );

	stackStyle->margin = Margin::px ( 50.0f );
	stackStyle->padding = Padding::px ( 20.0f );

	stackStyle->width = Unit::AUTO ();
	//stackStyle->width = Unit::px (500.0f);
	stackStyle->height = Unit::AUTO ();
	stackStyle->height = Unit::px ( 250.0f );

	stackStyle->border = Border ( 10.0f, BorderStyle::Solid, Colors::rgb ( 202, 198, 198 ), 0.0f );

	auto stackBackground = stack->addStyle<WIDGET::BackgroundComponent> ();
	stackBackground->backGroundColor = Colors::rgb ( 244, 41, 180 );


	auto label1 = std::make_unique<WIDGET::Label> ( render, "Hello!" );
	label1->id = "label1";

	//stack->appendChild ( std::move ( label1 ) );



	gameInfo.widgetManager->appendChild ( std::move ( stack ) );





	/*
auto stack1 = std::make_unique<WIDGET::Stack> ( render );
stack1->id = "stack 01";
auto style1 = stack1->addStyle<WIDGET::LayoutComponent> ();
style1->left = Unit::px ( 200.0f );
style1->top = Unit::px ( 300.0f );
style1->display = WIDGET::Display::Block;
style1->width = Unit::px ( 300.0f );
style1->height = Unit::px ( 100.0f );
style1->padding = Padding::px(0.0f);
style1->margin = Margin::px ( 0.0f );
style1->border = Border ( 0.0f, BorderStyle::Solid, Colors::Red );
auto background1 = stack1->addStyle<WIDGET::BackgroundComponent> ();
background1->backGroundColor = Colors::Yellow;

stack->appendChild ( std::move ( stack1 ) );

auto stack2 = std::make_unique<WIDGET::Stack> ( render );
stack2->id = "stack 02";
auto style2 = stack2->addStyle<WIDGET::LayoutComponent> ();
auto background2 = stack2->addStyle<WIDGET::BackgroundComponent> ();
style2->left = Unit::px ( 0.0f );
style2->top = Unit::px ( 0.0f );
style2->display = WIDGET::Display::Block;
style2->width = Unit::px ( 60.0f );
style2->height = Unit::px ( 100.0f );
background2->backGroundColor = Colors::Blue;
style2->margin = Margin::px ( 0.0f );
style2->padding = Padding::px ( 0.0f );
style2->border = Border ( 0.0f, BorderStyle::Solid, Colors::Red );
stack->appendChild ( std::move ( stack2 ) );*/


/*auto stack3 = std::make_unique<WIDGET::Stack> ( render );
stack3->id = "stack 03";
auto style3 = stack3->addStyle<WIDGET::LayoutComponent> ();
auto background3 = stack3->addStyle<WIDGET::BackgroundComponent> ();
style3->left = Unit::px ( 0.0f );
style3->top = Unit::px ( 0.0f );
style3->display = WIDGET::Display::Inline;
style3->width = Unit::px ( 80.0f );
style3->height = Unit::px ( 100.0f );
style3->margin = Margin::px ( 10.0f );
background3->backGroundColor = Colors::Red;
stack->appendChild ( std::move ( stack3 ) );


auto stack4 = std::make_unique<WIDGET::Stack> ( render );
stack4->id = "stack 04";
auto style4 = stack4->addStyle<WIDGET::LayoutComponent> ();
auto background4 = stack4->addStyle<WIDGET::BackgroundComponent> ();
style4->left = Unit::px ( 0.0f );
style4->top = Unit::px ( 0.0f );
style4->display = WIDGET::Display::Block;
style4->position = Position::Static;
style4->width = Unit::AUTO ();
style4->height = Unit::AUTO();
style4->margin = Margin::px ( 10.0f );
background4->backGroundColor = COLOR::Color::fromHex ( "#090979" );


	auto stack4_A = std::make_unique<WIDGET::Stack>(render);
	stack4_A->id = "stack 04_A";
	auto style4_A = stack4_A->addStyle<WIDGET::LayoutComponent>();
	auto background4_A = stack4_A->addStyle<WIDGET::BackgroundComponent>();
	style4_A->left = Unit::px(0.0f);
	style4_A->top = Unit::px(0.0f);
	style4_A->display = WIDGET::Display::Block;
	style4_A->width = Unit::px(300.0f);
	style4_A->height = Unit::px(100.0f);
	background4_A->backGroundColor = Colors::Magenta;
	style4_A->margin = Margin::px(10.0f);

	stack4->appendChild(std::move(stack4_A));

stack->appendChild ( std::move ( stack4 ) );


auto stack5 = std::make_unique<WIDGET::Stack> ( render );
stack5->id = "stack 05";
auto style5 = stack5->addStyle<WIDGET::LayoutComponent> ();
auto background5 = stack5->addStyle<WIDGET::BackgroundComponent> ();
style5->left = Unit::px ( 0.0f );
style5->top = Unit::px ( 0.0f );
style5->display = WIDGET::Display::Block;
style5->width = Unit::pc(100.0f);
style5->height = Unit::px ( 250.0f );
style5->margin = Margin::px ( 10.0f );
background5->backGroundColor = Colors::Aqua;
stack->appendChild ( std::move ( stack5 ) );*/













/*
WIDGET::TextInfo textInfo { "arial", "Yanny Esteban" };
textInfo.x = 100.0f;
textInfo.y = 100.0f;
textInfo.width = 600.0f;
textInfo.height = 600.0f;

textInfo.scale = 40.0f;

auto labelOk = std::make_unique<WIDGET::Label> ( render, textInfo );
labelOk->id = "labelOk";
labelOk->addEventListener ( EventType::Click, []( const Event& event ) {
	std::cout << "Botón Aceptar presionado\n";
	return false;
	} );
gameInfo.widgetManager->appendChild ( std::move(labelOk) ); // <-error


WIDGET::TextInfo okButtonInfo { "arial", "Aceptar" };
okButtonInfo.x = 300.0f;
okButtonInfo.y = 400.0f;
okButtonInfo.width = 300.0f;
okButtonInfo.height = 100.0f;
okButtonInfo.scale = 10.0f;

auto okButton = std::make_unique<WIDGET::Button> ( render, okButtonInfo );
okButton->id = "okButton";
okButton->addEventListener ( EventType::Click,  []( const Event& event ) {
	std::cout << "Botón Aceptar presionado\n";
	return false;
	} );
okButton->addEventListener (EventType::MouseEnter, [](const Event& event) {
	std::cout << "Entrando al Botón\n";
	return false;

	} );
okButton->addEventListener ( EventType::MouseLeave, []( const Event& event ) {
	std::cout << "Saliendo del Botón\n";
	return false;

	} );
okButton->addEventListener ( EventType::DblClick, []( const Event& event ) {
	std::cout << "Presionando Doble click\n";
	return false;
	} );
okButton->addEventListener ( EventType::DragStart, []( const Event& event ) {
	std::cout << "Drag Start\n";
	return false;
	} );
okButton->addEventListener ( EventType::Drag, []( const Event& event ) {
	std::cout << "Drag ......\n";
	return false;
	} );
okButton->addEventListener ( EventType::DragEnd, []( const Event& event ) {
	std::cout << "Drag End\n";
	return false;
	} );

okButton->addEventListener ( EventType::Focus, []( const Event& event ) {
	std::cout << "Is Focus\n";
	return false;
	} );

okButton->addEventListener ( EventType::Blur, []( const Event& event ) {
	std::cout << "Is Blur\n";
	return false;
	} );


gameInfo.widgetManager->appendChild ( std::move ( okButton ) );
*/
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
		gameInfo.widgetManager->update ( deltaTime );
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
