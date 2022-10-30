#pragma once
#include "Scene.h"
#include "../Texture.h"
#include <string>
#include <glm/glm.hpp>

struct ModelSelectTextureData
{
	std::string modelPath;
	std::string albedoPath;
	std::string normalPath;

	std::string metallicPath;
	unsigned int metallicChannel;

	std::string roughnessPath;
	unsigned int roughnessChannel;

	std::string ambientOcclusionPath;
	unsigned int aoChannel;
};

class ModelSelectTextureScene : public Scene
{
public:
	ModelSelectTextureScene(ModelSelectTextureData data);

	virtual void Update() override;
	virtual void Draw(Camera* camera) override;

private:
	ModelSelectTextureData modelData;

	ShaderProgram* activeShader;
	std::vector<Texture*> textures;

	glm::vec2 texCoordMultiplier = glm::vec2(1.0f, 1.0f);
	glm::vec3 lightPositions[3];
	glm::vec3 lightColors[3];
	float lightIntensities[3];
};