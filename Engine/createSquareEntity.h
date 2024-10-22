#pragma once
#include <memory>
#include "models.h" 
#include "Scene.h"
#define _USE_MATH_DEFINES
#include <math.h>
namespace SEVIAN {

    std::shared_ptr<Entity> createCamera ( Scene& scene );

    std::shared_ptr<Entity> createModelEntity ( Scene& scene, const std::string& path, const std::string texture );

    std::shared_ptr<Entity> createSpriteEntity ( const Key & name, Scene& scene, const std::string& textureName, const std::string& textureFile, float deltaX, float deltaY, float deltaZ, float width, float height, glm::vec3 color );

    std::shared_ptr<Entity> createSpriteEntity ( Scene& scene, const std::string& textureName, const std::string& textureFile, float deltaX, float deltaY, float deltaZ );

    std::shared_ptr<Entity> createSquareEntity2 ( Scene& scene, std::string textureName, std::string textureFile, float deltaX, float deltaY, float deltaZ );

    std::shared_ptr<Entity> createSquareEntity ( Scene& scene, float deltaX, float deltaY, float deltaZ );

    std::shared_ptr<Entity> createCubeEntity ( const std::string& textureName, const std::string& textureFile, const Key& name, Scene& scene, float deltaX, float deltaY, float deltaZ );
	std::shared_ptr<Entity> createCubeEntity2 ( const std::string& textureName, const std::string& textureFile, const Key& name, Scene& scene, float deltaX, float deltaY, float deltaZ );

	std::shared_ptr<Entity> createSphereEntity (
		const std::string& textureName,
		const std::string& textureFile,
		const Key& name,
		Scene& scene,
		float deltaX,
		float deltaY,
		float deltaZ,
		float radius,
		int sectorCount,
		int stackCount );
}