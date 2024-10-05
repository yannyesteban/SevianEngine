#pragma once

#include "createSquareEntity.h"


namespace SEVIAN {

	std::shared_ptr<Entity> createCamera ( Scene& scene ) {
		auto entity = scene.createEntity ();
		// entity->addComponent<MaterialComponent> ( material );

		CameraComponent camera { };
		camera.position = glm::vec3 ( 0.1f, 0.0f, 5.0f );
		camera.front = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		camera.up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

		entity->addComponent<CameraComponent> ( camera );
		return entity;

	}

	std::shared_ptr<Entity> createModelEntity ( Scene& scene, const std::string& path, const std::string texture ) {
		auto entity = scene.createEntity ();

		entity->addComponent<ModelComponent> ( path, texture );
		entity->addComponent<PositionComponent> ( glm::vec3 ( 0.0, 0.0, 0.0 ) );
		entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
		entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
		entity->addComponent<InputComponent> ();
		entity->addComponent<NameComponent> ( Key::N4 );

		return entity;
	}

	std::shared_ptr<Entity> createSpriteEntity ( const Key& name, Scene& scene, const std::string& textureName, const std::string& textureFile, float deltaX, float deltaY, float deltaZ, float width, float height, glm::vec3 color ) {
		auto entity = scene.createEntity ();

		// Define vertices for a sprite (a single square plane) with the given width and height
		std::vector<Vertex> vertices = {
			{{-width / 2.0f, -height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, color}, // Bottom-left
			{{ width / 2.0f, -height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, color}, // Bottom-right
			{{ width / 2.0f,  height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, color}, // Top-right
			{{-width / 2.0f,  height / 2.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, color}  // Top-left
		};

		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0 // Two triangles forming a square
		};

		// Add components to the entity
		entity->addComponent<MeshComponent> ( vertices, indices );

		TextureComponent texture { textureName, textureFile };
		entity->addComponent<TextureComponent> ( texture );

		MaterialComponent material = MaterialComponent (
			{ 0.1f, 0.1f, 0.1f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 1.0f },
			32.0f
		);
		entity->addComponent<MaterialComponent> ( material );

		entity->addComponent<PositionComponent> ( glm::vec3 ( deltaX, deltaY, deltaZ ) );
		entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
		entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
		entity->addComponent<InputComponent> ();

		entity->addComponent<NameComponent> (name);


		SpriteComponent sprite = SpriteComponent ( glm::vec3 ( deltaX, deltaY, deltaZ ), width, height, textureName, color );
		entity->addComponent<SpriteComponent> ( sprite );
		

		return entity;
	}


	std::shared_ptr<Entity> createSpriteEntity ( Scene& scene, const std::string& textureName, const std::string& textureFile, float deltaX, float deltaY, float deltaZ ) {
		auto entity = scene.createEntity ();

		// Define vertices and indices for a sprite (a single square plane)
		/*std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}, // Bottom-left
			{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},  // Bottom-right
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},   // Top-right
			{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}   // Top-left
		};*/


		std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}, // Bottom-left
	{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}, // Bottom-right
	{{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}, // Top-right
	{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}  // Top-left
		};


		std::vector<uint32_t> indices = {
			// Two triangles forming a square
			0, 1, 2, 2, 3, 0
			//0, 2, 3, 0, 1, 2
		};

		// Add components to the entity
		entity->addComponent<MeshComponent> ( vertices, indices );

		TextureComponent texture { textureName, textureFile };
		entity->addComponent<TextureComponent> ( texture );

		MaterialComponent material = MaterialComponent (
			{ 0.1f, 0.1f, 0.1f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 1.0f },
			32.0f
		);
		entity->addComponent<MaterialComponent> ( material );

		entity->addComponent<PositionComponent> ( glm::vec3 ( deltaX, deltaY, deltaZ ) );
		entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
		entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
		entity->addComponent<InputComponent> ();

		return entity;
	}

	std::shared_ptr<Entity> createSquareEntity2 ( Scene& scene, std::string textureName, std::string textureFile, float deltaX, float deltaY, float deltaZ ) {
		auto entity = scene.createEntity ();

		// Define vertices and indices for a square
		std::vector<Vertex> vertices = {
			// Front face
		   {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-back
			{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Bottom-front
			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // Top-front
			{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}, // Top-back
		};

		std::vector<uint32_t> indices = {
			// Front face
			0, 1, 2, 2, 3, 0
		};

		// Add components to the entity
		entity->addComponent<MeshComponent> ( vertices, indices );

		TextureComponent texture { textureName, textureFile };
		entity->addComponent<TextureComponent> ( texture );

		MaterialComponent material = MaterialComponent (
			{ 0.1f, 0.1f, 0.1f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 1.0f },
			32.0f
		);
		entity->addComponent<MaterialComponent> ( material );

		entity->addComponent<PositionComponent> ( glm::vec3 ( deltaX, deltaY, deltaZ ) );
		entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
		entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
		entity->addComponent<InputComponent> ();
		return entity;
	}

	std::shared_ptr<Entity> createSquareEntity ( Scene& scene, float deltaX, float deltaY, float deltaZ ) {
		auto entity = scene.createEntity ();
		/* {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 color;
	};*/
	// Definir los vértices y los índices
		std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-left
			{{0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Bottom-right
			{{0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // Top-right
			{{-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}  // Top-left
		};

		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		// Agregar los componentes a la entidad
		entity->addComponent<MeshComponent> ( vertices, indices );

		TextureComponent texture { "diffuse", "textures/a1.jpeg" };
		//texture.type = "diffuse";
		//texture.path = "textures/a1.jpeg";
		entity->addComponent<TextureComponent> ( texture );

		MaterialComponent material = MaterialComponent (
			{ 0.1f, 0.1f, 0.1f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 1.0f },
			32.0f
		);
		entity->addComponent<MaterialComponent> ( material );

		entity->addComponent<PositionComponent> ( glm::vec3 ( 0.0f, 0.0f + deltaY, 0.0f + deltaZ ) );
		entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f + deltaX, 0.0f ) );
		entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );

		return entity;
	}


	std::shared_ptr<Entity> createCubeEntity ( const std::string& textureName, const std::string& textureFile, const Key& name, Scene& scene, float deltaX, float deltaY, float deltaZ ) {
		auto entity = scene.createEntity ();

		glm::vec3 color1 = {(float)5/255, (float) 51/255, (float) 255/255 };
		glm::vec3 color2 = { (float) 255 / 255, (float) 255 / 255, (float) 51 / 255 };
		glm::vec3 color3= { (float) 96 / 255, (float) 96 / 255, (float) 96 / 255 };
		glm::vec3 color4 = { (float) 204 / 255, (float) 0 / 255, (float) 0 / 255 };
		glm::vec3 color5 = { (float) 0 / 255, (float) 255 / 255, (float) 0 / 255 };
		glm::vec3 color6 = { (float) 32 / 255, (float) 32 / 255, (float) 32 / 255 };
		/*
		float a = -0.5;
		float b = -0.5;
		float c = 0.5;
		if (textureName == "a") {
			a = -1.5;
		}
		if (textureName == "b") {
			b = -1.5;
		}
		if (textureName == "c") {
			c = 1.5;
		}
		*/
		// Define vertices and indices for a cube
		std::vector<Vertex> vertices = {
			// Frente //{-0.5f, -0.5f,  0.5f}
			{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, color1 }, // Abajo-izquierda
			{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}, color1}, // Abajo-derecha
			{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, color1}, // Arriba-derecha
			{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}, color1}, // Arriba-izquierda

			// Atrás
			{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, color2}, // Abajo-izquierda
			{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}, color2}, // Abajo-derecha
			{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, color2}, // Arriba-derecha
			{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}, color2}, // Arriba-izquierda

			// Izquierda
			{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, color3}, // Abajo-izquierda
			{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, color3}, // Abajo-derecha
			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, color3}, // Arriba-derecha
			{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, color3}, // Arriba-izquierda

			// Derecha
			{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, color4}, // Abajo-izquierda
			{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, color4}, // Abajo-derecha
			{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, color4}, // Arriba-derecha
			{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, color4}, // Arriba-izquierda

			// Arriba
			{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}, color5}, // Abajo-izquierda
			{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, color5}, // Abajo-derecha
			{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}, color5}, // Arriba-derecha
			{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, color5}, // Arriba-izquierda

