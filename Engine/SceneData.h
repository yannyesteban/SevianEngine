#pragma once
#include <string>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include <iostream>
//#include "Vertex.h"
#include "Entity.h"
#include "Scene.h"
#include "models.h" 
//#include "Animation.h"
#include "TransformSystem.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace SEVIAN {
	

	void loadScene ( const std::string& file );
	void createActorEntity ( Scene& scene, const std::string& path );
}