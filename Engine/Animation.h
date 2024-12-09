#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
namespace SEVIAN {

	struct Bone
	{
		std::string name;
		glm::mat4 offsetMatrix; // Matriz de offset (transformación del hueso en espacio local)
		glm::mat4 finalTransform; // Transformación final después de aplicar la animación
		glm::mat4 localTransform;            // Transformación local (posición, rotación, escala respecto al padre)
		int id; // Índice único para este hueso
		//glm::mat4 currentTransform; // Transformación actual del hueso

		int parentIndex;
	};

	struct Skeleton
	{
		std::unordered_map<std::string, Bone> bones; // Mapear nombre del hueso a su estructura
		std::vector<glm::mat4> boneTransforms; // Transformaciones finales para pasar al shader
	};

	struct KeyFrame
	{
		float time;
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;


	};


	struct AnimationChannel
	{
		std::string nodeName;
		std::vector<KeyFrame> positionKeys;
		std::vector<KeyFrame> rotationKeys;
		std::vector<KeyFrame> scaleKeys;


	};

	struct Animation
	{
		std::string name;
		float duration;
		float ticksPerSecond;
		std::vector<AnimationChannel> channels;
	};

	struct BoneAnimation
	{
		std::string boneName; // Nombre del hueso
		std::vector<KeyFrame> keyFrames; // Lista de keyframes
	};



	
}