			// Abajo
			{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}, color6}, // Abajo-izquierda
			{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, color6}, // Abajo-derecha
			{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}, color6}, // Arriba-derecha
			{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, color6}, // Arriba-izquierda
		};

		std::vector<uint32_t> indices = {
			// Front face
			0, 1, 2, 2, 3, 0,
			// Back face
			6, 5, 4, 4, 7, 6,
			// Left face
			8, 9, 10, 10, 11, 8,
			// Right face
			14, 13, 12, 12, 15, 14,
			// Top face
			16, 17, 18, 18, 19, 16,
			// Bottom face
			22, 21, 20, 20, 23, 22
		};

		// Add components to the entity
		entity->addComponent<MeshComponent> ( vertices, indices );

		//TextureComponent texture { "diffuse1", "textures/a1.jpeg" };
		TextureComponent texture { textureName, textureFile };
		entity->addComponent<TextureComponent> ( texture );

		MaterialComponent material = MaterialComponent (
			{ 0.1f, 0.1f, 0.1f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f, 1.0f },
			32.0f
		);
		entity->addComponent<MaterialComponent> ( material );

		entity->addComponent<PositionComponent> ( glm::vec3 ( 0.0f + deltaX, 0.0f + deltaY, 0.0f + deltaZ ) );
		entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
		entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
		entity->addComponent<InputComponent> ();
		entity->addComponent<NameComponent> ( name );
		return entity;
	}


}