#pragma once


#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace SEVIAN {

    

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 color;
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
    };

    struct Camera
    {
        glm::vec3 position = glm::vec3 ( 0.0f, 0.0f, -5.0f );
        glm::vec3 front;
        glm::vec3 up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

        float zoomLevel = 45.0f;

        float yaw = -90.0f;
        float pitch = 0.0f;
        float sensitivity = 0.1f;
    };


    struct Info3D
    {
        uint32_t entityId;
        uint32_t type;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::string texture;
        std::string path;
    };


    struct Sprite3D
    {
        uint32_t entityId;
        glm::vec3 position = glm::vec3 ( 0.0f, 0.0f, 0.0f );
        float width = 1.0;
        float height = 1.0;
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