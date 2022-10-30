#pragma once
#include "Scene.h"
#include <string>
#include <glm/glm.hpp>

class ModelTextureScene : public Scene
{
public:
	ModelTextureScene(std::string& filePath);

	virtual void Update() override;
	virtual void Draw(Camera* camera) override;

private:
	ShaderProgram* activeShader;

	glm::vec3 lightPositions[3];
	glm::vec3 lightColors[3];
	float lightIntensities[3];
};