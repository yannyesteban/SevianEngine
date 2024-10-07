#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <vector>
#include <string>




//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>

namespace SEVIAN {

	struct TextureInfo
	{
		std::string name;
		std::string path;
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 color;

		bool operator==( const Vertex& other ) const {
			return position == other.position && color == other.color && texCoords == other.texCoords;
		}
	};



	struct VertexText
	{
		glm::vec2 pos;
		glm::vec2 texCoord;

	};

	struct UniformBufferObject
	{
		
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::vec3 position; // Posición de la luz
		alignas(16) glm::vec3 cameraPos;  // Añadimos la posición de la cámara
		
	};

	struct UniformBufferObjectX
	{
		alignas(16) glm::vec3 color;
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::vec3 color2;

		alignas(4) float zoom;
		alignas(16) glm::vec3 position;
	};

	struct MeUBO
	{

		float intensity;    // Intensidad de la luz

		alignas(16) glm::vec3 color;    // Color de la luz


		alignas(16) glm::vec3 color2;    // Color de la luz

	};

	struct LightUBO
	{

		alignas(16) glm::vec3 color;    // Color de la luz
		alignas(4) float intensity;    // Intensidad de la luz
		alignas(16) glm::vec3 position; // Posición de la luz

	};

	struct LightUBO2
	{
		alignas(16) glm::vec3 position; // Posición de la luz
		alignas(16) glm::vec3 color;    // Color de la luz
		alignas(4)  float intensity;    // Intensidad de la luz
		// Añadir más propiedades si es necesario, como dirección para luces direccionales
	};
	struct Camera
	{
		glm::vec3 position = glm::vec3 ( 0.0f, 0.0f, 5.0f );
		glm::vec3 front;
		glm::vec3 up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

		float zoomLevel = 45.0f;

		float yaw = -90.0f;
		float pitch = 0.0f;
		float sensitivity = 0.1f;
	};


	struct Model3D
	{
		uint32_t entityId;
		std::string model;
		std::string texture;

	};

	struct Info3D
	{
		uint32_t entityId;
		uint32_t type;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::string texture;
		std::string path;

		TextureInfo text;
	};


	struct Sprite3D
	{
		uint32_t entityId;
		glm::vec3 position = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		float width = 1.0;
		float height = 1.0;
		glm::vec3 color = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		//uint32_t type;
		float x;
		float y;
		float z;
		//std::vector<Vertex> vertices;
		//std::vector<uint32_t> indices;

		std::string texture;
		std::string path;
	};
}


namespace std {
	template<> struct hash<SEVIAN::Vertex>
	{
		size_t operator()( SEVIAN::Vertex const& vertex ) const {
			return ((hash<glm::vec3> ()(vertex.position) ^ (hash<glm::vec3> ()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2> ()(vertex.texCoords) << 1);
		}
	};
}