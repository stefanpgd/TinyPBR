#pragma once
#include "Scene.h"
#include <glm/glm.hpp>

class SphereColorScene : public Scene
{
public:
	SphereColorScene();

	virtual void Update() override;
	virtual void Draw(Camera* camera) override;

private:
	glm::vec3 albedo = glm::vec3(1.0f);
	float metallic = 0.5f;
	float roughness = 0.5f;

	glm::vec3 lightPosition;
	glm::vec3 lightColor;
};