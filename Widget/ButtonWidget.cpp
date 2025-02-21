#include "ButtonWidget.h"
namespace SEVIAN::WIDGET {



    
    Button::Button ( std::shared_ptr<RENDERER::RenderInterface> render, TextInfo info ): 
        spriteManager ( render->getManager<RENDERER::iSpriteManager> () ) {

        position.x = info.x;
        position.y = info.y;
        size.x = info.width;
        size.y = info.height;

        SEVIAN::SpriteInfo spriteInfo { "escultura", {info.x, info.y, 0.0}, {info.width, info.height}, {0.9f, 0.1f, 2.0f} };
        object = spriteManager->createSprite ( spriteInfo );
    }

    void Button::render () {
    }

    void Button::update ( float deltaTime ) {
    }

    std::shared_ptr<RENDERER::IRenderizable> Button::getRenderObject () {
        return object;
    }

}
