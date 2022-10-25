#pragma once
#include "Scene.h"
#include <string>
#include <glm/glm.hpp>

class ModelColorScene : public Scene
{
public:
	ModelColorScene(std::string& filePath);

	virtual void Update() override;
	virtual void Draw(Camera* camera) override;

private:
	glm::vec3 modelColor;
};