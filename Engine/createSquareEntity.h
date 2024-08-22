#pragma once
#include <memory>
#include "models.h" 
#include "Scene.h"


namespace SEVIAN {

    std::shared_ptr<Entity> createCamera ( Scene& scene );

    std::shared_ptr<Entity> createSpriteEntity ( const Key & name, Scene& scene, const std::string& textureName, const std::string& textureFile, float deltaX, float deltaY, float deltaZ, float width, float height );

    std::shared_ptr<Entity> createSpriteEntity ( Scene& scene, const std::string& textureName, const std::string& textureFile, float deltaX, float deltaY, float deltaZ );

    std::shared_ptr<Entity> createSquareEntity2 ( Scene& scene, std::string textureName, std::string textureFile, float deltaX, float deltaY, float deltaZ );

    std::shared_ptr<Entity> createSquareEntity ( Scene& scene, float deltaX, float deltaY, float deltaZ );

    std::shared_ptr<Entity> createCubeEntity ( Scene& scene, float deltaX, float deltaY, float deltaZ );


}