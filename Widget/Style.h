#pragma once
#include <glm/glm.hpp> 

struct Style
{
	glm::vec3 backGroundColor;
	glm::vec3 color;
	float border;
	glm::vec3 borderColor;
	int borderStyle;
	float borderWidth;
	float borderRadius;

	float margin;
	float padding;
	float textShadow;
	float boxShadow;
	float zIndex;

	int textAlign;
	int verticalAlign;
	int display;
	int position;
	int overflow;


};