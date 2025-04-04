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

#define MAX_BONES 64

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

		int boneIDs[4];       // IDs de huesos que influyen en el vértice (máximo 4)
		float boneWeights[4]; // Pesos asociados a cada hueso

		bool operator==( const Vertex& other ) const {
			return position == other.position && color == other.color && texCoords == other.texCoords;
		}
	};



	struct VertexTextOld
	{
		glm::vec2 pos;
		glm::vec2 texCoord;
		int color = 1;

	};

	struct UniformDataDept
	{
		glm::mat4 MVP;
	};
	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::mat4 MVP;
		alignas(16) glm::mat4 lightView;
		alignas(16) glm::mat4 lightProj;
		
		alignas(16) glm::mat4 boneTransforms[MAX_BONES]; // Coloca las matrices juntas
		alignas(16) glm::vec3 outColor;  // Vec3 debe ir al final para evitar relleno innecesario
		alignas(16) glm::vec3 lightPos;  // Vec3 debe ir antes del float o rellenará espacio
		alignas(4) float lightIntensity;
		alignas(16) glm::vec3 lightColor;
		alignas(16) glm::vec3 cameraPos;
		
	};


	struct StyleUbo
	{
		alignas(16) glm::vec2 size;
		alignas(16) glm::vec4 backgroundColor;
		alignas(16) glm::vec4 borderColor;
		alignas(4) float borderWith = 0.0f;
		alignas(4) float borderRadius = 0.0f;
		int useTexture = 0;
	};
	

	struct UniformBufferObjectX
	{
		
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		
		glm::mat4 MVP;

		glm::mat4 lightView;  // Añadido: Matriz de vista desde la luz
		glm::mat4 lightProj;  // Añadido: Matriz de proyección desde la luz
		glm::mat4 boneTransforms[MAX_BONES];


		glm::vec3 lightPos; // Posición de la luz
		glm::vec3 outColor;

		glm::vec3 lightColor;
		glm::vec3 cameraPos;  // Añadimos la posición de la cámara
		
		float lightIntensity;
		
		
	};

	struct BoneUBO
	{
		glm::mat4 boneTransforms[MAX_BONES]; // Matrices finales de los huesos
	};

	struct UniformBufferObject1
	{

		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;


		alignas(16) glm::mat4 lightView;  // Añadido: Matriz de vista desde la luz
		alignas(16) glm::mat4 lightProj;  // Añadido: Matriz de proyección desde la luz
		alignas(16) glm::vec3 lightPos; // Posición de la luz
		alignas(16) glm::vec3 lightColor;
		alignas(16) glm::vec3 cameraPos;  // Añadimos la posición de la cámara

		alignas(4) float lightIntensity;

	};

	struct UniformBufferObjectX2
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
		glm::vec3 front = glm::vec3 ( 1.0f );
		glm::vec3 up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

		float zoomLevel = 45.0f;

		float yaw = -90.0f;
		float pitch = 0.0f;
		float sensitivity = 0.1f;


	};

	class Camera2D: Camera
	{
	public:
		Camera2D ( float screenWidth, float screenHeight ) {
			updateProjection ( screenWidth, screenHeight );
			updateView ();
		}

		void updateProjection ( float screenWidth, float screenHeight ) {
			// Sistema de coordenadas: origen en esquina superior izquierda, Y hacia abajo

			

			m_projection = glm::ortho (
				0.0f,        // left
				screenWidth, // right
				0.0f,        // top
				screenHeight, // bottom (para Y invertido)
				-1.0f, 1.0f  // near/far
			);

			//m_projection[1][1] *= -1;
		}

		void updateView () {
			m_view = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( -m_position, 0.0f ) );
			m_view = glm::scale ( m_view, glm::vec3 ( m_zoom, m_zoom, 1.0f ) );
		}

		// Setters
		void setPosition ( const glm::vec2& position ) {
			m_position = position;
			updateView ();
		}

		void setZoom ( float zoom ) {
			m_zoom = zoom;
			updateView ();
		}

		// Getters
		const glm::mat4& getViewProjection () const {
			return m_projection * m_view;
		}
		const glm::mat4& getView () const {
			return m_view;
		}
		const glm::mat4& getProjection () const {
			return m_projection;
		}

	private:
		glm::mat4 m_projection;
		glm::mat4 m_view;
		glm::vec2 m_position = { 0.0f, 0.0f };
		float m_zoom = 1.0f;
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

	struct Sprite2DInfo
	{
		std::string texture;
		glm::vec2 position = glm::vec2 ( 0.0f, 0.0f );
		glm::vec2 size = glm::vec2 ( 0.0f, 0.0f );
		glm::vec3 color = glm::vec3 ( 0.0f, 0.0f, 0.0f );

	};

	struct SpriteInfo
	{
		std::string texture;
		glm::vec3 position = glm::vec3 ( 0.0f, 0.0f, 0.0f );

		glm::vec2 size = glm::vec2 ( 0.0f, 0.0f );
		glm::vec3 color = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		glm::vec3 scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
		glm::vec3 rotation = glm::vec3 ( 1.0f, 1.0f, 1.0f );
		
	};

	struct MeshInfo
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::string texture;
	};

	struct GeometricInfo
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::string texture;
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