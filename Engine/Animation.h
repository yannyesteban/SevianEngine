#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
namespace SEVIAN {

	struct Bone
	{
		std::string name;
		glm::mat4 offsetMatrix; // Matriz de offset (transformaci�n del hueso en espacio local)
		glm::mat4 finalTransform; // Transformaci�n final despu�s de aplicar la animaci�n
		glm::mat4 localTransform;            // Transformaci�n local (posici�n, rotaci�n, escala respecto al padre)
		int id; // �ndice �nico para este hueso
		//glm::mat4 currentTransform; // Transformaci�n actual del hueso

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