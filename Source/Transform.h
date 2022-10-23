#pragma once
#include <glm/glm.hpp>
#include <string>

class Transform
{
public:
	Transform();
	
	void DebugDrawImGui();
	glm::mat4& GetModelMatrix();

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

private:
	void DrawVector3Edit(const std::string name, glm::vec3& data, float resetValue, float precision = 0.1f);

	glm::mat4 model;
};